# 파티클 소멸시키기

> 2018-08-08 문서 작성

## 문제

파티클 스포너 (Spawner) 을 가진 오브젝트가 소멸되었거나 할 때, `IsSleep()` 이 `True` 가 아니라면 마지막 파티클의 생명주기가 끝날 때 까지 해당 파티클 스포너를 유지시켜야 한다. 하지만 지금 구현이 되어있질 않다.

## 제안 & 구현 사항

* 오브젝트가 삭제되서 갈 곳이 없는(소유주가 없는) `Spawner` 을 옮길 수 있는 컨테이너를 `CObject` 에 옮긴다. 옮겨진 스포너는 더 이상 모든 이미터에 대해 파티클을 생성할 수 없으며, 단지 파티클이 생명주기가 다해 `Deactivate` 되는 것만을 조작할 수 있다.
* 오브젝트가 삭제되는 것 뿐만 아니라, `RemoveComponent` 등으로 컴포넌트가 삭제가 될 때도 `CObject` 의 파티클 스포너 소멸 컨테이너에 옮긴다. 문제는 `RemoveComponent` 등을 할 경우에는 `erase` 가 되면서 자동으로 소멸자가 불려와 지는데, 소멸자에서 오브젝트를 다른 곳으로 옮기게 되면 RAII 으로 구현되지 않았기 때문에 미정의 행동을 일으킬 수 있다.
* 따라서... 오브젝트가 소멸 할 때 `Component` 에 대해서는 소멸시키지 않고 자동으로 파티클 소멸 컨테이너에 옮길 수 있어야 한다.

#### 브랜치

`fix_particle_survived` 라는 브랜치를 생성해서 작업한다.

## 결과

#### 2018-08-09 PM 09:15

우선 컴포넌트에서 명시적으로 제거를 할 때 (`RemoveComponent<>`) 리소스를 이동시켜서 `CObject` 의 `m_spawner_list` 에 이동을 시킨 후, `IsSleep()` 을 사용해서 모든 파티클들이 다 죽었는지 확인한 뒤에 제거를 하는 것 까지는 성공을 했다.

하지만 오브젝트 자체가 없어질 때, `pPopComponent<>` 을 활용해서 컴포넌트 자체를 옮기는데, 그러면 옮긴 후의 컴포넌트는 군데군데 `empty` 가 되어있기 때문에 성능 처리에도 안 좋을 뿐더러, 이미 빈 오브젝트에 대해서 참조를 하려고 하고 있기 때문에 오류가 일어난다.

이를 보정해줘야 한다. 보정해 주는 방법은, `unique_ptr<>` 는 힙의 포인터만을 저장한다는 것을 이용해서, `empty` 된 곳의 포인터와 뒤에서 부터 살아있는 `unique_ptr<CComponent>` 의 힙의 포인터와 해당 컴포넌트의 성질을 스왑하는 방식으로 처리한다.

#### 2018-08-09 PM 09:31

``` c++
template <
  class TType,
  typename = std::enable_if_t<std::is_base_of_v<_Component, TType>>
>
std::vector<std::unique_ptr<TType>> pPopComponents() {
  using opgs16::component::_internal::EComponentType;
  std::vector<std::unique_ptr<TType>> result_list;

  // m_componentsからTTypeであるコンポネントに対して他のところに移す。
  auto it = --m_components.end();
  int32_t remove_back_count = 0;
  for (auto& [component, item] : m_components) {
    if (!component) continue;
    if (component->DoesTypeMatch(OP16_GET_HASH(TType), TType::__string_literal)) {
      result_list.push_back(std::unique_ptr<TType>(static_cast<TType*>(component.release())));
      ++remove_back_count;

      while (it->second == EComponentType::Particle && &(it->first) != &component) {
        --it;
      }
      if (&(it->first) == &component) continue;
      component.swap(it->first);
      item = it->second;
      --it;
    }
  }
  // 移動した分だけ真後ろから削除する。
  while (remove_back_count > 0) {
    m_components.pop_back();
    --remove_back_count;
  }
  return std::move(result_list);
}
```

`CObject` 에 `pPopComponents` 라는 함수를 사용해서, 오브젝트가 삭제될 때 이 함수를 이용해 모든 파티클 컴포넌트들을 가져와 다른 곳에 이동시키는 메커니즘을 구현했다.

이동한 파티클 컴포넌트들은 생식 능력을 제거한 상태이지만, `CObject::Update()` 을 통해 남겨진 유효한 파티클 오브젝트에 대해서는 오브젝트 생명주기가 끝날 때 까지 업데이트와 렌더링을 할 수 있도록 했다.

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Make it possible to particle going on even though object disappear"
[fix_particle_survived 5eec12e] feat: Make it possible to particle going on even though object disappear
 6 files changed, 106 insertions(+), 59 deletions(-)
 rename OPGS16/Include/Component/Internal/{type.h => component_type.h} (90%)
```

