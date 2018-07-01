# opgs16::random

* [x] `RandomInteger()`

  * `uniform` 하게 `int` (platform 마다 다를 수 있음) 범위에서 Int 을 가져온다.

* [x] `RandomFloat()`

  * `uniform` 하게 `float` 범위에서 float 을 가져온다.
  * `Float` 으로 쓰는 것보다는 `Real` 로 쓰는게 더 유저 접근적이지 않을까? 하지만 이렇게 되면 `Double` 과 구분을 지어야 한다...

* [x] `RandomPositiveInteger()`

  * `uniform` 하게 `std::numeric_limits<int32_t>::max` 에서 `0` 까지의 수 중 랜덤으로 가지고 온다.

* [x] `RandomNegativeInteger()`

  * `uniform` 하게 `0` 에서 `std::numeric_limits<int32_t>::lowest() + 1` 까지의 수 중 랜덤으로 가지고 온다.

* [x] `RandomPositiveFloat()` 

  * `uniform` 하게 `0~` 에서 `std::numeric_limits<float>::max()` 까지의 수 중 랜덤으로 가지고 온다.

* [x] `RandomNegativeFloat()`

  * `uniform` 하게 `std::numeric_limits<float>::lowest()` 에서 `-0` 까지의 수 중 랜덤으로 가지고 온다.

* [x] `RandomVector2Length(int32_t length)`

  * 길이가 `length` 인 `DVector2` 을 생성한다.

* [x] `RandomVector3Length(int32_t length)`

  * 길이가 `length` 인 `DVecotr3` 을 생성한다.

  * 이렇게 해볼까? (Unreal engine 4)

    ``` c++
    inline FVector FMath::VRand()
    {
  FVector Result;
  float L;
  do
  {
    // Check random vectors in the unit sphere 
    //so result is statistically uniform.
    Result.X = FRand() * 2.f - 1.f;
    Result.Y = FRand() * 2.f - 1.f;
    Result.Z = FRand() * 2.f - 1.f;
    L = Result.SizeSquared();
  }
  while(L > 1.0f || L < KINDA_SMALL_NUMBER);
  return Result * (1.0f / Sqrt(L));
    }
    ```

* [ ] `RandomVector2Range(ERandomPolicy, float from, float to)`

* [ ] `RandomVector3Range(ERandomPolicy, float from, float to)`

