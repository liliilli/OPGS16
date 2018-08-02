# 텍스쳐 매니저 리팩터링

> 2018-07-29 문서 작성

## 문제

다른 매니저들은 `namespace` 을 활용해서 구현을 하고 있지만, `texture` 는 아직까지도 클래스 안의 매직 스태틱을 사용하고 있다. 따라서 `Texture` 을 불러오기에도 매우 번거롭고 `namespace` 로 한 단계 호출 단계를 내리면 더 좋을 것 같다는 생각을 한다.

## 제안



## 구현 사항



## 결과

``` powershell
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is ahead of 'origin/0.2.0' by 7 commits.
  (use "git push" to publish your local commits)
PS D:\Development\OPGS16> git merge -
Updating f5a3857..6a0379c
Fast-forward
 OPGS16/Include/Manager/texture_manager.h     | 186 ++++++++++-----------------
 OPGS16/Source/Component/sprite2d_renderer.cc |   5 +-
 OPGS16/Source/Manager/scene_manager.cc       |   2 +-
 OPGS16/Source/Manager/texture_manager.cc     | 108 ++++++++--------
 4 files changed, 124 insertions(+), 177 deletions(-)
PS D:\Development\OPGS16> git status
```

옮기긴 했으나, `TextureManager` 의 상태가 생각외로 심각했다. API 도 없을 뿐만 아니라, 완전히 고정되어 있는 상태이며 `Texture` 의 반환, 생성 도 `.cc` 파일을 보지 않고서는 아예 알 수 없는 상태였다...

아무래도 이 부분에 대해서는 대대적으로 개편을 해야할 것 같다. 

또한 텍스쳐 자체도 `VAO` 와 같이 카운팅을 사용해서 함부로 `Release` 하지 않도록 해야하 것 같음. 애초에 `Release` 라는 함수 자체가 맞지 않고, 텍스쳐에도 `GC` 을 사용해야 할 것 같다.