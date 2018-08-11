# 커스터마이즈한 파티클 이미터 컨테이너 삽입

> 2018-08-09 문서 작성

## 문제

현재 `CParticleSpawner` 에서는 텅빈 파티클을 생성하고 이 파티클에 모듈을 삽입해서 꾸며넣는 방식으로 동작하고 있다.

``` c++
  auto emitter  = particle->CreateEmptyParticleEmitter("Fire");
  emitter->SetTexture("Thinking");
  {
    auto& ref = emitter->GetSpawnModuleRef();
    ref.SetPeriod(2'000);
    ref.SetSpawnNumber(50);
    ref.SetSpawnStyle(opgs16::component::EParticleSpawnStyle::Batch);
    ref.SetLoop(true);
  }
  {
    using opgs16::component::CParticleModuleLifetime;
    auto lifetime = emitter->AddModule<CParticleModuleLifetime>();
    lifetime->SetLifeTime(2'000);
    lifetime->SetLifeTimeVariationOffset(500);
  }
  {
    using opgs16::component::CParticleModuleInitialSize;
    auto initial_size = emitter->AddModule<CParticleModuleInitialSize>();
    initial_size->SetInitialSize(32);
    initial_size->SetSizeVariationOffset(16);
  }
  // ... and so many module setting codes.
```

하지만 여러 오브젝트에 동일한 파티클을 사용하고 싶을 때, 일일히 이미터를 생성하고, 모듈을 생성하는 코드를 작성하는 것을 계속 반복하면 보기도 불편할 뿐더러 손의 수고가 든다.

따라서 `Emitter` 을 상속하게 해서, 별도의 `Emitter` 형에서는 모듈을 미리 생성할 수 있도록 하고자 한다.

## 제안 & 구현 사항

1. `CParticleEmitter` 을 상속하게 한다.
2. 상속한 `Derived` 한 임의 파티클 이미터는 생성자에서 모듈 등을 설정할 수 있도록 한다.

## 결과

* 우선 이미 만들어진 이미터를 `ParticleSpawner` 에 바인딩하기 위해, 템플릿 함수를 구현했다.

``` c++
template <
  typename TEmitterType,
  typename = std::enable_if_t<std::is_base_of_v<CParticleEmitter, TEmitterType>>
>
CParticleEmitter* CreateParticleEmitter() {
  const char* name = OP16_GET_LITERAL_NAME(TEmitterType);
  if (GetParticleEmitter(name)) {
    // ...
  }
  auto [it, result] = m_emitter.try_emplace(
      name, 
      std::make_unique<TEmitterType>(GetBindObject())
  );
  // ...
  return it->second.get();
}
```

이제 `Derived` 된 이미터는 스포너에 위 함수를 사용해서 바인딩을 할 수 있게 된다.

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add User derived emitter type binding function"
[feat_particle_system 9ec89b2] feat: Add User derived emitter type binding function
 2 files changed, 28 insertions(+), 2 deletions(-)
```

