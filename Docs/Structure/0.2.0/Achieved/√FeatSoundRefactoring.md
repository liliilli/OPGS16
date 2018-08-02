# SoundManager 리팩터링

> 2018-07-26 문서 작성

## 문제

## 제안

1. `SetMute()` 을 지우고, `IsSoundSystemAvailable()` 로 바꾼다.

본디 `0.1.0` 에서 `SetMute()`는 사운드 시스템 전반 채널을 뮤트하도록 설계가 될 예정이었다. 하지만 끝끝내 구현되지 않았고 `0.2.0` 에서는 이를 대신하고 좀 더 명확한 함수인 `MuteMasterSoundChannel()` 가 있기 때문에 무용지물이 되버렸다.

대신, 사운드 시스템이 현재 사용이 가능한가 아닌가를 `bool` 값으로 반환하는 함수를 구현하도록 한다. 왜냐면 `0.1.0` 에서는 사운드 시스템이 사용 가능한가를 반환하는 함수가 없었고, 만약 사운드 시스템이 지원되지 않는 하드웨어에서는 이를 커버링할 필요가 반드시 있을 수 밖에 없기 때문이다.

2. `IsSoundMuted()` 을 지운다.

이 역시 `0.2.0` 에서 구현된 각 채널의 `IsSoundChannelMuted()` 로 역할이 넘겨졌기 때문이다.

3. 기존 `SoundXyz()` 함수를 `SoundElementXyz()` 함수 명칭으로 바꾼다.
4. 에러 로그들을 `inline static constexpr char*` 로 한 곳에 다 집어넣는다.

## 구현 사항

1. `SetMute()` 을 지우고, `IsSoundSystemAvailable()` 로 바꾼다.

``` c++
///
/// @brief Set mute with true or false.
/// @param[in] value Mute switch.
///
void SetMute(phitos::enums::ESwitch value);
```

을 지우고,

``` c++
bool IsSoundSystemAvailable() noexcept;
```

로 바꾼다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "refc: Sound manager refactoring"
[refc_sound_refactoring 9ea4c1a] refc: Sound manager refactoring
 2 files changed, 186 insertions(+), 149 deletions(-)
 
PS D:\Development\OPGS16> git commit -m "refc: Move comments"
[refc_sound_refactoring 75f2b88] refc: Move comments
 1 file changed, 60 insertions(+), 55 deletions(-)
```

