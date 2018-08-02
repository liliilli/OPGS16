# 테스트 AABB 렌더링 실장하기

> 2018-07-28 문서 작성
> 2018-07-31 FixDrawAABBTest 로 문서 이동.

## 문제

2018년 7월 29일 17시 28분 기점으로 `bullet3` 엔진을 일단 실장하는 데는 성공을 했으나, 해당 컬리젼 박스의 AABB 을 묘화를 하지 못했지 때문에 AABB 박스가 어떻게 움직이는 가를 알 수가 없다. 따라서 이를 2차원 그리고 3차원에 `GL_LINE_STRIP` 등을 써서 구현을 하고자 한다.

## 제안

우선 각 `Collider` 마다 `C__AABBRenderer` 라는 **내부** 컴포넌트를 만들어서, 이를 오브젝트에 바인딩하거나 아니면 `Collider`가 `Update` 시에 내부 컴포넌트의 로직을 호출할 수 있도록 한다.

`C__AABBRenderer` 는 각자 `2D` 혹은 `3D` 을 가지고 있는데, 이 타입의 분류는 부모가 되는 혹은 소유주가 되는 `Collider` 에서 어떤 타입의 컬리젼 셰이프를 만들었는가에 따라 달라진다. 예를 들어서 `Box2dShape` 라면 `2D` 로, 그렇지 않고 `BoxShape` 라면 `3D` 가 된다.

**`C__AABBRenderer` 는 현재 디버그 모드가 아니라면 생성을 하지 않는다. 이는 `manifest.h` 등에서 매크로를 활용한 조정이 필요하다. 그리고 `Collider` 컴포넌트가 삭제가 되면 `C__AABBRenderer` 컴포넌트도 자동으로 사라진다. 뿐만 아니라 `Collider` 가 Deactivate 될 경우에는 `C__AABBRenderer` 역시 Deactivate 된다. 반대 역시 반대의 효과를 가진다.**

생각을 해보니까 `Collider` 안쪽에 내부 컴포넌트를 집어넣어서, `Update()` 로 `C__AABBRenderer` 을 렌더링할 수 있도록 후보군에 넣는 것이 더 좋을 것 같다. 왜냐하면 `CComponent` 를 삭제하기 위해서는 일렬 리스트에서 각 컴포넌트의 고유 해쉬값을 사용해서 제거를 하는데, 만약 `CObject` 의 컴포넌트 리스트에 `C__AABBRenderer` 가 복수개 존재한다면 바인딩되지 않은 렌더러를 지워서 런타임 에러로 직행할 수도 있기 때문이다.

이 대신에 `Update()` 시에 활성화된 컴포넌트에 한해 무조건 `Update()` 가 수행되기 때문에, 내부에 `C__AABBRenderer` 에 대한 포인터 변수를 사용해 동작하게 끔 한다. 그것이 더 관리하하기도 더 좋고.

#### AABB 표시 색

* `Collider` Actv , `Rigidbody` Actv / Kinetic :: 바다색 (Aqua)
* `Collider` Actv , `Rigidbody` Actv / Dynamic :: 연두색 (Green)
* `Collider` Actv , `Rigidbody` Deact or NotExist :: 주황색 (Orange)
* `Collider` 충돌 시 :: 노랑색 (Yellow)
* `Collider` 연산 제외 및 휴식 (Sleep) 시 :: 연회색 (Gray)

#### 어떻게 `Render()` 에서 렌더링하게 할 것인가?

`OPGS16` 의 각 오브젝트가 `Update()` 될 때 해당 `CObject` 의 컴포넌트 역시 `Update()` 함수를 가지고 있어, 컴포넌트가 비활성화 되지 않은 상황에서는 리스트의 컴포넌트가 하나씩 `Update(delta_time)` 이 수행이 된다.

이 때 `core_setting` 에서 디버그 모드와 AABB 세팅이 켜져있는 가에 따라 `rendering_manager` 와 같은 곳에 `C__AABBRenderer` 의 포인터를 집어넣도록 한다. 이 때 `C__AABBRenderer` 에는 AABB 의 좌표 정보를 가지고 있고, 일반 `opQuad2d` 을 렌더링할 때 처럼 ShaderWrapper 역시 구현하고 있다.

