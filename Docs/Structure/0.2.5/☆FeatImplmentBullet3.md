# Physics Engine Bullet3 도입

> 2018-07-28 문서 작성
> version 0.1.0

## 문제

게임 엔진에 있어서 물리 엔진은 가장 필수적인 요소 중 하나이다. 다만 물리 엔진을 처음부터 만들어서 적용하기에는 구현해야 할 것들도 많거니와 오픈소스로 제공되어 있는 여러 물리엔진에 비해서 퀄리티가 매우 떨어진다.

물리 엔진을 적용하지 않은 게임들은 퍼즐류와 같은 간단한 게임들이 있으나, 조금만 장르를 확장해도 물리 엔진은 게임에 있어서 필수적이다. 특히나 플랫폼 게임, 지형 인식, 효과 등등에서 물리 엔진이 쓰이지 않은 곳은 없다.

또한 물리 엔진은 충돌체들의 충돌 판정을 해결하는 데도 사용할 수 있다. 왜냐면 컴퓨터 상에서 물리 효과를 각 충돌체에 반영하기 위해선 충돌이 기본적으로 해결이 되어야 하기 때문이다.

## 제안

1. 물리 엔진 `bullet3` 을 도입한다. 브랜치의 이름은 `feat_bullet3` 이라고 하자.
2. 일단 여기저기 흩어져 있는 `physics` 관련 오브젝트들을 세부 폴더에 다 같이 넣는다. `Physics2D` 폴더 등은 삭제하고 통합한다.
3. Bullet3 을 사용해서 우선 겉으로 1 리기드바디에 1 콜라이더가 붙을 수 있는 형식으로 구현을 한다.
   * 다중 콜라이더는 나중에 구현한다.
4. `Bullet3` 이 알아차릴 수 있는 `opgs16::component::CRigidbody` 컴포넌트를 만든다.

