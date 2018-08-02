# 고정 함수 가변 해상도

> 2018-07-23 파일 생성 및 초안 작성
> 2018-07-23 초안 및 구현 확정
> 2018-07-24 누락된 `400x240` 해상도 추가

가변 해상도를 여러 개 제공해서 쓸 수 있도록 한다. 가변 해상도의 종류는 다음과 같이 쓸 수 있다.

1. `256 x 224` : OPGS16 0.1.0 에서 쓰였던 고정 해상도이다. 
2. `320 x 240` : OPGS16 0.2.0 에서 쓰일 고정 해상도이다.
   이 해상도는 `Windows` 데스크탑 버전에서 스케일 조정이 가능하다. 
3. `640 x 480` : OPGS16 0.2.0 에서 쓰일 고정 고해상도이다.
   이 해상도는 `320 x 240` 과는 다르게 스케일 조정이 불가능하다.

> 왜 고정 해상도를 만들어야 하는데?
>
> `OPGS16` 은 16비트에서 32비트 초창기의 콘솔을 재현하고자 한다. 당시의 해상도들은 `SNES` 을 제외하고 모두 `320 x 240` 이거나 `640 x 480` 였다.
> 사실 스케일링 후처리 쉐이더를 버리기에는 너무 아까워서 그렇다. 또한 TV 모니터를 출력하는 쉐이더 역시 안 쓰기에는 너무 아깝다.

![resolution_comparaison](D:\Development\OPGS16\Docs\Media\resolution_comparaison.png)

## 구현 사항

* Manifest 파일에서 전처리 매크로를 사용해, 실행 시에 해상도를 결정할 수 있도록 한다. 즉, 해상도 3개 분의 매크로를 준비해서, 하나가 켜질 때에만 그 해당되는 하나의 해상도 옵션에 따라 각기 다른 창이 열릴 수 있도록 한다.

``` c++
// manifest.h

//#define OP16_SETTING_RESOLUTION_256224
#define OP16_SETTING_RESOLUTION_320240
//#define OP16_SETTING_RESOLUTION_640480

// application.cc

#if defined(OP16_SETTING_RESOLUTION_256224)
  #if defined(OP16_SETTING_RESOLUTION_320240)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #elif defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  opgs16::manager::setting::SetResolution(256, 224);
#elif defined(OP16_SETTING_RESOLUTION_320240)
  #if defined(OP16_SETTING_RESOLUTION_256224)
    // ...
  #elif defined(640480)
    // ...
  opgs16::manager::setting::SetResolution(320, 240);
#elif defined(640480)
  // ...
  opgs16::manager::setting::SetResolution(640, 480);
  opgs16::manager::setting::SetFeatureScaling(false);
#else
  static_assert(false, "At least one resolution setting option must be turned on.");
#endif
```

* `640 x 480` 해상도는 스케일링 기능을 꺼야한다. 이 경우, PreprocessingManager 의 코드 수정이 불가피할 것이다.

## 결과

* `Core/core_setting.h` 에서는 전처리기에 따라서 자동으로 해상도 변경을 하기로 했다.

``` c++
// core_setting.h

#if defined(OP16_SETTING_RESOLUTION_256224)
  #if defined(OP16_SETTING_RESOLUTION_320240) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr static size_type screen_size{ 256, 224 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_320240)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_640480)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr static size_type screen_size{ 320, 240 };
  #endif
#elif defined(OP16_SETTING_RESOLUTION_640480)
  #if defined(OP16_SETTING_RESOLUTION_256224) ||\
      defined(OP16_SETTING_RESOLUTION_320240)
    static_assert(false, "Just only one resolution setting option must be turned on.");
  #else
    constexpr static size_type screen_size{ 640, 480 };
  #endif
#else
  static_assert(false, "At least one resolution setting option must be turned on.")
#endif
```

* 그리고, `640 x 480` 해상도의 경우 기존 Scaling 을 하지 않도록 설정을 해야했다.
* 세번째, 일반 프레임 버퍼의 기본으로 생성되는 컬러 버퍼 역시 해상도에 맞춰서 버퍼를 뽑아낼 수 있도록 해야만 했다. 문제는 `core_setting.h` 의 설정 클래스가 Application 쪽에 바인딩이 되어있기 때문에 이 종속성을 없애면서도 `core_setting.h` 을 무명 네임스페이스로 변환을 해야만 했다.
  * 따라서, 파생 브랜치인 `refc_core_setting` 을 만들어야 한다.
  * `√FixCoreSetting.md` 으로 `core_setting` 을 리팩터링함.
* 마지막으로, `640 x 480` 해상도로 렌더링할 경우에는 `Scaling` 기능을 끌 수 있도록 해야한다.

``` c++
// core_setting.cc

bool m_scaling_feature =
#if defined(OP16_SETTING_RESOLUTION_640480)
  false;
#else
  true;
#endif
```

``` c++
// application.cc

#if !defined(OP16_SETTING_RESOLUTION_640480)
  if constexpr (manifest::k_size == 1)
    ChangeScalingOption(EScaleType::X1);
  else if constexpr (manifest::k_size == 2)
    ChangeScalingOption(EScaleType::X2);
  else
    ChangeScalingOption(EScaleType::X3);
#endif
```

* 그리고 `texture` 에도 예전 고정 사이즈로 (`256 x 224`) 해상도를 받는 부분이 있어서 이것도 고쳤다.

``` c++
CTexture2D::CTexture2D(const GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height) {
  PHITOS_ASSERT(width > 0,  "Width value must be bigger than 0.");
  PHITOS_ASSERT(height > 0, "Height value must be bigger than 0.");
  m_width = width; m_height = height;

  // Make Texture for entire screen size.
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0,
               internal_format, width, height,
               0, format, type, nullptr);
  SetDefaultParameterSetting(*this);
}
```

![0723_resolution](D:\Development\OPGS16\Docs\Media\0723_resolution.png)

문제가 해결된 것을 확인할 수 있다.

## 추기

`400x240` 해상도를 빼먹었다. 이것도 추가했다.