다만 `C__AABBRenderer` 는 `CObject` 가 `Render()` 함수에서 직접 변수를 만져주는 것과는 다르게 해당 씬의 메인 카메라 오브젝트를 따라 자동으로 유니폼 변수를 넣고 렌더링을 한다. `glMap` 등을 활용해서 `VBO` 정보는 만지지 않는다.

또는 회전, 스케일, 이동 등이 모두 포함된 `btTransform` 을 저장해서 `C__AABBRenderer` 의 Model 매트릭스로 해서 구현을 하도록 한다.

#### `C__AABBRenderer` 파일을 어떻게 만들까?

우선 `Collider` 의 성질에 따라서 `2D` 인가 `3D` 인가 달라지고, 다른 타입으로 변경되지 않고, 렌더링 시에도 각기 다른 타입에 따라서 다르게 렌더링이 된다. 하지만 색상 변경 및 렌더링 자체는 (쉐이더가 다를지언정) 메커니즘을 공유한다.

* `Collider2D` 인 경우 `C__AABBRenderer2D` 을 사용한다.
* `Collider3D` 인 경우 `C__AABBRenderer3D` 을 사용한다.

메커니즘을 공유하는 부분에서는 객체를 생성할 수 없는 `C__AABBRendererBase` 을 만들어서 사용한다. `2D` 와 `3D` 타입은 이로부터 상속을 통해서 구현을 한다.

## 구현 사항

#### `CPrivateAabbRenderer`

* 쉐이더는 고정, VAO 역시 기존에 빌트인으로 주어진 것을 활용한다.
* 사이즈 역시 `Set` `Get` 함수가 있으며, 트랜스폼은 항상 `FinalPosition` 과 `Wp? FinalRotation` 만을 받는다. 사이즈가 이미 주어져 있기 때문에 사용하지 않아도 됨.
* `CPrivateAabbRenderer` 는 `Update` 함수를 가지고 있고, 자체 `Render` 함수를 가지고 있다. 이 `Render` 함수에서 렌더링을 하게끔 한다. 즉,...
  1. `CProtoRigidbodyCollider2D::Update()` 와 같은 `CCollider::Update()` 에서 업데이트를 한다. 정보를 다 처리하고, 컬라이더 혹은 컬라이더의 변수에 바인딩 된 `CPrivateAabbRenderer` 을 렌더링할 수 있도록 `Update()` 을 호출한다.
  2. ~~`Update()` 을 호출하기 전에, `Position` `Rotation` 등을 업데이트 한다. 이 때 `Size` 업데이트는 하지 않는다. 왜냐면 각 컬라이더에 대해서 크기는 고정되어 있으며, 만약 컬라이더의 크기가 변경될 때에만 렌더러에 크기를 갱신하게 할 수 있도록 한다.~~
     * ~~현재 `Position` 만 업데이트 하게한다. `Rotation` 은 X.~~
     * AABB 정보를 그대로 받아서 렌더링하게 했다.
  3. `CPrivateAabbRenderer` 의 공통 `Update()` 에서는 만약 Transform 정보가 변경됬을 경우 `ModelMatrix` 을 갱신하고, `manager::object::InsertAABBInformation` 에 해당 렌더러를 나중에 렌더 프레임에서 렌더링할 수 있도록 자기 자신의 포인터를 집어넣는다.
  4. `InsertAABBInformation` 안에서는 해당 렌더러의 타입이 2D인가 3D 인가를 확인해서 별도의 렌더링 후보군 리스트에 집어넣는다. 만약 `None` 인 경우에는 어설트 에러를 발생시킨다.
  5. `CPrivateAabbRendererBase*` 은 `Render()` 가 가상 함수이기 때문에 직접 호출을 할수 없지만, `2D` 와 `3D` AABB 렌더러에서는 함수 바디를 구현해서 렌더링할 수 있도록 한다.

#### `CPrivateAabbRenderer2D`

* `2D` `AABB` 에 대해서 렌더링을 하기 위해서는...
  1. 4개의 정점을 가진 빌트인 VAO 가 필요하다.
     * 그런데 빌트인 VAO 을 사용하기 위해서는, `Indice` 가 없는 새로운 `VAO` 을 만들어야 한다. `BModel2DQuadLine` 이라고 명명하자.
  2. `Projection` `View` 및 `Model` 과 `Color` , 그리고 `opPosition` 을 받아 `GL_LINEAR_STRIP` 으로 렌더링할 수 있는 쉐이더가 필요하다.
     * 텍스쳐는 필요하지 않다.

