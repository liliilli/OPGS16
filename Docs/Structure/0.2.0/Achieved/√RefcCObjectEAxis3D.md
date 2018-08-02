# EDirection to EAxis3D

> 2018-07-31 문서 작성

## 문제 & 제안

> 여기서 CObject 에서 EAxis3D 을 쓰지 않고 독자 DirectionType 을 쓰는 것을 확인할 수 있었는데, 이 역시 리팩터링이 당장 필요할 것 같다.
>
> 발췌 `FeatCObjectAddOffset.md` 

CObject 에서 사용하는 `_internal::DirectionType` 을 모조리 `opgs16::EAxis3D` 로 고친다. 그리고 `DirectionType` 파일 자체를 지워서 쓸 여지를 없애버린다.

## 구현 사항

_internal::DirectionType 을 지우고, `opgs16::EAxis3D` 으로 교체한다.

브랜치 명은 `refc_replace_eaxis3d` 로 한다.

## 결과

``` powershell
no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "refc: Delete _internal::DirectionType replace with EAxis3D"
[refc_replace_eaxis3d b5f0de7] refc: Delete _internal::DirectionType replace with EAxis3D
 9 files changed, 57 insertions(+), 93 deletions(-)
 delete mode 100644 OPGS16/Include/Element/Internal/direction_type.h
PS D:\Development\OPGS16> git checkout -
Switched to branch 'feat_aabb_color'
PS D:\Development\OPGS16> git merge -
Updating 4357baa..b5f0de7
Fast-forward
 OPGS16/Include/Element/Impl/object_impl.cc         | 12 ++---
 OPGS16/Include/Element/Impl/object_impl.h          | 61 +++++++++++-----------
 OPGS16/Include/Element/Internal/direction_type.h   | 31 -----------
 OPGS16/Include/Element/object.h                    | 16 +++---
 OPGS16/OPGS16.vcxproj                              |  1 -
 OPGS16/OPGS16.vcxproj.filters                      |  1 -
 .../Component/Physics/prot_rigidbody_collider2d.cc |  7 ++-
 OPGS16/Source/Element/object.cc                    | 19 +++----
 OPGS16/Source/Helper/Math/math.cc                  |  2 +-
 9 files changed, 57 insertions(+), 93 deletions(-)
 delete mode 100644 OPGS16/Include/Element/Internal/direction_type.h
```

