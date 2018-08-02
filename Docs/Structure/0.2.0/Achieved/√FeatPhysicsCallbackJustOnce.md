# 한 메인 프레임에 한번의 콜백만 호출할 수 있도록 하기

> 2018-08-01 문서 작성

## 문제

`CCollider` 가 다른 `CCollider` 에 충돌하는 것을 보면, 한 프레임에 여러 번 콜백이 호출되는 경우가 있다. 이를 방지하자.

## 제안 & 구현 사항

`feat_physics_cb_just_once` 라는 브랜치를 만든다.

`m_is_collided_flag_setup` 이라는 플래그는 오직 물리 환경에 의해서만 호출이 되어 `true` 가 되며, 다음 프레임의 `Update()` 에서 다시 `false` 가 되기 때문에 이를 사용해서 쓴다.

문제는 아직 개개별의 컬라이더에 대해서의 플래그가 서있지 않기 때문에 지금은 단일 플래그만으로 쓴다.

## 결과

``` powershell
PS D:\Development\OPGS16> git diff --stat
 .../Include/Component/Physics/prot_rigidbody_collider2d.h  |  2 ++
 .../Source/Component/Physics/prot_rigidbody_collider2d.cc  |  6 +++++-
 OPGS16/Source/Manager/Internal/physics_environment.cc      | 14 ++++++++++----
 3 files changed, 17 insertions(+), 5 deletions(-)
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "feat: (6) Reface to be called callback function just once per frame."
[feat_physics_cb_just_once cd6d07c] feat: (6) Reface to be called callback function just once per frame.
 3 files changed, 17 insertions(+), 5 deletions(-)
PS D:\Development\OPGS16> git checkout -
Switched to branch 'feat_bullet3'
PS D:\Development\OPGS16> git merge -
Updating b5f0de7..cd6d07c
Fast-forward
 .../Include/Component/Physics/prot_rigidbody_collider2d.h  |  2 ++
 .../Source/Component/Physics/prot_rigidbody_collider2d.cc  |  6 +++++-
 OPGS16/Source/Manager/Internal/physics_environment.cc      | 14 ++++++++++----
 3 files changed, 17 insertions(+), 5 deletions(-)
```

