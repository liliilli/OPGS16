# AABB 가 실제 AABB 좌표를 받도록 하기

> 2018-07-31 문서 작성

## 문제

심각한 문제다. 누가 이따구로 짰는지는 몰라도 졸라 맞아야 한다.

현재 `e878305` 브랜치에서 AABB 의 영역 렌더링은, `AABB` 가 붙은 `CCollider` 컴포넌트가 붙은 `CObject` 게임 오브젝트의 `FinalPosition` 과, 초기에 정해진 콜라이더의 사이즈를 정해서 모델 매트릭스를 만들고 있다. 이는 즉 `AABB` 의 실제 좌표를 통해서 렌더링을 하고 있지 않다는 것이다. 이런 망할

``` c++
// prot_rigidbody_collider2d.cc Update
if (m_aabb_renderer) {
  m_aabb_renderer->SetCollisionRenderPosition(position);
  m_aabb_renderer->Update(delta_time);

  opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
}
```

## 제안

1. 물리 환경의 업데이트는 `Render()` 전에 일어난다.

2. 따라서 물리 환경이 업데이트 되기 전에 현재 프레임의 `AABB` 가 렌더링 될 일은 없다.

3. `physics_environment.cc` 에서 업데이트 할 때, `btRigidbody` 에 포인터로 묶여 있는 구조체에 `CObject` 의 포인터와 해당 `btRigidbody` 에 묶인 `CCollider` 의 포인터를 넘기고, (`btRigidbody` 가 물체에서 사라지면, 구조체 역시 사라지기 때문에 댕글링에서 안전함) 
   업데이트를 해서 좌표 보간을 할 때 `CCollider` 의 포인터에 해당 AABB 의 정보를 갱신해서 `CPrivateAABBRenderer` 가 이를 가지고 현재 Size 와 중심 지점을 알 게 해서 최종 `ModelMatrix` 을 계산할 수 있도록 하자.

   * 따라서 기존에는 `CObject*` 을 포인터로 넘겼지만, 현재는 `CCollider` 에 메모리 공간을 두고 있는,
     `DPrivateColliderBindInfo` 라는 구조체를 넘겨서 위 로직을 수행할 수 있도록 한다.

     ``` c++
     struct DPrivateColliderBindInfo {
         opgs16::element::CObject* obj;
         opgs16::Component::_internal::CCollider* collider;
     };
     ```

     이런 식으로...

## 구현 사항

1. `fix_draw_aabb_test` 브랜치를 생성해서 여기서 구현을 한다.
2. `DPrivateColliderBindInfo` 구조체를 `Element/Internal` 에서 생성한다. 파일명은 `physics_` 로 시작하게끔 한다.
3. 해당 구조체에서 포인터의 타입을 설정할 때, 전방 선언을 사용한다.
4. `Collider` 에서 렌더러의 업데이트 및 바인딩을 하는 것을, 별도의 함수로 옮긴다. 물리 환경에서 해당 컬라이더에 AABB 정보를 넘길 때 해당 함수를 사용한다.
   이 때 별도의 함수는 `private` 이어야 하며, 물리 환경에서 이를 접근하기 위해 `friend` 가 허용될 수 있다.

## 결과

``` c++
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is ahead of 'origin/0.2.0' by 5 commits.
  (use "git push" to publish your local commits)
PS D:\Development\OPGS16> git merge -
Updating e878305..4c0bd69
Fast-forward
 .../Component/Physics/prot_rigidbody_collider2d.h  | 23 +++++++++--
 .../Element/Internal/physics_collider_bind_info.h  | 44 ++++++++++++++++++++++
 OPGS16/Include/Helper/Type/vector3.h               |  6 +--
 OPGS16/OPGS16.vcxproj                              |  5 +++
 OPGS16/OPGS16.vcxproj.filters                      |  5 +++
 .../Component/Internal/aabb_renderer_base.cc       |  6 +--
 .../Component/Physics/prot_rigidbody_collider2d.cc | 25 +++++++-----
 .../Source/Manager/Internal/physics_environment.cc | 27 ++++++++++---
 8 files changed, 117 insertions(+), 24 deletions(-)
 create mode 100644 OPGS16/Include/Element/Internal/physics_collider_bind_info.h
```

``` c++
// void CPhysicsEnvironment::PhysicsUpdate(float delta_time) 
auto obj_ptr = static_cast<DPrivateColliderBindInfo*>(
    rigidbody_obj->getUserPointer()
);

// Update position and aabb information.
obj_ptr->bind_object->SetWorldPosWithFinalPos(
    static_cast<DVector3>(trans.getOrigin())
);

btVector3 min, max;
rigidbody_obj->getAabb(min, max);
obj_ptr->bind_collider->pUpdateAabbToRenderer(min, max);
```

``` c++
void CProtoRigidbodyCollider2D::pUpdateAabbToRenderer(
    const DVector3& min,
    const DVector3& max) {
  if (m_aabb_renderer) {
    m_aabb_renderer->SetCollisionSize(max - min);
    m_aabb_renderer->SetCollisionRenderPosition((max + min) / 2);
    m_aabb_renderer->Update(0);
    opgs16::manager::object::InsertAABBInformation(*m_aabb_renderer);
  }
}
```

``` c++
namespace opgs16::element::_internal {

struct DPrivateColliderBindInfo {
  opgs16::element::CObject* bind_object;
  opgs16::component::CProtoRigidbodyCollider2D* bind_collider;
};

} /// ::opgs16::element::_internal namespace

```

을 구현했다. 

나중에 `Collider` 을 따로 빼내서 구현하게 될 시, `DPrivateColliderBindInfo` 의 `CProtoRigidbodyCollider2D` 부분을 수정해야 한다.