# 다른 Collider 도 실장하기

> 2018-08-02 문서 작성

## 문제

현재 `OPGS16` `v0.2.0` 에서 구현 예정 중인 컬라이더는 다음과 같다.

  Box2D, (완료).
  Box3D,
  Sphere,
  Convex2D,
  Circle (완료)

여기에 `Circle` 을 추가한다. `Circle` 은 2D Collider 이며, $$ (x, y) $$ 축에 대해서 기준을 가지는 원형 컬라이더이다.

## 제안 & 구현 사항

#### Common

브랜치는 `feat_other_collider` 을 사용한다.

#### Circle Sphere

* 조사 결과, `btConvexHullShape` 을 사용해서, `radius` 만큼의 거리를 가지는 근사하는 원을 이루는 정점들을 넣어서 만들면 될 것 같다. 내 생각에는...
* 

## 결과

#### 2018-08-02 PM 01:10

> 문제점 하나 발견 :: 컬라이더가 회전을 하게 되면, 회전 역시 CObject 에 반영을 시켜야함.
>
> 또한 AABB 외에도 실제 컬라이더를 렌더링하게 했으면 좋겠음.

우선 `CColliderCircle2D` 로 $$ (x, y) $$ 축에 바인딩이 되는 2D 컬라이더를 만듦.

``` c++
PS D:\Development\OPGS16> git diff --stat
 OPGS16/Include/Component/Physics/collider2d_box.h | 3 ---
 1 file changed, 3 deletions(-)
PS D:\Development\OPGS16> git diff --stat --cached
 .../Include/Component/Physics/collider2d_circle.h  |  81 +++++++++
 OPGS16/OPGS16.vcxproj                              |   2 +
 OPGS16/OPGS16.vcxproj.filters                      |   2 +
 .../Source/Component/Physics/collider2d_circle.cc  | 202 +++++++++++++++++++++
 4 files changed, 287 insertions(+)
PS D:\Development\OPGS16> git commmit -m "feat: Add circle collider2d (CColliderCircle2D)"
```

