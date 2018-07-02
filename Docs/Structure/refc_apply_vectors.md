* `Physics` 부분은 건드리질 못하겠음. 나중에 하는 것으로 하자.

  * `rigidbody_2d.h` 도 나중에.

* `animator.h` 의 `size_t` 수정 필요

* `DMatrix4x4` 혹은 `DMatrix3x3` 가 필요할 것 같음. 근데 성능이 더 떨어질듯...

* [ ] `Component/camera.h` 의 `CCamera` 자체의 리팩터링도 필요함.
  `Component/camera.cc` 포함.

* [x] `Component/camera.h` `CCamera` 수정 완료.
  * `glm::mat4` 와 같은 매트릭스 타입은 아직 그대로 둠.
  * 인덴트 역시 그대로 둠.

* [x] `Component/font2d_renderer.h` 에서 `glm::vec3 m_color` 을 수정하기 전에, `DColor3` `DColor4` 을 만들어야 했음. 왜냐면 `float` 색상은 각 채널마다 $$ [0, 1] $$ 의 범위만을 가지기 때문에, `DVector3` 으로 그대로 두면 컬러가 맛이 갈 확률이 높음.
  * ~~만약 색상이 노멀을 가지고 있다면 어떻게 해야함? 따라서 `DColor3` 에서 `DVector3` 으로 변환을 해야하지 않나?~~
    ~~그런 경우를 대비해서 `DVector3` 과 `DColor3` 은 서로 `explicit` 하게 변환될 수 있도록 해야함.
  * `DColor` $$ (r, g, b, a) $$ (Float) 을 만들어서 대체함.
  * `position` 은 그대로 뒀음. 이걸 고칠려면 `opgs16::element::CObject` 까지 거슬러 올라가야 함.

* [ ] `shaer_wrapper.h` 의 `SetUniformValue` 역시 고칠 것.

  * [ ] `font2d_renderer.cc` 의 290 줄 고칠 것.

    ``` c++
      if (m_color_dirty == _internal::EDirtyFlag::Dirty) {
        m_wrapper.SetUniformValue("textColor",
          static_cast<glm::vec3>(static_cast<DVector3>(m_color)));
    ```

* [ ] `preprocessing2d_renderer.h` `.cc` 는 따로 리팩터링을 해야함. 
  일단 `vao` RAII 자동 해제는... 만들어야 함.

* [ ] `SetUniformValue` 관련 죄다 뜯어 고칠 것.

  * [ ] `sprite_renderer_impl.cc` 리팩터링

* [ ] `Element/object.h` 의 `CreateChildTag` 을 `Scene` 에서도 사용할 수 있게 할 것. (현재 Scene 에서는 같은 이름의 오브젝트를 생성하면 생성이 안되거나 오류가 발생함.)