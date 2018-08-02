# `Float` 과 같은 단, 배정도 실수의 대략적인 `Equal` 함수 구현

> 2018-07-28 문서 작성

## 문제

`float` 및 `double` 과 같은 부동소수점 빌트인 타입은 특정 소수점을 완벽하게 구현하지 못한다. 예를 들어서 `0.5` 같은 경우에는 완벽히 구현할 수 있지만, `0.1` 을 저장하면 값을 체크할 때는 `0.10000007` 과 같은 오류가 발생한다. 

이는 `IEEE754` 에서 표준으로 정한 규격에서 일어날 수 밖에 없는 오류이기 때문이다. 문제는 해당 `float` 변수와 다른 `float` 변수에 대해 같은가 아닌가를 확인하기 위해서는 이 부동소수점 오류를 고려를 해야한다. 하지만 지금 OPGS16 `v0.1.0` 에서는 부동소수점을 제한 **대략적인 Equal** 함수를 제공하고 있지 않다.

## 제안

`opgs16::math` 에 `NearlyEqual` 함수를 제공한다. 읽어들일 수 있는 `Type` 으로는 `float` `double` 만 가능하다.

> 생각한건데 `Lerp` 역시 만들어야 할 것 같다...

## 구현 사항

1. `feat_math_nearlyequal` 브랜치를 만든다.
2. `float` 용 `IsNearlyEqual` 과 `double` 용 `IsNearlyEqual` 을 만든다. 왜냐하면 유저가 항상 `float` 을 쓴다는 보장이 없기 때문이다. 

이때 시그니쳐는 다음과 같이 적는다.

``` c++
bool IsNearlyEqual(float lhs, float rhs, float error_tolerance);
```

* `lhs` 는 피대상이다.
* `rhs` 는 대조군이다.
* `error_tolerance` 는 피대상과 대조군을 매칭 시, 같음을 판단하는 오차 범위를 나타낸다. 여기서 `error_tolerance` 는 기본 값을 가진다. `float` 및 `double` 모두 오차에 대한 기본값은 `0.0001` 이다.

## 결과

``` c++
///
/// @brief Check float lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two floating points.
///
constexpr bool IsNearlyEqual(const float lhs, const float rhs,
                             const float error_tolerance = 0.0001f) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) <= error_tolerance;
}

///
/// @brief Check double lhs is nearly equal to rhs.
/// floating-point number type like a float and double has a natural problem like
/// a floating-error such as 0.1 but 0.10000007, so you have to use this function
/// if you want to compare two double points.
///
constexpr bool IsNearlyEqual(const double lhs, const double rhs,
                             const double error_tolerance = 0.0001) noexcept {
  return (rhs < lhs ? lhs - rhs : rhs - lhs) <= error_tolerance;
}
```

``` powershell
PS D:\Development\OPGS16> git merge -
Updating 7d929fe..0d1c455
Fast-forward
 OPGS16/Include/Helper/Math/math.h | 22 ++++++++++++++++++++++
 1 file changed, 22 insertions(+)
```

