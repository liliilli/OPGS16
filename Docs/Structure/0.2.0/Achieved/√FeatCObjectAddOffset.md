# CObject 이동, 회전에 Offset Add 하기

> 2018-07-31 문서 작성

## 문제

`OPGS16` `v0.1.0` 에서는 물체를 이동시키려고 하면 다음과 같이 로직을 짜야만 했다.

``` c++
// 임의 CScriptFrame.h

auto& obj = GetBindObject();
const auto pos = obj.GetWorldPosition();
obj.SetWorldPosition(pos + {0, 10, 0}); // y 축으로 10 이동...
```

이렇게 하면 포지션에 대한 복사비용이 들 뿐만 아니라 매우 번거롭다.

## 제안

그래서 이번에는 `CObject` 에 전체 좌표나 회전 각도를 가지고 오지 않고도 바로 축과 값을 집어넣어서 좌표를 반영할 수 있도록 하게 하면 안될까? 싶다.

예를 들면 다음과 같이, y 축으로 10을 이동시키고 싶으면

``` c++
// 임의 CScriptFrame.h

obj.AddOffsetWorldPosition(Axis::Y, 10); // y 축으로 10 이동
```

과 같이 구현하고자 한다.

## 구현 사항

1. 이동 좌표축에 대해서는 다음 좌표들에 대해 Offset 함수를 구현해줘야 한다.

* Local Position
* World Position

내부 좌표축에 대해서는 상관할 필요 없음.

2. 회전 축에 대해서는 다음에 대해 Offset 함수를 구현해줘야 한다.

* Local Rotation
* World Rotation

브랜치 명은 `feat_object_add_offset` 으로 한다.

첫번째 인자로 들어갈 축은 `Helper/Type/axis.h` 의 `EAxis3D` 으로 한다.

## 결과

1. `opgs16::math::GetRotationAngle(value)` 라는 -180 도에서 180 도까지의 Degree 함수 각을 반환하는 함수를 만들어 리팩터링을 겸했다.

``` c++
float GetRotationAngle(float angle_value) noexcept {
  const float angle = std::fmod(angle_value, 360.f);
  return (angle > 180.f) ?  angle - 360.f :
             ((angle <= -180.f) ?  angle + 360.f : angle);
}
```

2. `AddOffsetLocalPosition` `AddOffsetLocalAngle` 및 `WorldPosition` `WorldRotation` 의 오프셋 값 증가 함수를 만들었다.

``` c++
void AddOffsetLocalPosition(EAxis3D axis, float value) noexcept;
void AddOffsetWorldPosition(EAxis3D axis, float value) noexcept;
void AddOffsetLocalAngle(EAxis3D axis, float value) noexcept;
void AddOffsetWorldAngle(EAxis3D axis, float value) noexcept;
```

> 여기서 CObject 에서 EAxis3D 을 쓰지 않고 독자 DirectionType 을 쓰는 것을 확인할 수 있었는데, 이 역시 리팩터링이 당장 필요할 것 같다.

