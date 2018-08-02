# AABB 에 색칠하기

> 2018-07-31 문서 작성

## 문제

현재 브랜치 `4c0bd69` 에서는 AABB 의 색상이 노랑색 한 단계로 통일되어 있다. 따라서 해당 AABB 가 무슨 상태인가를 알아보기 어렵다.

## 제안

AABB 의 상태에 따라 각기 다른 색상을 부여한다. 컬라이더, 리기드바디 의 유무, 활성화 유무에 따라서 `Static` `Kinetic` `Dynamic` 등으로 컬라이더의 종류를 나눌 수 있으며, 충돌 판정 등에 대해서도 나눌 수 있다.

#### AABB 표시 색

- `Collider` Actv , `Rigidbody` Actv / Kinetic :: 바다색 (Aqua)
- `Collider` Actv , `Rigidbody` Actv / Dynamic :: 연두색 (Green)
- `Collider` Actv , `Rigidbody` Deact or NotExist :: 주황색 (Orange)
- `Collider` 충돌 시 :: 노랑색 (Yellow)
- `Collider` 연산 제외 및 휴식 (Sleep) 시 :: 연회색 (Gray)

## 구현 사항

#### AABB 표시 색

- `Collider` Actv , `Rigidbody` Actv / Kinetic :: 바다색 (Aqua)
- `Collider` Actv , `Rigidbody` Actv / Dynamic :: 연두색 (Green)
- `Collider` Actv , `Rigidbody` Deact or NotExist :: 주황색 (Orange)
- `Collider` 충돌 시 :: 노랑색 (Yellow)
- `Collider` 연산 제외 및 휴식 (Sleep) 시 :: 연회색 (Gray)

을 구현한다.

#### CProtoRigidbodyCollider2D 에서는?

`CProtoRigidbodyCollider2D` 컴포넌트는 Rigidbody 와 고정된 `btBox2DShape` 가 같이 붙어다니는 물리 컬라이더 컴포넌트이다. 이 경우 ...

#### 절차

1. `feat_aabb_color` 브랜치를 만든다. 아 몰라... ㅅㅂ

## 결과

``` c++
// Element/Internal/physics_enums.h
namespace opgs16::element::_internal {

enum class EColliderStateColor {
  /// Activated (Aqua, Green or Orange)
  Activated,
  /// Collided with any collider. (Yellow)
  Collided,
  /// Is sleep (Light-Gray)
  Sleep,

  /// Not initiated
  None
};

enum class EColliderActualType {
  /// Both CCollider and CRigidbody activated but is kinetic. (Aqua)
  Kinetic,
  /// Both CCollider and CRigidbody activated and not kinetic, has mass. (Green)
  Dynamic,
  /// CCollider activated but CRigidbody (not btRigidbody!) deactivated. (Orange)
  Staic,

  /// None. Not initiated.
  None
};

} /// ::opgs16::element::_internal namespace
```

에서 `EColliderStateColor` 을 `EColliderBehaviorState` 로 바꿨다.

#### 2018-07-31 PM 09:42

우선 `CCollider` 와 `CPrivateAabbRenderer` 는 서로 연결되어 있다. 그리고 `CPriavateAabbRenderer` 에서 색상을 정하는 것은 `CCollider` 의 함수인 `GetColliderState()` 을 통해서 가져온 속성에 따라서 색상을 골라 넣게 된다. 

``` c++
void CPrivateAabbRendererBase::pSetAabbRenderingColor() {
  using opgs16::element::_internal::EColliderBehaviorState;
  if (!m_parent) {
    PHITOS_UNEXPECTED_BRANCH();
  }

  const auto state = m_parent->GetColliderState();
  PHITOS_ASSERT(state != EColliderBehaviorState::None, 
                "Collision state must not be None.");
  switch (state) {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case EColliderBehaviorState::Activated: {
    const auto type = m_parent->GetColliderActualType();
    switch (type) {
    default: PHITOS_UNEXPECTED_BRANCH(); break;
    case element::_internal::EColliderActualType::Kinetic:
      m_state_color = opgs16::DColor::Aqua;
      break;
    case element::_internal::EColliderActualType::Dynamic:
      m_state_color = opgs16::DColor::Green;
      break;
    case element::_internal::EColliderActualType::Staic:
      m_state_color = opgs16::DColor::Orange;
      break;
    }
  } break;
  case EColliderBehaviorState::Collided:
    m_state_color = opgs16::DColor::Yellow;
    break;
  case EColliderBehaviorState::Sleep:
    m_state_color = opgs16::DColor::Gray;
    break;
  }
}
```

만약에 `Activated` 라고 하면, `Dynamic` 이냐 `Static` `Kinematic` 이냐에 따라서도 색상을 달리 해야하기 때문에 `m_parent :: CCOllider` 에서 해당 콜라이더의 유형 정보를 가져와서 파란색, 초록색, 오렌지색을 가져올 수 있도록 한다.

그리고 쉐이더는 다음과 같이 수정해서, `uColor` 로 3색 컬러를 가져올 수 있도록 했다. 이름은 `opQuad2dLineLoop.cc` 이다.

``` glsl
#version 430 core

uniform vec3 uColor;
uniform float opAlpha;

out vec4 outputColor;

void main() { outputColor = vec4(uColor, opAlpha); }
```

그리고 `Render()` 함수에서 해당 컬러를 유니폼에 집어넣은 후, 렌더링을 한다.

``` c++
  pSetAabbRenderingColor();
  m_wrapper.SetUniformVec3("uColor", m_state_color);
```

#### result

``` powershell
no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git diff --stat
 .../Component/Physics/prot_rigidbody_collider2d.h  | 10 +++----
 OPGS16/Include/Element/Internal/physics_enums.h    |  2 +-
 .../Source/Component/Internal/aabb_renderer_2d.cc  |  9 +++---
 .../Component/Internal/aabb_renderer_base.cc       | 35 ++++++++--------------
 .../Component/Physics/prot_rigidbody_collider2d.cc | 26 +++++++---------
 .../Source/Manager/Internal/physics_environment.cc | 15 +++++-----
 OPGS16/manifest.h                                  |  2 +-
 7 files changed, 40 insertions(+), 59 deletions(-)
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "feat: (5) Refactoring coloring"
[feat_aabb_color 12078ab] feat: (5) Refactoring coloring
 7 files changed, 40 insertions(+), 59 deletions(-)
```

