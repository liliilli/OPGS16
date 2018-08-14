# 새로 오브젝트 위치, 회전, 스케일 전파 하기

> 2018-08-12 문서 작성

## 문제



## 제안 & 구현 사항

### A. 이동

#### 1. 오브젝트가 Scene 에 생성될 경우

* `parent` 는 `nullptr` 이 된다. `Scene` 에서 오브젝트를 만들 경우에는 `nullptr` 을 설정한다.
* 오브젝트 자체에서 부모 오브젝트의 전파 위치를 가져오는 `flag` 는 `clean` 하다. 따라서 첫 갱신 때 포지션을 가져오지 않는다.
* 앞으로 생길 다른 오브젝트들을 위해서 미리? 혹은 lazy 하게 전파 포지션을 갱신한다.
  전파 포지션은 오브젝트의 스케일, 월드 회전에 영향을 미친다.

#### 2. 오브젝트가 CObject 의 아래에 생성될 경우

* `CObject->CreateGameObject` 의 함수로 생성이 됬을 경우 (정상적으로 생성을 시도할 경우) 함수 내부에서 `parent` 을 `CObject` 의 포인터 값으로 변경한다.
  * 만약 `CObject` 가 바로 삭제된다고 하면, 해당 오브젝트의 트리의 자식 오브젝트 역시 삭제가 되므로 댕글링의 위험은 없다고 본다.
* 그리고 나서, 해당 생성된 오브젝트의 경우 맨 처음에 `Activate` 가 되서 `Update()` 가 될 경우, `first_position_get_flag` 가 `dirty` 이면 부모로부터 포지션 위치 조정을 실시한다. 다만 `nullptr` 일 경우에는 (정상적일 경우 부모가 없고 자신이 씬 루트 바로 밑임) 아무것도 하지 않는다. 트랜스폼을 가져올 경우, 현재 **부모 오브젝트에서 **
* `UIObject` 의 경우에는 또 다르긴 하지만 이는 별도로...

#### 구현

* `World` Scale 과 `Local` Scale 은 부모의 최종 `Axis` 축에 따라서 늘려진다. 최종 `Axis` 축은 합한 `World` 회전 각의 각도와 같다.
* 부모를 가지는 오브젝트의 `Local` 과 `World` 포지셔닝은 부모의 최종 `Axis` 축에 따라서 이동된다.
* 오브젝트가 `World` 회전을 했을 경우, 오브젝트가 부모로서 자식에게 전달할 `Axis` 축은 이전 부모의 축과 달라진다.
* 오브젝트가 `World` 스케일을 조정했을 경우, 오브젝트가 부모로서 자식에게 전달할 `Scale` 곱값은 이전 부모와 달라진다.

#### 전달 방식

* Activate 된 각 Update 에서,

  ``` c++
  m_data->SetObjectWorldAxisBasisValue(m_parent->pGetParentWorldAxisValue());
  m_data->SetObjectWorldRotationBasisValue(m_parent->pGetParentSummedWorldRotationAngle());
  m_data->SetObjectWorldScaleBasisValue(m_parent->pGetParentProductedWorldScaleValue());
  
  ```

  을 호출해서 현재 오브젝트에서 사용할 축 (전파 축), 합 각도, 곱 스케일을 가져온다.

#### `CObjectImpl::pUpdatePropagationAxisBasis`

``` c++
void CObjectImpl::pUpdatePropagationAxisBasis() noexcept {

}
```

1. `m_is_world_propagation_axis_dirty` 플래그는 `World` 합 각도 혹은 자신의 `World` 각도가 갱신이 되었을 때 `dirty` 가 된다. 따라서 월드(`m_world_rotation_euler_angle`) 혹은 부모로부터의 각도(`m_propagated_world_rotation_euler_angle`) 가 들어올 때마다 `pUpdateSummedWorldRotationEulerAngle` 함수을 사용해 `m_summed_world_rotation_euler_angle` 을 갱신한다.

#### `m_object_space_axis`

