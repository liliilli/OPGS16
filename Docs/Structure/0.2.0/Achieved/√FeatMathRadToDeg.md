# RadToDeg, DegToRad

> 2018-07-31 문서 작성

## 문제 & 제안

`opgs16::math` 에서 `Radian` 을 `Degree` 단위로 바꾸는 함수와, 역으로 `Degree` 를 `Radian` 으로 바꾸는 함수를 만든다.

그리고 기왕이니 `PI` 도 넣는다. 다만 `PI` 는 `double` 을 쓸 수도 있기 때문에 `Template<>` 변수 템플릿으로 작성한다.

## 구현 사항

1. 변수 템플릿 `PI` 을 구현한다.
2. `RadToDeg` `DegToRad` 을 구현한다. `float` 과 `double` 둘 다 구현한다.

## 결과

``` c++
PS D:\Development\OPGS16> git status
On branch feat_math_redtodeg
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

        modified:   OPGS16/Include/Helper/Math/math.h
        modified:   OPGS16/Source/Helper/Math/math.cc

PS D:\Development\OPGS16> git diff --stat --cached
 OPGS16/Include/Helper/Math/math.h | 55 ++++++++++++++++++++++++++++++++++++++-
 OPGS16/Source/Helper/Math/math.cc | 49 ++++++++++++++++++++++++++++++++--
 2 files changed, 101 insertions(+), 3 deletions(-)
```

1. 변수 템플릿 `pi` 와 `e` 을 실장함

``` c++
template <typename TType>
constexpr TType pi = TType(3.1415926535897932385L);

template <typename TType>
constexpr TType e = TType(2.71828182845904523536L);
```

2. `RadToDeg` `DegToRad` 함수를 구현함.

``` c++
float RadToDeg(float radian) noexcept;
double RadToDeg(double radian) noexcept;
float DegToRad(float degree) noexcept;
double DegToRad(double degree) noexcept;
```

3. `GetRotationAngle` 에 대해서 `Radian` 버전과 `double` 오버로딩을 구현

``` c++
float GetRotationAngleRadian(float angle_value) noexcept;
double GetRotationAngleRadian(double angle_value) noexcept;
```

