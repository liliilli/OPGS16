## CRigidbodyBase 만들기

> 2018-08-01 문서 작성

## 문제



## 제안 & 구현 사항

1. 한 `CObject` 의 `CCollider` 는 생성 시에는 바인딩이 되지 않지만, `CRigidbody` 가 만들어진 이후에 `CRigidbody` 가 해당 `CObject` 의 `CRigidbody` 에 적합한 `CCollider` 의 포인터를 가져와 바인딩해서 생성을 하도록 해야 한다. 근데 그러면 `Static` 은 어떻게 되는거임? ㅋㅋㅋ
   * `Static` 역시 `CRigidbody` 계열의 컴포넌트를 생성해서 바인딩하지만, 다만 `Static` 만을 위한 `CRigidbody` 를 사용한다. 이를 `CStaticRigidbody` 라고 하자.

그러면 지금 구조는 다음과 같이 된다.

* `CComponent` => `CRigidbodyBase`  OK
* `CRigidbodyBase` => `CRigidbodyDynamicBase` OK
  `CRigidbodyBase` => `CRigidbodyStaticBase` OK
* `CRigidbodyDynamicBase` => `CRigidbody2D` OK
  `CRigidbodyDynamicBase` => `CRigidbody3D`
* `CRigidbodyStaticBase` => `CRigidbodyStatic2D`  OK
  `CRigidbodyStaticBase` => `CRigidbodyStatic3D`
* `CRigidbody2D` & `CRigidbodyStatic2D` => `CCollider2DBase` 계열의 컬라이더들 가짐
* `CRigidbody3D` & `CRigidbodyStatic3D` => `CCollider3DBase` 계열의 컬라이더들 가짐

#### `CPrivateRigidbodyCollider2D` 를 `CCollider` 자체로 분리하기

우선 `CRigidbodyDynamicBase` 는 `Kinematic` `Dynamic` 을 지원한다. `Gravity` 사용도 지원한다. 총 질량 `Mass` 도 지원한다. `LinearFactor` 도 지원한다.

* `ColliderSize` 는 지원하지 않고, `CCollider` 에서 지원해야 한다.

1. `CCollider` 와 `CRigidbody` 가 있다. `CRigidbody` 는 `CCollider` 들을 가져와서 총 질량을 잴 것이다. 총 질량을 잰 후에, 비율에 따라서 분배를 한다. 만약 처음부터 `Kinematic` 이라면 모두 키네매틱으로 한다.
2. 또한 같은 `Rigidbody` 에 속해있는 `CCollider` 사이에는 충돌 판정을 일으키지 말아야 한다.

* `CCollider` 와 `CRigidbody` 가 있을 때, `CRigidbody` 는 `pFindNewColliderOnObject` 함수를 사용해 `CObject` 의 `CCollider` 을 찾아 바인딩한다. 그리고 `CCollider` 의 `pInitializeCollider` 을 사용해 리소스를 구축한다.

  * 여기서 `CRigidbody2D` 의 경우, 각 `CCollider` 에 공통으로 
    전달해야 할 값들은 다음과 같다. (`CColliderBase`)

  1. Mass
  2. IsKinetic or Dynamic or Static (`EColliderActualType` 을 사용하자)
  3. LinearFactor `TLinearFactorSetting`
  4. Gravity 사용 여부

  그리고 `CCollider` 는 이 전달된 값을 자기 사본으로 저장한다.

## 결과 

#### 2018-08-01 PM 08:43

우선 기본적인 실장은 다음과 같다.

`CRigidbody2D` :: `pEnrollColliderOnObject` 을 구현하며, 오직 `CCollider2DBase` 가 모태인 `CCollider` 만 받아서 바인딩을 한다.

`CRigidbodyDynamicBase` :: 질량, 키네마틱 여부, 중력, 축 제한 등을 설정할 수 있으며, `Dynamic` 혹은 `Kinematic` 중에서 선택을 할 수 있는 `CRigidbody2D` 의 부모형이다.

