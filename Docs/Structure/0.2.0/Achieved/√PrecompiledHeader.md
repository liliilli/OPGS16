# 미리 정의된 헤더파일 만들기

`OPGS16` 이 0.2.0 으로 넘어오면서 코어 소스만 해도 27,000 줄을 넘기 시작했다. 그렇기 때문에 아무리 모듈화를 잘 하였다고 하더라도 소스코드가 한번 바뀌면 프로젝트를 컴파일 하는데만 해도 대략 3~4분이 걸린다. 특히나 나중에 어떤 대용량 파일이 인클루드 되어 파싱이 될 지 모르는 상황이기 때문이다.

`Precompiled Header` 는 소스 수정으로 인해서 매 번마다 소스를 계속 파싱해야 하는 것을 막는다. 비주얼 스튜디오의 경우 타겟마다 `.pch` 파일을 생성해서 이 안에 현재 컴파일된 여러 심볼들에 대한 컴파일 된 정보를 가지게 한다.

이 이후에 다시 소스 코드를 수정해서 컴파일을 하고자 하면, 타임 스탬프를 비교해 각 단위에서 변경이 되지 않은 소스 파일들에 대해서는 새로 소스 혹은 헤더 파일 코드를 컴파일 하는 대신에 `.pch` 파일을 참고하게 해서 불필요한 컴파일을 줄인다.

## 제안

* `VS2017` 에서 작업을 할 때 `precompiled header` 을 적용시킨다.
* `OPGS16` 의 내부 코드는 수정의 여지가 있기 때문에 섣불리 적용을 시키지 않는다. 대신에 외부 라이브러리 코드는 헤더에 포함시킨다.

## 구현 사항

1. `Include` 폴더와 `Source` 폴더에 각각 `precompiled.h` 와 `precompiled.cc` 라는 미리 컴파일된 헤더 파일을 생성한다.
2. `precompiled.cc` 는 속성에서 `\Yc` 을 체크해 `.pch` 가 생성이 될 수 있도록 한다.
3. 모든 `.cpp` 파일에 `#include <precompiled.h>` 을 적용해서 `.pch` 을 생성할 수 있도록 한다.

위치는 바로 위에 적용한다.

## 결과

* `library_impl.cc` 는 어떻게 할까?

일단은 그대로 둔다.

* `Windows.h` 와의 매크로 재정의로 `C4005 수준 1` 에러가 난다. 어떻게 할까?

`Windows.h` 을 쓰지 않는건 불가피했기 때문에 (콘솔창을 닫는데 사용) 우선은 `precompiled.h` 의 맨 위에 다음과 같이 윈도우 헤더 파일을 붙였다.

``` c++
#if defined(_WIN32)
#include <Windows.h>
#endif

// ... and below is other library files.
```

그러고나니 C4005 에러가 발생하지 않는 것을 확인할 수 있었다.

* 스크립트 파일에도 precompiled header 을 써야하는데, 괜찮을까?

**개인적인 생각이지만, 유저가 직접 클래스 선언과 정의파일을 만들지 않고 템플릿으로 딱 만들 수 있게 하면 아무런 문제가 없을 것 같다. '우리는 템플릿 만들었으니까 쓰라는 곳 안에다가 쓰시고, 딴 거 건들여서 빌드가 안되도 우린 아무 책임이 없습니다' 라고 하면 되니까.**

**근데 지금은 그렇지 않으니까 문제긴 하다. 그냥 문서로 `.cc` 파일에 대해서 맨 위에 `<precompiled.h>` 을 인클루드 하도록 하세요라고 하면 될 것 같다.**

아무튼 적용 결과 컴파일 타임이 3 분에서 1분 30초 정도로 줄어드는 것을 확인할 수 있다.

![img23_preprecomp-down](D:\Development\OPGS16\Docs\Media\img23_preprecomp-down.PNG)

## 참조

> [C++]빌드 타임을 줄여보자 - Precompiled Header  출처: <http://alzpfm.tistory.com/7> [아 아아 아아아] 
>
> /Yc(미리 컴파일된 헤더 파일 만들기) 
> [https://docs.microsoft.com/ko-kr/cpp/build/reference/yc-create-precompiled-header-file](https://docs.microsoft.com/ko-kr/cpp/build/reference/yc-create-precompiled-header-file)

## /MP 을 사용해서 빌드속도를 증가 시키기

![1532386335513](D:\Development\OPGS16\Docs\Media\1532386335513.png)

마이크로소프트 C++ 에서 제공하는 기능인 `/MP` 는 다중 프로세서를 활용해서 빌드를 처리한다. 문제는 이 기능이 기본 설정에서는 꺼져 있어서, 이를 켜줘야만 했다.

> 프로젝트 설정 > C/C++ > General > Multi-Processor Compilation 에서 Yes 을 설정한다.

그러면 1분 39초에서 더 줄어들어서 54초밖에 걸리지 않음을 확인했다.