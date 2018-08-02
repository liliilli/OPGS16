# Bullet3 충돌 Callback 구현하기

> 2018-07-31 문서 작성

## 문제

`Bullet3` 을 사용해서 충돌을 하고 기본적인 물리 연산을 적용하는 것은 좋으나, 이제 충돌 시의 `Callback` 을 사용해서 해당 `CObject` 의 `CScriptFrame` 의 콜백과, `AABBRenderer` 가 있을 경우 `AABBRenderer` 혹은 `CCollider` 의 상태 플래그를 변경해서 색상을 바꿔야 한다.

## 제안

충돌이 발생할 때의 충돌 콜백의 로직을 구현한다. 대략적인 구현 방법은 여기를 참고한다.

> http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers



## 구현 사항

1. `Unity 5` 의 API 을 참고해서 각 `CScriptFrame` 에 다음의 콜백 함수를 만든다. 이 때 콜백 함수는 `virtual` 을 사용한다.

``` c++
void OnCollisionEnter(Collider& collider);
void OnCollisionStay(Collider& collider);
void OnCollisionExit(Collider& collider);

void OnTriggerEnter(Collider& collider);
void OnTriggerStay(Collider& collider);
void OnTriggerExit(Collider& collider);
```

2. 그리고 `CPrivateAabbRenderer` 도 충돌이 됬는가, 안 됬는가에 따라서 색상 상태를 변경할 수 있도록 해야한다. 하지만 `CPrivateAabbRenderer` 계열은 `CCollider` 가 가지고 있기 때문에 `CCollider` 에게 상태를 변경할 수 있도록 하는 것이 더 좋을 것 같다.
3. 브랜치 명은 `feat_physics_callback` 으로 하자.

#### CObject 의 CScriptFrame Callback 구현에 대해

`Collider` 에서 `IsTriggered` 플래그를 세워놔야 한다. 위에서도 시그니쳐에서 있듯이, 컬라이더를 넘겨줘야 한다.

* `IsTriggered` 는 해당 컬라이더에 대해서 물리 효과를 끄되, 충돌 판정만을 남길지를 결정한다.
* `CRigidbody` 에 묶인 모든 컬라이더는 `Static` `Kinetic` `Dynamic` 을 공유한다.

`IsTriggered` 의 T/F 유무에 따라서 불려오는 충돌 콜백 함수의 종류는 다음과 같다.

| IsTriggered | True                          | False                            |
| ----------- | ----------------------------- | -------------------------------- |
| True        | L : OnTrigger / R : OnTrigger | L : OnTrigger / R : OnTrigger    |
| False       | L : OnTrigger / R : OnTrigger | L : OnCollision/ R : OnCollision |

그런데... 문제는 `IsTriggered` 을 비교하려고 하면, `IsTriggered` 는 `CCollider` 에 있고 이를 `CObject` 의 private 함수에서 비교하려면 인자가 2개 필요하거나, 쓸데없이 현재 Collider 와 일치하는 컴포넌트를 순회해서 가지고 와야한다는 불필요함과 성능 낭비가 존재한다.

**그래서 생각컨데 피 `CCollider` 에 충돌한 `CCollider` 을 넘기고 이를 바로 비교해서 어느 함수를 부를 것인지를 정한 다음, `GetBindObject()` 으로 오브젝트를 가져와 `private` 함수로 `CScriptFrame` 의 콜백을 불러올 수 있도록 하면 좋을 것 같다.**

> 여기서 문제는 `CScriptFrame` 역시 ComponentList 에 들어가 있는데, 스크립트 임을 파악하는데 O(n) 시간이 걸리고, 콜백을 호출하는데 O(n) 시간이 걸리기 때문에 충돌 콜백이 많이 일어날 경우 성능 저하가 있을 가능성이 있다. 여기에 대한 브랜치는 `RefcCScriptFrameContainer` 에 있다.

그리고 `CCollider` 마다 `Enter` 시점과 `Stay` , `Exit` 시점이 다를 수 있기 때문에 해당 `Flag` 들 역시 `CCollider` 에서 관리할 수 있도록 해야한다. `Enter` `Stay` `Exit` `Idle` 의 상태를 사용해서 플래그를 세운다.

그리고 도중에 컬라이더가 `Trigger` 가 된다거나 한다고 해도, 맨 처음에 `Enter` 가 될 때의 속성 그대로 `Exit` 까지 간다.

#### `EColliderCollisionState` 플래그 관리

