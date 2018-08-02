# Manifest 매크로 네이밍 통일시키기

> 2018-07-24 제안

## 문제

`OPGS16` `0.1.0` 버전에서는 매크로 네이밍이 통일되지 않을 뿐만 아니라 무슨 의도로 이름을 지었는지 알 수 없을 매크로도 있다. 따라서 매크로 네미잉을 통일시켜서 유저가 어플리케이션 설정을 할 때 해메는 일이 없도록 한다.

## 제안

현재 `0.2.0` 에서 구현된 매크로 네이밍은 다음과 같다.

``` c++
매크로       세팅
#define OP16_SETTING_XYZ
        OP16의       세부용도 및 이름
```

이 네이밍을 따르지 않는 매크로는 이 네이밍을 따른다.

예를 들어서, 

``` c++
#define _SHOW_BOOT_SCREEN
```

가 있다고 하면, 다음으로 고친다.

``` c++
#define OP16_SETTING_BOOTSCREEN_SHOW
```

## 구현 사항

위 제안대로 `manifest.h` 파일 및 종속된 곳을 고친다.

## 결과

추후에 변경할 `_DEBUG` 을 제외하고 전부 고쳤다.

``` powershell
PS D:\Development\OPGS16> git commit -m "refc: Rename manifest macores"
[refc_manifest_macroes 1bd3b70] refc: Rename manifest macores
 5 files changed, 102 insertions(+), 65 deletions(-)
```

