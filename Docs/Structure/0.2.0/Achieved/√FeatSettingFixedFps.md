# 고정형 FPS 세팅하기

> 2018-07-23 초안 작성

`OPGS16` 의 `0.1.0` 버전에서는 고정형 FPS 가 항상 60으로 설정되었다. 굳이 이미 있는 고정 FPS 을 따로 떼어내서 `core_setting` 에 설정을 해줘야 하나 생각을 할 수도 있지만, 유저가 `30` 으로 세팅할 수도 있고 `24` 로 세팅할 수도 있기 때문에 이 역시 `manifest` 에서 매크로를 사용해서 설정을 해줘야 하지 않을까 하는 생각이 든다.

## 제안

* `manifest` 에서 전처리 매크로를 사용해 컴파일 타임에 `core_setting` 에 Fps 을 설정하게 한다. 그리고 `vsync` 가 켜졌을 때 `Fps` 가 필요한 부분에서는 `Fps` 을 `core_setting` 에서 가져와서 쓰도록 한다.
* `application.cc` 의 `constexpr float k_fps_count = 60.f;` 을 `core_setting` 으로 옮긴다.

## 구현 사항

1. `manifest.h` 에서 기본 고정형 FPS 을 설정하게끔 할지, 아니면 다른 고정형 FPS 을 설정하게 할지를 결정한다?

2. 아니면 거의 모든 어플리케이션이 `60` 혹은 `30` 만을 쓴다는 점에서, 다음과 같이 `60` FPS 을 쓸지 `30` FPS 을 쓸지만을 정하게 하는 것도 좋을 것 같기도 하다.

   ``` c++
   #define OP16_SETTING_USING_60FPS
   //#define OP16_SETTING_USING_30FPS
   ```

   그러고 나면, `core_setting.h` 에서는 매크로 정의에 따라 `60` 과 `30` 을 나눈다.

## 결과

`manifest.h` 파일에는 다음과 같이 옵션 매크로를 추가했다.

``` c++
///
/// HARD OPTION : Fixed function fixed fps controller
///
/// You must turn on only one FPS value option
/// even though you are using no-vsync option on application.
///
/// OP16_SETTING_USING_60FPS_ON_VSYNC
/// OP16_SETTING_USING_30FPS_ON_VSYNC
///

#define OP16_SETTING_USING_60FPS_ON_VSYNC
//#define OP16_SETTING_USING_30FPS_ON_VSYNC
```

그리고 `core_setting.cc` 에서는 `Fps` 의 설정과, 이를 리턴하는 메소드를 추가했다.

``` c++
// Vsync fixed fps
#if defined(OP16_SETTING_USING_60FPS_ON_VSYNC)
  #if defined(OP16_SETTING_USING_30FPS_ON_VSYNC)
    static_assert(false, "Just only one vsync fixed fps setting must be turned on.");
  #else
    constexpr int32_t s_vsync_fixed_fps = 60;
  #endif
#elif defined(OP16_SETTING_USING_30FPS_ON_VSYNC)
  #if defined(OP16_SETTING_USING_60FPS_ON_VSYNC)
    static_assert(false, "Just only one vsync fixed fps setting must be turned on.");
  #else
    constexpr int32_t s_vsync_fixed_fps = 30;
  #endif
#else
  static_assert(false, "At least one vsync fixed fps option must be turned on.");
#endif
```

