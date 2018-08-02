# object::Destroy() 에 Recursive 옵션을 주기

> 2018-07-27 문서 작성

## 문제

현재 `object::Destroy()` 함수는 재귀적으로 오브젝트 트리의 모든 노드를 거치면서 적절한 오브젝트 노드가 찾을 때 까지 순회하고 있다. 문제는 유저가 현재 오브젝트의 트리 구조를 알고 있고, 해당 오브젝트에서 바로 밑 레벨의 오브젝트를 지우고자 할 때에는 매우 시간이 낭비가 될 수 밖에 없다.

## 제안

따라서 현재 `v0.1.0` 에서 구현된 `opgs16::manager::object::Destroy()` 의 함수가 다음과 같을 때

``` c++
bool Destroy(const element::CObject& object, element::CObject* root);
```

세 번째 인자로 `is_recursive` 라는 `boolean` 타입의 옵션 인자를 붙인다. 이 경우 옵션 인자는 이전 구현체와의 호환성을 위해서 기본 값으로 `false` 을 사용한다. 왜 기본으로 재귀를 선택하지 않냐고 하면은, 게임을 개발하는 유저 입장에서는 임의 오브젝트가 루트인 서브 트리에서 2 단계 이상 아래의 오브젝트를 삭제하는 경우는 거의 없기 때문이다.

## 구현 사항

1. `feat_object_destruction_option` 브랜치를 추가한다.

2. **별도의 함수 추가 없이** `object::Destroy()` 의 세번째 인자 `is_recursive` 을 추가해서 기본값을 `false` 로 한다.

3. 함수 바디에서 `is_recursive` 가 `false` 인 경우에는 한 단계 아랫만을 훑는다. 그렇지 않은 경우 재귀적으로 호출한다.

4. `Scene` 에서도 오브젝트를 삭제할 수 있도록 함수를 제공한다.

5. `CObject::DestroyObject` 에서도 재귀적으로 삭제할 수 있도록 API 을 변경한다. 이 때 `is_recursive` 인자의 기본값 역시 `false` 가 된다.

   * ``` c++
     bool CObject::DestroyGameObject(const std::string& child_name,
                                     bool is_recursive)
     ```

     `child_name` 은 recursive 을 하지 않도록 하자. 왜냐면 내부 로직에서 호출하는 `object::Destroy()` 는 오브젝트의 레퍼런스를 사용해서 제거를 실행하는데, 2 단계 이상의 오브젝트에 대한 `child_name` 을 사용해서 제거를 호출하는 것은 말이 안된다. => 오브젝트 레퍼런스를 쓰는 것을 권장한다.

6. `opgs16::manager::object::Destroy` 의 시그니쳐를 `::DestroyGameObject` 로 바꾼다. (통일성을 위해서)

## 결과

1. 내부 코드에서 `Hash value` 의 대조의 삭제
   * 왜냐면 메모리 침범 혹은 중복이 없는 한에서는 오브젝트 레퍼런스의 어드레스 값과 현재 트리가 가지고 있는 각 오브젝트의 어드레스 값만을 비교하면 되기 때문에 사전에 `Hash value` 을 비교하는 것은 비용 낭비이다.
2. `is_recursive` 의 여하에 따라서 로직을 다르게 함.

``` c++
bool DestroyGameObject(const element::CObject& object,
                       element::CObject* root,
                       bool is_recursive) {
  using TObjectMap = std::unordered_map<std::string, object_ptr>;
  using TObjectItType = TObjectMap::iterator;

  if (!is_recursive) { // 추가함.
    TObjectMap* object_list = nullptr;

    if (!root)
      object_list = scene::GetPresentScene()->GetGameObjectList();
    else
      object_list = &root->GetGameObjectList();

    for (auto& [object_name, object_ptr] : *object_list) {
      if (object_ptr.get() == &object) {
        AddDestroyObject(object_ptr);
        return true;
      }
    }

    return false;
  }
  //...
}
```

3. 또한 `object::Destroy` 을 `object::DestroyGameObject` 로 네이밍 통일함.
4. `CScene::DestroyGameObject` 을 만듬.

``` c++
bool DestroyGameObject(const std::string& object_name);
bool DestroyGameObject(const element::CObject& object_reference,
                       bool is_recursive = false);
```

``` powershell
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is ahead of 'origin/0.2.0' by 1 commit.
  (use "git push" to publish your local commits)
PS D:\Development\OPGS16> git merge -
Updating dc43869..7d929fe
Fast-forward
 OPGS16/Include/Element/object.h         |  7 +++++--
 OPGS16/Include/Element/scene.h          | 18 +++++++++++++++++
 OPGS16/Include/Manager/object_manager.h |  6 ++++--
 OPGS16/Source/Element/object.cc         | 12 +++++------
 OPGS16/Source/Element/scene.cc          | 20 ++++++++++++++++++
 OPGS16/Source/Manager/object_manager.cc | 36 +++++++++++++++++++++++++--------
 6 files changed, 81 insertions(+), 18 deletions(-)
```

