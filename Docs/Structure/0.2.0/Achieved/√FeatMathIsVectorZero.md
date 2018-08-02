# DVector 시리즈 $ 0 $ 판별 함수

> 2018-07-31 문서 작성

## 문제

간혹 `opgs16::DVector` 시리즈에서 벡터의 값이 모두 $$ 0 $$ 인가를 판별해야 하는 경우가 생긴다. 그런데 `opgs16::math` 수학 네임 스페이스에서는 해당 기능을 실장하고 있지 않기 때문에 유저가 직접 로직을 구현해야 하는 경우가 생긴다. 

따라서 이것을 `opgs16::math` 에 함수로, 그리고 각 `DVector` 시리즈에 `static` 으로 해당 `opgs16::math` 의 구현된 함수를 불러서 사용할 수 있도록 한다.

## 제안

구현 해야할 타입은 다음과 같다.

1. `DVector2`
2. `DVector3`
3. `DVector4`
4. `DVectorInt2`
5. `DVectorInt3`
6. `DVectorInt4`

여기서 `DVector4` 와 `DVectorInt4` 는 아직은 구현하지 않는다. 하지만 추후에는 구현한다.

## 구현 사항

`feat_math_isallzero` 브랜치를 만들어서 구현한다.

`opgs16::math` 에서의 함수 구현 시그니쳐는 다음과 같다.

``` c++
bool IsAllZero(const opgs16::DVector3& vector) noexcept;
```

만약 `float` 을 요소 타입으로 가지는 경우라면, `IsNearlyEqual` 을 사용해서 $$ 0 $$ 인가를 따진다.

`opgs16::DVector` 에서 함수 구현 시그니쳐는 다음과 같다.

``` c++
static bool IsAllZero(const opgs16::DVector& vector) noexcept;
```

이 함수 바디에서는 `opgs16::math::IsAllZero` 을 호출해서 결과를 반환한다.

## 결과

``` powershell
nothing added to commit but untracked files present (use "git add" to track)
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is ahead of 'origin/0.2.0' by 4 commits.
  (use "git push" to publish your local commits)
PS D:\Development\OPGS16> git merge -
Updating b1b3e2b..e878305
Fast-forward
 OPGS16/Include/Helper/Math/math.h       | 22 ++++++++++++++++++++++
 OPGS16/Include/Helper/Type/vector2.h    | 12 ++++++++++++
 OPGS16/Include/Helper/Type/vector3.h    | 10 ++++++++++
 OPGS16/Include/Helper/Type/vectori2.h   | 10 ++++++++++
 OPGS16/Include/Helper/Type/vectori3.h   | 10 ++++++++++
 OPGS16/Source/Helper/Math/math.cc       | 26 ++++++++++++++++++++++++++
 OPGS16/Source/Helper/Type/vector2.cc    | 33 +++++++++++++++++++++++++++++++++
 OPGS16/Source/Helper/Type/vector3.cc    | 33 +++++++++++++++++++++++++++++++++
 OPGS16/Source/Helper/Type/vectorint2.cc | 33 +++++++++++++++++++++++++++++++++
 OPGS16/Source/Helper/Type/vectorint3.cc | 33 +++++++++++++++++++++++++++++++++
 10 files changed, 222 insertions(+)
 create mode 100644 OPGS16/Source/Helper/Type/vector2.cc
 create mode 100644 OPGS16/Source/Helper/Type/vector3.cc
 create mode 100644 OPGS16/Source/Helper/Type/vectorint2.cc
 create mode 100644 OPGS16/Source/Helper/Type/vectorint3.cc
```

1. `DVector2` `DVector3` `DVectorInt2` `DVectorInt3 ` 에 대해 `opgs16::math::IsAllZero()` 함수를 구현했다.
   * 아직 `DVector4` 와 `DVectorInt4` 에 대해서는 타입이 구현되지 않은 관계로 구현 X
2. `DVector2` `DVector3` `DVectorInt2` `DVectorInt3` 의 내부 구현부에서 `IsAllZero()` `static` 버전과 일반 버전 함수를 구현했다.
   * 아직 `DVector4` 와 `DVectorInt4` 에 대해서는 타입이 구현되지 않은 관계로 구현 X