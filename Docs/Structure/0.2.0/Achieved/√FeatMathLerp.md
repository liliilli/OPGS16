# Lerp 함수 구현

> 2018-07-28 문서 작성

## 문제

게임 엔진의 가장 기초의 기초 함수인 `Lerp` 구현이 안 되어 있다. `Lerp` 는 `Linear Interpolation` 의 약자로 $$ 0 $$ 과 $$ 1 $$ 사이의 오프셋 값에 따라 타입이 동일하며 곱하기 혹은 나눗셈 및 더하기 및 빼기 연산이 가능한 `T` 타입 `lhs` `rhs` 에 대해 중간값을 반환한다.

## 제안

`Lerp` 함수를 구현한다. 구현할 타입은 다음과 같다.

* `DVector2`
* `DVector3`
* `float`
* `double`

`int32_t` `uint32_t` 은 구현을 아직 하지 않는다. 왜냐면 결과값으로 실수값이 나올 수 있는데 해당 타입들은 실수의 소수점을 버리기 때문이다.

`DVector4` 역시 구현을 하지 않는다. 왜냐면 아직 `DVector4` 자체가 구현이 안되있을 뿐더러, 나중에 구현을 한다고 하더라도 `w` 축에 대해서는 어떻게 해야할 지 아직 정하지 않았기 때문이다.

> 만약에 `Lerp` 을 사용시에, `overflow` 가 된다면 어떻게 해야할까?...
> `float` 및 `double` 에 대해 구현하는 `Lerp` 는 자동으로 오버플로우 체크를 한다.

## 구현 사항

1. `feat_math_lerp` 브랜치를 생성한다.
2. 함수를 생성한다. 이 때 `Lerp` 함수는 `constexpr` 을 붙이지 않는다. 왜냐면 별도의 헤더파일 등이 추가로 필요하기 때문이다.
3. `float` 및 `double` 함수의 경우 `<cfenv>` 을 사용해서 오버플로우 체크를 실시한다.

## 결과

``` c++
float Lerp(float lhs, float rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<float>::lowest();
  }
  return result;
}

double Lerp(double lhs, double rhs, float offset) {
  std::feclearexcept(FE_ALL_EXCEPT);
  const auto result = lhs * (1 - offset) + rhs * offset;
  if (std::fetestexcept(FE_INVALID | FE_OVERFLOW)) {
    return std::numeric_limits<double>::lowest();
  }
  return result;
}

DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}

DVector3 Lerp(const DVector3& lhs, const DVector3& rhs, float offset) {
  return lhs * (1 - offset) + rhs * offset;
}
```

``` powershell
PS D:\Development\OPGS16> git merge -
Updating 7d929fe..0d1c455
Fast-forward
 OPGS16/Include/Helper/Math/math.h | 22 ++++++++++++++++++++++
 1 file changed, 22 insertions(+)
PS D:\Development\OPGS16> git checkout -b "feat_math_lerp"
Switched to a new branch 'feat_math_lerp'
```

