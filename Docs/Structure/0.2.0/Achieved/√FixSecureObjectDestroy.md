# CObject:: 에서의 DestroyGameObject 안전하게 하기

> 2018-07-27 문서 작성

## 문제

``` c++
bool CObject::DestroyGameObject(const std::string& child_name);
```

에서 오브젝트를 삭제할 때 항상 오브젝트를 삭제하는데 성공했음을 반환함.

## 제안

내부 로직에서는 `opgs16::manager::object::Destroy(obj, parent)` 을 사용해서 오브젝트의 삭제를 처리한다. 그런데 문제는 매니저쪽의 `Destroy` 에서 해당 오브젝트를 삭제하는 데 성공을 했는가 안했는가를 반환하는 값이 없다는 것이다. `Destroy()` 의 반환형은 void 이기 때문에 이를 `bool` 등으로 바꿔서 삭제가 됬는가 안됬는가를 확인해야 한다.

## 구현 사항

1. `fix_object_destruction` 브랜치를 만든다.
2. `opgs16::manager::object::Destroy` 의 반환형을 `Phitos::enums::ESuccess` 으로 바꾸고, 오브젝트가 실제로 삭제가 되었으면 `ESuccess::Succeed` 을, 그렇지 않으면 `ESuccess::Failed` 으로 반환한다.
   * 혹시나 `Phitos::enums` 는 내부 타입이기 때문에 유저에게 노출되서는 안되는데, 문제는 `object::Destroy` 의 경우 유저에서 직접 호출할 수 있다. 따라서 `Destroy()` 함수는 외부에 노출되어 있다. 그렇기 때문에 성공 플래그 반환 타입으로 `bool` 을 사용해야 한다.
3. `CObject::DestroyGameObject()` 내부 로직에서 해당 반환형의 값이 `false` 인가 `true` 인가를 확인해서 에러 플래그를 분기한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "fix: Change signature of Destroy to boolean for checking success"
[fix_object_destruction dc43869] fix: Change signature of Destroy to boolean for checking success
 3 files changed, 10 insertions(+), 10 deletions(-)
PS D:\Development\OPGS16> git brach -av
git: 'brach' is not a git command. See 'git --help'.

The most similar command is
        branch
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is up to date with 'origin/0.2.0'.
PS D:\Development\OPGS16> git merge -
Updating 689a021..dc43869
Fast-forward
 OPGS16/Include/Manager/object_manager.h |  2 +-
 OPGS16/Source/Element/object.cc         |  8 ++++----
 OPGS16/Source/Manager/object_manager.cc | 10 +++++-----
 3 files changed, 10 insertions(+), 10 deletions(-)
PS D:\Development\OPGS16>
```

구현 완료. 근데 문제는... recursive 가 구현이 안되있음.