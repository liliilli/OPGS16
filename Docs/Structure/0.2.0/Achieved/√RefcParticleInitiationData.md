# 파티클 초기 데이터 리팩터링

> 2018-08-09 문서 작성

## 문제

현재 파티클을 생성할 때에는 다음과 같이 일일히 데이터 값과 플래그를 넣어줘야 한다.

``` c++
const auto lifetime         = GetModule<CParticleModuleLifetime>();
const auto initial_size     = GetModule<CParticleModuleInitialSize>();
const auto initial_color    = GetModule<CParticleModuleInitialColor>();
const auto initial_alpha    = GetModule<CParticleModuleInitialAlpha>();
const auto initial_velocity = GetModule<CParticleModuleInitialVelocity>();
const auto initial_initacc  = GetModule<CParticleModuleInitialAccelation>();

if (initial_size) { object.pfSetInitialSize(initial_size->GetInitialSize()); }
if (lifetime) { object.pfEnableLifetime(lifetime->GetLifeTime()); }
if (initial_color) { object.pfSetInitialColor(initial_color->GetInitialColor()); };
if (initial_alpha) { object.pfSetInitialAlpha(initial_alpha->GetInitialAlpha()); };
if (initial_velocity) { object.pfSetInitialVelocity(initial_velocity->GetInitialVelocity()); }
if (initial_initacc) { object.pfSetInitialAccelation(initial_initacc->GetInitialAccelation());}
if (GetModule<CParticleModuleScaleByLife>()) {
  object.pfSetFlag(true);
}
if (GetModule<CParticleModuleAlphaByLife>()) {
  object.pfSetAlphaFlag(true);
}
```

## 제안 & 구현 사항

따라서 `Initial` 한 모듈에서 값을 가져오고자 할 때, 그리고 `ByLife` 계열의 모듈에서 플래그를 가져오고자 할 때, 특수한 데이터 컨테이너에 값을 담아서 넘겨주는 것이 좋을 것 같다. 또한 넘겨줄 때 사용할 함수는 `private` 로 하고, 이미터에 대해 `friend` 로 설정해서 이미터만 파티클 오브젝트에 전달할 수 있도록 한다.

* 현재 `Initial` 모듈은 `Size`, `Color` `Alpha` `Velocity` `Accelation` 이 있으며, 파티클을 액티베이션 할 때 해당 모듈이 있는가에 대한 플래그와, 모듈이 있을 경우의 값들을 넣어서 파티클을 액티베이트 하기 전에 파티클에 전달해줘야 한다.

``` c++
///
/// @struct DParticleInitialData
/// @brief
///
struct DParticleInitialData {
  int32_t m_lifetime      = 0;
  int32_t m_initial_size  = 0;
  DColor  m_initial_color = DColor::White;
  float   m_initial_alpha = 1.f;
  DVector3 m_initial_velocity;
  DVector3 m_initial_accelation;
  DVector3 m_initial_postion;

  bool m_is_lifetime = false;
  bool m_is_initial_size = false;
  bool m_is_initial_color = false;
  bool m_is_initial_alpha = false;
  bool m_is_initial_velocity = false;
  bool m_is_initial_accelation = false;

  bool m_is_bylife_size = false;
  bool m_is_bylife_alpha = false;
};
```

위와 같이 파티클 액티베이션 시에 설정할 데이터들에 관한 컨테이너를 만든다.

* `CInternalParticleObject` 에서는 해당 데이터 컨테이너를 받아 플래그에 따라 유동적으로 설정할 수 있도록 한다.
* 또한, `bylife_` 가 종속되는 `initial_` 플래그가 설정되어 있지 않으면, 기본 값을 넣고 체크할 수 있도록 한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "refc: Refactoring particle object initialize data forwarding"
[feat_particle_system f633297] refc: Refactoring particle object initialize data forwarding
 2 files changed, 99 insertions(+), 58 deletions(-)
```

``` powershell
PS D:\Development\OPGS16> git commit -m "refc: (2)"
[feat_particle_system 9a6dfc0] refc: (2)
 2 files changed, 64 insertions(+), 8 deletions(-)
```

이제 여러가지 모듈을 구현해서 플래그 및 값을 넘겨야 할 때는 위 `DParticleInitialData` 컨테이너에 값을 집어넣으면 된다.