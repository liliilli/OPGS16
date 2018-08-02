# SGlobalSetting 리팩터링

## 문제 제기

`0.1.0` 부터 존재해왔던 `SGlobalSetting` 은 어플리케이션의 주요한 세팅을 미리 담고 있는 설정 관리자이다. 하지만 `SGlobalSetting` 은 싱글턴이 아니며, `Application` 에 따로 바인딩이 되어 있기 때문에 어떤 설정이 켜져있는가 꺼져있는가를 확인할려면 `Application` 헤더파일에서 세팅의 참조를 가져와, 세팅에서 다시 접근해야 하는 번거로운 단점이 존재한다.

그렇기 때문에 이전 `Manager` 을 일반 싱글턴에서 네임스페이스화 시킨 것 처럼 `SGlobalSetting` 역시 이 방법으로 리팩터링을 실시한다.

##구현 사항

`SGlobalSetting` 은 설정 관리자 클래스이며, 싱글턴이 아니다. 싱글턴이 아니기 때문에 유저가 임의로 마음만 먹으면 여러 개의 세팅을 만들 수 있도록 할 수 있다. 하지만 이는 의도한 바가 절대 아니고 따라서 전역 세팅은 하나만 존재해야 한다.

1. `SGlobalSetting` 의 재구현체는 하나만 존재할 수 있도록 해야한다.

그리고 값들은 Heap 할당이 될 수 있는 객체가 아니어야 한다. 고정이 된다면 컴파일 타임에 값이 결정될 수 있어야 한다.

2. `constexpr` 을 될 수 있으면 사용한다. 

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "fix: SGlobalSetting refactoring and move to namespace"
[fix_core_setting 94d2347] fix: SGlobalSetting refactoring and move to namespace
 3 files changed, 200 insertions(+), 168 deletions(-)
 rewrite OPGS16/Include/Core/core_setting.h (96%)
```

`SGlobalSetting` 클래스를 네임스페이스로 옮기는데 성공했다. 덤으로 함수들의 개명도 했다.