1. `Parent` 로부터 각을 받아올 때 갱신한다. 그러면 `m_object_space_axis` 는 x, y, z 축이 회전에 따라 갱신이 되며 `position` 이 이 축에 따라서 다시 갱신되어야 한다.

#### `m_object_propagate_axis`

1. `Child` 에게 축을 전달할 때 갱신한다. 이 축의 값을 받는 함수는 `GetChildObjectWorldAxisBasisValue` 밖에 없고, `m_is_world_propagation_axis_dirty` 가 dirty 일 때만 갱신할 수 있도록 늦은 계산을 구현한다.

#### `CObjectImpl::SetObjectWorldRotationBasisValue`

``` c++
void SetObjectWorldRotationBasisValue(const DVector3& propagated_world_rotation_angle) 
    noexcept {
  m_propagated_world_rotation_euler_angle = propagated_world_rotation_angle;
  pUpdateObjectSpaceAxisBasis();

  m_offset_model_matrix_deprecated = true;
  m_final_pos_deprecated = true;
  m_is_local_model_matrix_deprecated = true;
  m_is_world_propagation_axis_dirty = true;
}
```

부모로부터 **이미 계산되고 정렬된 합 각도**를 가져와 붙인다. 이 함수가 사용 될 때는 `Scale` 을 제외한 `Axis` 및 `Position` 의 재갱신이 필요하다. `Scale` 은 왜 제외됬냐면, 좌표축에 독립적이며 회전을 통해서 좌표축과 동기화를 할 수 있기 때문이다.

### B. 스케일 전파

`Local` 은 마지막에 곱한다. `World` 는 전파한다. 둘 다 기본값으로는 $$ (x, y, z) $$ 모두 $$ 1 $$ 을 가진다.

`World` 역시 각 오브젝트마다 자기만의 `World` , 그리고 부모로부터 전파된 `WorldPropagated` , 자식에게 전달할 `WorldProducted` , 마지막으로 로컬을 곱한 `FinalScale` 을 가진다.

* `WorldProducted`  = `WorldPropagated` * `World`
* `FinalScale` = `WorldProducted` * `Local`

`WorldProducted` 의 기본 값은 $$ 1 $$ 로 한다.

#### Flags

1. `LocalScale` 을 갱신할 때, `m_is_final_scale_dirty` 및 `m_is_model_matrix_dirty` 을 갱신한다.
2. `WorldScale` 을 갱신할 때, `m_is_final_scale_dirty` 와 `m_is_model_matrix_dirty` 그리고 `m_is_producted_scale_dirty` 을 갱신한다.
3. 부모로부터 `WorldPropagated` 을 받아서 갱신할 때, `m_is_final_scale_dirty` 와 `m_is_model_matrix_dirty` `m_is_producted_scale_dirty` 을 갱신한다.

이름이 `m_is_producted_scale_dirty`  좋지 않으므로 `m_is_summed_scale_dirty` 로 개명.

## 결과

#### 2018-08-13 PM 03:27

1차로 올림. 로컬 + 월드 + 전파 = 파이널 구현이 안되있기 때문에 이걸 구현한다.

#### 2018-08-14 AM 10:19

성능은 아직 보장되지 않았으나, 일단 쿼터니언을 사용해서 짐벌락 및 회전 문제를 해결하는데 성공했다. 여러가지 변수가 쓰였으며 쿼터니언 및 매트릭스 연산 등은 플래그에 의해 `Lazy` 하게 연산되도록 했다.

#### 2018-08-15 AM 11:57

``` c++
PS D:\Development\OPGS16> git commit -m "feat: (5) Implement world scale, propagation"
[feat_internal_xyz_renderer aaefaf6] feat: (5) Implement world scale, propagation
 5 files changed, 104 insertions(+), 58 deletions(-)
    
PS D:\Development\OPGS16> git commit -m "feat: (6) Refactoring"
[feat_internal_xyz_renderer e23f6ff] feat: (6) Refactoring
 3 files changed, 14 insertions(+), 27 deletions(-)
```