`CRigidbodyBase` :: 바인딩된 `CCollider` 의 포인터들을 저장한다. 여기서 `pFindColliderInContainer` `pSetColliderInContainer` 등이 구현되어 컨테이너 안에서 유효된 컬라이더의 포인터를 받아오거나 세팅할 수 있다. 다만 이 동작은 `CColliderBase` 에서만 가능하다. 만약 임의 `CRigidbody` 가 소멸이 된다면, 해당 리기드바디에 바인딩 된 모든 `CCollider` 을 바인딩 해제한다. `CCollider` 에 상호 연결된 리기드바디 포인터 역시 `nullptr` 이 된다.

`CColliderBox2D` :: btBox2dShape 을 사용해서 컬리젼 영역을 구현한다. 이 `CCollider` 의 경우에는 사이즈를 사용해서 초기화를 시켜줘야 한다. 또한 컬리젼 영역을 다르게 할 수도 있는데 이 경우에는 상당한 오버헤드가 있다. 아직 컬라이더를 캐시에 추가하는 것은 하지 않았다. 

> 아직 Collision 쪽 함수는 위쪽으로 빼지 않았다. 내일이나 모레 할 것.

`CCollider2DBase` :: `2D` 컬리젼 영역들의 베이스가 되는 타입이다. 

`CColliderBase` :: 해당 컬리젼의 고유 `uid` 와 `CRigidbodyBase` 에서 컬리젼 타입, 질량 등의 값을 받아서 갱신한다. 또한 고유 기능으로 `Trigger` 혹은 `Collision` 이 가능하다. 이 역시 소멸 시 `rigidbody` 가 유효한지를 확인해서 유효해서 (즉 아직 바인딩이 되어있으면) 리기드바디에 자신의 컬라이더를 삭제하라고 요청 한 뒤에 소멸된다.

#### 2018-08-01 PM 10:21

`CRigidbodyStaticBase` :: ~~질량, 중력, 키네마틱 여부~~ 를 제외한 축 제한 만을 설정할 수 있으며 오직 `Static` 으로만 `CCollider` 가 움직인다.

> 또 문제가 있는데, `CRigidbodyDynamicBase` 에서 유효화된 컬라이더에 대해 전파가 안되는 것...이 있다. 또한 `m_collider_container` 에서는 유효화된 컬라이더만 있기 때문에 바로 전파하면 된다.

아무튼 속성의 Propagation 은 완료했음.

#### 2018-08-01 PM 11:26

``CRigidbodyStaticBase` 의 구현 완료. 그리고 `InitiateColliders` 을 맨 최하위 `Rigidbody` 에 구현했다. 이 함수는 수동으로 컬라이더를 생성하게 해서 바인딩하게 하는 함수이다. 컬라이더를 만듬과 동시에 프로퍼티를 조작해서 조작된 프로퍼티가 적용되면서 컬라이더가 바인딩하게 하고 싶을 때 사용할 수 있다.

#### 2018-08-01 PM 11:50

우선 `opgs16::element::_internal::DPrivateColliderBindInfo` 의 `bind_collider` 을 치워야 한다. 그런데 `CColliderBox2D` 을 덜 추상화된 상위 타입으로 옮기기 전에 일단 `CColliderBox2D` 의 충돌 부분에 관련된 함수를 상위 타입으로 옮겨야 겠다.

근데... 또 문제가 뭐냐면. 이건 `CCollider2DBase` 가 아니라 `CColliderBase` 로 옮겨야 할 것 같다. 지금 `2D` 와 `3D` 사이에 충돌이 될 수도 있기 때문에 이걸 어떻게 좀 고려해봐야 할 것 같으니.

#### 2018-08-02 PM 01:00

일단 끝. 그런데 분리를 하고 나니 `refraction` 이 0 인 상황에서, `Dynamic` 이고 `Trigger` 가 아닌 컬라이더의 충돌이 계속 `Stay` 가 일어난다. 이는 원래 이래야 하는걸까? 흠...

리팩터링이 필요하다.

남은 기능은 두 브랜치로 옮긴다.

1. 리팩터링 `refc_rigidbody_collider` 완료
2. `2D` 및 `3D` 컬리더 구현 `feat_collider_implement`