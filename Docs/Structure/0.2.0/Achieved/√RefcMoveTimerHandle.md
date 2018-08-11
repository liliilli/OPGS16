# 'CTimerHandle 을 Element 로 옮기기

> 2018-08-09 문서 작성

## 문제

현재 `CTimerHandle` 을 `element::` 네임 스페이스에 속함에도 불구하고 `Frame/` 폴더에 위치해 있다. 이것을 옮기자.

## 제안 & 구현 사항

1. 옮긴다.
2. 브랜치 명은 `refc_move_timerhandle` 로 한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git merge -
Updating ebb5b08..6f3f00b
Fast-forward
 OPGS16/Include/Core/Boot/__b_scr.h               |  2 +-
 OPGS16/Include/Element/timer_handle.h            | 78 ++++++++++++++++++++++++
 OPGS16/Include/Frame/timer_handle.h              | 77 -----------------------
 OPGS16/Include/Manager/Internal/timer_internal.h |  2 +-
 OPGS16/Include/Manager/timer_manager.h           |  2 +-
 OPGS16/OPGS16.vcxproj                            |  4 +-
 OPGS16/OPGS16.vcxproj.filters                    |  4 +-
 OPGS16/Source/Element/timer_handle.cc            | 47 ++++++++++++++
 OPGS16/Source/Frame/timer_handle.cc              | 47 --------------
 9 files changed, 132 insertions(+), 131 deletions(-)
 create mode 100644 OPGS16/Include/Element/timer_handle.h
 delete mode 100644 OPGS16/Include/Frame/timer_handle.h
 create mode 100644 OPGS16/Source/Element/timer_handle.cc
 delete mode 100644 OPGS16/Source/Frame/timer_handle.cc
PS D:\Development\OPGS16> git branch -av
```

