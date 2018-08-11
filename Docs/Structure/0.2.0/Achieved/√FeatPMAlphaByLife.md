# CParticleModuleAlphaByLife 구현

> 2018-08-09 문서 작성
> 2018-08-09 구현 완료

## 문제

생명주기에 따라서 파티클의 알파 투명도가 변하도록 한다.

## 제안 & 구현 사항

기존의 `CParticleModuleScaleByLife` 을 사용한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add CParticleModuleAlphaByLife"
[feat_particle_system c8b28f0] feat: Add CParticleModuleAlphaByLife
 2 files changed, 223 insertions(+)
 create mode 100644 OPGS16/Include/Component/ParticleModule/module_bylife_alpha.h
 create mode 100644 OPGS16/Source/Component/ParticleModule/module_bylife_alpha.cc
```

구현 완료