* 기본 `CCollider` 의 충돌 상태 플래그는 `Idle` 이다.
* `Update()` 시퀀스는 한 프레임에 `CCollider->Update()` 후 물리 환경의 `Update()` 을 행한다. 그 후에 충돌이 발생하면 추가 로직으로 플래그가 변할 수 있다.

`pCallBindObjectCallback` 을 호출할 때 플래그 관리

1. `Idle` => `Enter` :: `On...Enter()` 을 호출한다. 이후 다음 프레임의 `CCollider->Update()` 에서는 `IsCollided` 플래그를 끄고 아무것도 하지 않는다.
2. `Enter` => `Stay` :: `On...Stay()` 을 호출한다. 만약 `Enter` 이후에 다음 프레임의 물리 환경의 `Update()` 에서 충돌이 다시 되서 호출이 됬다면, 상태를 바꾼다. 그리고 다음 프레임의 `CCollider->Update()` 에서는 `IsCollided` 플래그를 끄고 아무것도 하지 않는다.
3. `Stay` => `Stay` :: `On...Stay()` 을 호출한다. 상태를 바꾸지 않는다. 이하 (2) 와 같은 메커니즘을 가진다.
4. `Enter` => `Idle` :: `On...Exit()` 을 호출한다. (1) 의 마지막에서 또 다시 다음 프레임이 지났음에도 불구하고 `IsCollided` 플래그가 `true` 가 아닌 경우에, `Collider->Update()` 에서 상태를 `Idle` 로 바꾸고 `On...Exit()` 을 호출한다.
5. `Stay` => `Idle` :: `On...Exit()` 을 호출한다. (2) 의 마지막에서 또 다시 다음 프레임이 지났음에도 불구하고 `IsCollided` 플래그가 `true` 가 아닌 경우에, (4) 와 같은 메커니즘을 가진다.
6. `Idle` => `Idle` :: 아무것도 호출하지 않는다. `IsCollided` 가 항상 `false` 인 상태이며, 이 경우 현재 상태가 `Idle` 인가를 확인해서 `pCallBindObjectCallback` 을 호출하지 않도록 한다.

> 따라서 `Exit` 플래그는 필요하지 않다.
>
> 그런데 또 문제가, `On...Exit()` 역시 인자로 상대방의 컬라이더를 받는다는 것인데, 이 경우 컬라이더가 도중에 없어지면? nullptr 을 건네야 한다. 문제는 지금 메커니즘으로는 그러질 못 한다는 것이 있다.
>
> 근데 잘 생각해보니, 만약에 `On...` 컬라이더 함수에서 해당 컬라이더의 오브젝트를 삭제하면, 컬라이더 자체 역시 물리 효과에서 제외되기 때문에 애초에 컬라이더를 자체로 보관할 필요가 없을 것이다. 문제는... 여러 컬라이더가 동시에 호출이 되면, `Idle` => `Enter` 상태 변환이 각 컬라이더에 대해서 흘러가야 하는데, 그러질 못하고 `Idle` => `Enter` => `Stay` 바로 이렇게 호출되어 버린다.
>
> 이런 상황에서는... 해당 `Collider` 에 대한 개별의 상태를 가지는 `UnorderedMap` 을 사용하는 것이 더 좋지 않을까? 싶다. 하지만 이 경우 간접적으로 컬라이더를 내포하는 것이기 때문에 컬라이더가 족족 사라지게 되면 맵에 남아있는 각 컬라이더의 해쉬값이 그대로 남게 되는데, 이 경우 메모리 릭이 일어날 수 있다.
>
> 이에 대한 방법으로는... 각 컬라이더 혹은 전체 컬라이더를 관리하는 매니저에서 `GC` 을 두게 해서 해당 컬라이더가 특정 조건에 의해 `dangling` 한 상태라면 일정 주기 혹은 가변 주기로 `GC` 을 실행해서 컬라이더의 댕글링된 해쉬 키와 값을 지우도록 한다.
>
> 이는 일단 한 오브젝트에 대해서만 충돌 테스트를 한 다음에 쓰도록 하자.

#### 그래도 결국은 `OnExit` 는 항상 호출되는데?

그러면... 다음과 같이 하자.

1. `A` 와 `B` 가 충돌한다. `A` 와 `B` 는 `CCollider` 라고 하자.

2. `A` 에서 `B` 에 대해서 `On...Enter()` 을 호출할 때 `B` 에게 `A` 가 현재 `B` 을 쓰고 있음을 알린다. 그러면 `B` 에 대해서 `A` `CCollider` 포인터를 가지게 한다. `A` 에 대해서도 `B` `CCollider` 포인터를 가지게 끔 한다.