![img](https://docs.unity3d.com/uploads/Main/Inspector-Rigidbody.png) 

구현은 Unity 5 의 `Rigidbody` 을 사용한다.

5. `Bullet3` 이 알아차릴 수 있는 `opgs16::component::CCollisionShape` 컴포넌트를 만들고, 그걸 베이스 삼아서 `opgs16::component::CColliderBox2D` 을 만든다. (우선 이것부터 만들고 테스트한다)

![img](https://docs.unity3d.com/uploads/Main/Inspector-BoxCollider.png) 

5. `CColliderShape` 컴포넌트는 `RIgidbody` 가 있는가 없는가, 활성화 되있는가 안 되어있는가를 `Update()` 에서 확인해서 해당 컴포넌트를 `Static` 전환하거나 `Dynamic`혹은 `Kinetic` 으로 전환하게 하거나 할 수 있다. 이 역시 상당한 플래그가 필요할 것 같다...

## 구현 사항

### `opgs16::component::CRigidbody` 의 구현 사항

1. `Mass` :: 전체 Collider 들의 최종 질량을 나타낸다. $$ 0 $$ 인 경우에는 0.001 으로 고정이 되며 이는 `bullet3` 에서는 `Dynamic` 일 때에도 질량을 가져야만 움직일 수 있기 때문이다. 
2. `IsKinematic` 플래그 :: 움직이지는 않으나 런타임 생성 후에도 임의로 위치를 지정할 수 있다. `Kinematic` 인 경우에는 키 인풋 등에 의해서 움직인다고 하더라도 `Physics` 의 업데이트 혹은 컴포넌트의 업데이트에서 자동으로 위치를 보정해줘야 한다.
3. `Convex` 플래그 :: Collider 가 Convex 한가 아닌가를 설정한다. `bullet3` 에서는 `Convex` 인가 `Concave` 한가에 따라서 연산이 달라지기 때문에 필수적이다.  하지만 기본 메쉬를 활용한 `Collider` 에서는 이 플래그를 만들 필요가 없을 것이다.
4. `Tag` :: 충돌 레이어를 지정한다.  이건 필수.

### `opgs16::component::CCollisionShape` 의 구현 사항

### `opgs16::component::CColliderBox2D` 의 구현 사항



### 프로토타입 `opgs16::component::CProtRigidbodyCollider2D` 의 구현

1. 우선 `CRigidbody` 와 `CBox2D`가 결합된 형태를 구현해서 빨리 시험을 해본다. 
2. `RigidbodyCollider2D` 에 접목할 `CollisionShape` 는 `btBox2dShape` 이다.
3. `CollisionShape` 가 컴포넌트에 들어가게 되면, 자동으로 `AABBRenderer2D` 을 해당 오브젝트에 생성하도록 한다. `opgs16::component::AABBRenderer2D` 는 개별의 Collider 을 바인딩하는 Axis Aligned Bounding Box 렌더러이다.
   * 문제는 한 오브젝트에 렌더러가 여러 개 있을 수가 없다. 
     이 경우에는........... AABB 렌더링할 오브젝트의 리스트에는 오브젝트가 아니라, Collider 컴포넌트를 넘기고, 해당 컴포넌트가 내부적으로 AABBRenderer 을 가지고 있어서 `GetBindObject()` 로 해당 오브젝트의 **물리 엔진에 의해 조정된** 최종 좌표와 콜라이더의 크기 등을 고려해서 그리도록 한다...? 가 더 깔끔할 것 같다.

#### 프로토타입 테스트

1. `CProtRigidbodyCollider2D` 와 해당 컴포넌트가 바인딩 된 `CObject` 의 `Update` 는 각각 현재 콜라이더의 위치, AABB 의 경계 위치, 그리고 CObject 에서 가지고 있는 최종 위치를 매 프레임마다 출력한다.	
2. 하나는 `ObjectPlayer` 이고, 또 다른 하나는 `ObjectFloor` 이며 `Player` 는 중력에 따라서 밑으로 낙하하다가 `Static` 인 `Floor` 에 충돌해서 반동 후, 자유이동을 할 수 있게 된다.

## 1차 결과

1. 2018-07-29 12:11

우선 `CProtRigidbodyCollider2D` 을 만들었지만, 아직 내부의 컬라이더에 대해서 `DynamicWorld` 및 `AllocatorArray` 에 등록은 하지 않은 상태이다. 그리고 `ObjectPlayer` 와 `ObjectFloor` 역시 `WorldPosition` 을 $$ (0, 0, 0) $$ 인채로 그대로 둔 상태에서 로그 테스트를 했다.

``` text
[2018-07-29 00:06:36.939] [OPGS16] [info] Floor center is (0, 0, 0) / AABB (-24, -24, 0) ~ (24, 24, 0)
[2018-07-29 00:06:36.954] [OPGS16] [info] Player center is (0, 0, 0) / AABB (-8, -8, 0) ~ (8, 8, 0)
[2018-07-29 00:06:36.955] [OPGS16] [info] Floor center is (0, 0, 0) / AABB (-24, -24, 0) ~ (24, 24, 0)
[2018-07-29 00:06:36.971] [OPGS16] [info] Player center is (0, 0, 0) / AABB (-8, -8, 0) ~ (8, 8, 0)
[2018-07-29 00:06:36.972] [OPGS16] [info] Floor center is (0, 0, 0) / AABB (-24, -24, 0) ~ (24, 24, 0)
[2018-07-29 00:06:36.988] [OPGS16] [info] Player center is (0, 0, 0) / AABB (-8, -8, 0) ~ (8, 8, 0)
[2018-07-29 00:06:36.989] [OPGS16] [info] Floor center is (0, 0, 0) / AABB (-24, -24, 0) ~ (24, 24, 0)
[2018-07-29 00:06:37.004] [OPGS16] [info] Player center is (0, 0, 0) / AABB (-8, -8, 0) ~ (8, 8, 0)
```

#### 해결점은?

우선 `CObject` 는 `CRigidbody` 와 `CCollider` 외에도 `CScriptFrame` 이 여러개 있을 수 있으며 해당 스크립트의 `Update` 와 같은 함수에서 임의로 `CObject` 의 이동을 수행하게 할 수도 있다. 

그리고 `CRigidbody` `CCollider` 의 물리 엔진 처리 및 좌표 보간은 `Object `의 `Update` 가 다 끝이 난 뒤에, Dynamic world 에 바인딩 된 Rigidbody 을 업데이트 해서 좌표를 보정한다. 그런데 이 좌표를 보정한 후에는 해당 좌표를 **CObject 의 중심점과 CCollider 의 중심점 차이를 고려해서 WorldPosition 에 좌표 재갱신**을 수행해야 한다. ~~만약 `CRigidbody` 가 Static 이라면 좌표를 갱신하지 않는다.~~ 

일단 `CProtoRigidbodyCollider2D` 의 `btRigidbody` 을 Dynamic world 에 얹어 보았다.

``` text
[2018-07-29 01:05:16.332] [OPGS16] [info] Player center is (-0.0325685, -0.0760556, 0) / AABB (-8.03276, -8.07625, 0) ~ (7.96763, 7.92414, 0)
[2018-07-29 01:05:16.332] [OPGS16] [info] Player center is (0, 0, 0)
[2018-07-29 01:05:16.333] [OPGS16] [info] Floor center is (0.0325685, -0.0795, 0) / AABB (-23.9675, -24.0796, 0) ~ (24.0326, 23.9206, 0)
[2018-07-29 01:05:16.334] [OPGS16] [info] Floor center is (0, 0, 0)
[2018-07-29 01:05:16.334] [OPGS16] [info] Player center is (-0.0325685, -0.0760556, 0) / AABB (-8.03276, -8.07625, 0) ~ (7.96763, 7.92414, 0)
[2018-07-29 01:05:16.335] [OPGS16] [info] Player center is (0, 0, 0)
[2018-07-29 01:05:16.336] [OPGS16] [info] Floor center is (0.0325685, -0.0795, 0) / AABB (-23.9675, -24.0796, 0) ~ (24.0326, 23.9206, 0)
[2018-07-29 01:05:16.336] [OPGS16] [info] Floor center is (0, 0, 0)
[2018-07-29 01:05:16.343] [OPGS16] [info] Player center is (-0.0325685, -0.0760556, 0) / AABB (-8.03276, -8.07625, 0) ~ (7.96763, 7.92414, 0)
[2018-07-29 01:05:16.344] [OPGS16] [info] Player center is (0, 0, 0)
```

`btRigidbody` 내부에서는 좌표가 이동하지만, `CObject` 에서는 이를 반영하지를 않고 있다. 따라서 `Update` 시에 회전과 좌표를 `CObject` 에 반영시켜야 한다. 

* 여기서 또 분기가 발생하는데, 지형에 따라 회전을 하지 말아야할 캐릭터 메쉬, 회전을 반영할 공 메쉬 등이 따로따로 나뉘어서 갱신을 해야할 것 같다.

우선은 **위치 이동**만 반영해본다. 그런데 

``` c++
if (rigidbody_obj && rigidbody_obj->getMotionState()) {
  rigidbody_obj->getMotionState()->getWorldTransform(trans);

  auto obj_ptr = static_cast<element::CObject*>(rigidbody_obj->getUserPointer());
  obj_ptr->SetWorldPosition(trans.getOrigin());
}
```

여기서 `trans.getOrigin()` 은 실제 위치 즉 Final Position 이다. 따라서 FInal Position 을 넣어서 World Position 을 갱신하게 하는 함수를 `opgs16::element::CObject` 에 새로 만들어야 했다.

``` c++
///
/// @brief The method set final position but keeping parent position
/// but adjusting CObject's world position.
///
void SetWorldPosWithFinalPos(const DVector3& final_position);
```

* 문제는 각 `CObject` 의 전파와 위치 갱신이 `GetModelMatrix()` 을 호출함에 따라서 일어나기 때문에 `GetModelMatrix()` 을 호출하지 않으면 위치가 잘못될 수도 있다는 잠재적인 문제점이 있다. 따라서 이는 `fix_position_propagation` 브랜치를 만들어서 별도의 플래그에 따라 자동으로 위치를 갱신할 수 있도록 해야한다.

우선 적용된 물리엔진에 따라 물체가 충돌하고 이동하는 것은 `2018-07-29-02-17-41-log.txt` 에서 확인할 수 있다.

#### 2018-07-29 PM 17:28

이동하는 것을 확인. 그런데 문제는, 반동 (`restitution`) 이 있을 경우에 $$ z $$ 축 오차 때문에? 아마 $$ z $$ 축 오차 때문에 앞이나 뒤로 흘러 들어가버리는 상황이 발생.

및 실제 Collider 을 만들 때에는 `Static` `Kinetic` `Dynamic` 여부를 전체를 관리하는 유사 `CRigidbody` 에서 관리하게 끔 해야할 것 같다. 왜 콜라이더 개개별이 아니라 `CRIgidbody` 에서 관리하게 하냐면, 오브젝트가 움직이면서 일정 컬라이더만 `Static` 이지는 않기 때문임.