#### `BModel2DQuadLine`

* `BModel2DQuadLine` 은 `Indice` 가 없는 시계 방향의 정점 4 개로 구성된 하나의 메쉬를 가진다.

* 해당 모델의 이름은 `opbtQuadLine` 으로 하자.

* 그리고 이 모델을 `Builtin` 에서 불러와서 쓰기 위해서는 `opgs16::manager::mesh` 에서 초기화할 때 이를 불러올 수 있도록 해야한다.

  > opgs16::manager::mesh 가 model 이 아니라 mesh 인 것은 네이밍 실수. 이것도 고쳐야 함.

  불러오도록 한 후에는, `vao` 을 만들 수 있도록 내부 `vao_management` 의 `vao` 컨테이너에 붙여 넣어줘야 한다.

#### Shader, opQuad2DLineLoop

* 이름 그대로 4개의 정점을 선형으로 `GL_LINE_LOOP` 로 루핑하면서 진행한다.

#### `IsEnableRenderingAabb` 바인딩하기

* `core_setting.h` 에서 바인딩을 한다.

> 문제 발견. 렌더러에서 VAO 증가시키는 것은 있는데, 렌더러 컴포넌트가 소멸하면서 VAO 을 감소하는 것은 없음. 이거 무어ㅣㅁ?

## 결과

``` c++
// mesh_manager.cc

void InitiateBuiltinModelMeshes() {
  // ...
  using opgs16::builtin::model::BModel2DQuadLine;
  // ...
  m_model_map.try_emplace(BModel2DQuadLine::s_model_name, BModel2DQuadLine{});
  // ...
}
```

``` c++
// vao_management.cc

void InitiateBuiltinVaoItems() {
  // ...
  using opgs16::builtin::model::BModel2DQuadLine;
  GenerateVaoContainer(BModel2DQuadLine::s_model_name,
                       EVboBufferType::StaticDraw,
                       EEboBufferType::StaticDraw);
}
```

그리고 일단 AABB 을 렌더링하는 것은 성공했다. 하지만 리팩터링 등의 여지가 남아있다...

#### More

`CPrivateAabbRenderer2D` 의 소멸자에서 VAO 의 레퍼런스 카운팅을 감소시키도록 했다.

#### 알아낸 것

`btBox2dShape` 혹은 다른 축의 사이즈에 비해서 한 축이 비정상적으로 작은 경우에 관성이 적용되어 있으면, 의도한 동작을 하지 않을 가능성이 있다.

![1533002662115](D:\Development\OPGS16\Docs\Media\1533002662115.png)

#### 2018-07-31 PM 10:00

`FeatDrawAABBTest` 완료. core_setting 에 따라서 Aabb 을 렌더링하게 할지 말아야 할지를 구현 해야할 것 같다.

그 외에도 글로벌 설정으로 `Aabb` 을 끄고 킬 수 있도록 했다. `Aabb` 컴포넌트 자체는 생성이 되지만, 이걸 렌더링하는 것은 하지 않는다. 왜 생성 자체를 하느냐고 물어보면, 리얼타임에서  껐다가 켰다가를 할 수도 있기 때문이다. 만약 틀리다면 나중에 생성까지 하지 않도록 하면 되니까...

``` powershell
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   OPGS16/Source/Component/Internal/aabb_renderer_base.cc
        modified:   OPGS16/Source/Component/Physics/prot_rigidbody_collider2d.cc
        modified:   OPGS16/Source/Manager/Internal/physics_environment.cc
        modified:   OPGS16/Source/Manager/object_manager.cc

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        Docs/Media/0723_resolution.png
        Docs/Media/0724_fixstretch1.png
        Docs/Media/0724_fixstretch2.png
        Docs/Media/1532386335513.png
        Docs/Media/1533002662115.png
        Docs/Media/img23_preprecomp-down.PNG
        Docs/Media/resolution_comparaison.png
        Docs/Structure/0.2.0/
        OPGS16/Include/Helper/Type/axis.h
        OPGS16/Include/Helper/include_vector.h
        OPGS16/setting.sav

no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "refc: Add (6) set core_setting Aabb rendering option to Renderer"
[feat_aabb_color ff554ed] refc: Add (6) set core_setting Aabb rendering option to Renderer
 4 files changed, 26 insertions(+), 35 deletions(-)
```

