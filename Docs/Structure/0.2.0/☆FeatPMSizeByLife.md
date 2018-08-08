# Size By Life 파티클 모듈

> 2018-08-07 문서 작성

## 문제



## 제안 & 구현 사항

![1533631799393](D:\Development\OPGS16\Docs\Media\1533631799393.png)

#### 2018-08-09 AM 01:39

우선 `Linear` 와 `Curve` 종류를 둬서 업데이트 도중에 파티클이 스케일 값을 불러오게 끔 하면, `Linear` 인가 `Curve` 인가에 따라 최종 보간된 값을 가져오게 한다.

`Linear` 라면, 찍혀진 컨트롤 포인트 사이 사이에는 선형 보간으로 값을 계산한다. 만약 `Curve` 라면 `Catmull-rom spline` 이라는, 컨트롤 포인트를 지나는 곡선 알고리즘을 사용해서 값을 계산해서 가져온다.

#### 2018-08-09 AM 01:50

생명 주기는 항상 $$ [0, 1] $$ 사이로 제한한다. 컨트롤 포인트를 넣을 때도 런타임 어설트로 값을 넘는지 안 넘는지를 확인한다. 해당 모듈에서는 스케일 크기는 제한하지 않는다. 의도적으로 $$ < 0 $$ 의 값을 넣을 수도 있다.

* 산출된 값이 $$ 1 $$ 일 때는 `InitialSize` 의 크기가 된다.

* `Cat-mull curve` 의 연산을 위해서는 양측에 2 개의 더미 포인트가 존재해야 한다.

* 만약 컨트롤 포인트가 1 개이거나 0 개 이라면, 처음에 워닝 레벨의 로그를 띄우고 디폴트 포인트를 사용한다.
  디폴트 포인트는 다음과 같다.

  ``` json
  0 : [0, 1.0f]
  1 : [0, 1.0f]
  ```

#### 2018-08-09 AM 01:57

 그러면 생명주기 타임 포인트와, 스케일 값을 저장하기 위한 컨테이너 구조 및 구조체 설계, 그리고 알고리즘 구상이 필요하다.

* 0 과 1 은 꼭 넣어야 한다. 그렇지 않으면 기본 값을 사용한다.
  이를 0 과 1 이 아닌, `Start` 와 `End` 의 별도 함수를 만들어서 제공할까? 음...
  그렇게 되면 위의 디폴트 포인트는 별 필요가 없어진다. 어차피 `Start` 와 `End` 가 있으니까.
  * 그리고 더미포인트는 0 과 1 의 값을 그대로 따른다. 그래야 곡선이 이상하게 흘러가지 않을 것이다.

## 결과

#### 2018-08-07 PM 05:53

일단 보류. `InitialSize` 부터 구현을 하자.

#### 2018-08-09 AM 01:39

``` c++
///
/// @enum EParticleProcessType
/// @brief Used when following-lifecycle particle module is set.
///
enum class EParticleProcessType {
  /// Get value just linearly.
  Linear,
  /// Get value using catmull-rom spline.
  /// https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
  Curve,
};
```

우선 `Linear` 와 `Curve` 종류를 둬서 업데이트 도중에 파티클이 스케일 값을 불러오게 끔 하면, `Linear` 인가 `Curve` 인가에 따라 최종 보간된 값을 가져오게 한다.

`Linear` 라면, 찍혀진 컨트롤 포인트 사이 사이에는 선형 보간으로 값을 계산한다. 만약 `Curve` 라면 `Catmull-rom spline` 이라는, 컨트롤 포인트를 지나는 곡선 알고리즘을 사용해서 값을 계산해서 가져온다.