3. `A` 의 `On...Enter()` `On...Stay()` 의 함수 바디에서 `A` 자신이 죽어버리면?

   * `A` 는 앞으로 `On...Exit()` 을 불러올 `B` 에 대해서 자신이 죽었음을 알린다. 이 때 `B` 는 아무것도 하지 않는다. 아무것도 하지 않기 위해서는 `pCallBindObjectCallback` 을 호출할 때 피 컬라이더가 유효한가를 확인해야 한다. `A` 는 `B` 에게 자신이 죽었다는 것을 알렸기 때문에, 해쉬값이 유효하지 않으며, 따라서 아무것도 호출이 되지 않을 것이다.

4. `A` 의 `On...Enter()` `On...Stay()` 함수 바디에서 `B` 가 죽어버리면?

   * `B` 가 죽으면, `A` 에 대해서 자신이 죽었음을 알린다. `B` 는 죽었기 때문에 (1프레임 뒤에 소멸 당하지만서도) `pCallBindObjectCallback` 을 호출당해도 아무것도 하지 않는다. `A` 역시 `On....Exit()` 을 호출하지 않을 것이다. 왜냐하면 죽었기 때문에.

5. `A` 의 `On...Exit()` 은 어떻게 부르나?

   만약 아직까지 피충돌체 `CCollider` `B` 가 맵 안에서 유효하다면, 이터레이션을 돌려서 `On...Exit()` 을 호출한다.

이것 역시 별도의 브랜치를 만들어서 사용하자.

#### CScriptFrame 이 `Deactivated` 될 경우에는?

그러게... 플래그를 하나 더 만들어서 `Callback()` 에 대한 호출을 꺼버릴까?

## 결과

#### 2018-07-31 PM 02:59

우선, `Dispatcher` 에서 포인터 충돌 사본을 가져와 이터레이션을 돌려 현재 프레임에서 충돌된 `btCollisionObject` 가 가진 `OPGS16` 의 구현체 포인터를 가져오는 식으로 충돌이 될 때 발생하는 로그를 가져올 수 있었다.

``` powershell
[2018-07-31 15:02:12.971] [OPGS16] [critical] Object Collided. A : Player, B : Floor
[2018-07-31 15:02:13.003] [OPGS16] [critical] Object Collided. A : Player, B : Floor
```

이제 `CCollider` 쪽에서 충돌 플래그를 세워서 색상을 바꿀 수 있도록 해야한다. 이 때 충돌이 해제될 경우에는 `CCollider` 자체에서 충돌 플래그를 비교해서 색상을 원래 `Type` 의 것으로 돌려놓아야 한다.

#### 2018-07-31 PM 03:13

`CProtoRigidbodyCollider2D` 에 `m_is_collided_flag_setup` 이라는 불리언 플래그를 만들었다. 이 플래그는 매 `Update()` 에서 `false` 로 리셋이 된다. 물리 환경에서는 오브젝트가 충돌 할 때마다 위를 사용한다.

`Sleep` 을 지금 구현할까 생각도 했는데, 잠시 좀 뒤로 미뤄야 할 것 같다. 그리고 `Static` 한 오브젝트에 대해서는 `Sleep` 을 구현하지 않는다. `Sleep` 상태를 가지는 것은 `Kinetic` 과 `Dynamic` 뿐이다.

이제 `CObject` 의 바인딩 된 `CScriptFrame` 에 대한 Callback 을 사용한다.

#### 2018-07-31 PM 06:17

위에서 떠오른 잠재적인 문제들을 제쳐두고, 일단 물리 환경에서 `CCollider` 에 피`Collider` 을 넘기는 것은 했다. 일단 구현대로 `CScriptFrame` 에 콜백을 만드는 것을 해야겠다.

#### 2018-07-31 PM 09:00

단일 컬라이더에 대해서 `Enter` `Stay` `Exit` 구현 완료. 하지만 아직 위에 구현할 것들이 많이 남아있어서 좀... 해야한다. ㅠㅠ

#### 2018-07-31 PM 10:23

`FeatDrawAABBTest` 브랜치와 `FeatPhysicsAABBColor` 브랜치 기능 달성.

#### 2018-08-01 AM 12:58

한 프레임에 한번만 콜백을 부를 수 있도록 해야한다. 이에 대한 브랜치를 `FeatPhysicsCallbackJustOnce` 라고 해서 만들자. 만들었고 했다.

