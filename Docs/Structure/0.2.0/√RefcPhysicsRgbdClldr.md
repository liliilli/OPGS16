# Rigidbody & Collider 리팩터링

> 2018-08-02 문서 작성

## 문제

리팩터링을 하자.

## 제안 & 구현 사항

`FeatPhysicsCRigidbodyBase` 에서 구현된 타입들을 리팩터링 한다.

## 결과

`CColliderBase` 완료
`CRigidbodyBase` 완료

`CCollider2DBase` 완료
`CCollider3DBase` 완료

`CRigidbodyDynamicBase` 완료
`CRigidbodyStaticBase` 완료

`CColliderBox2D` 완료
`CRigidbody2D` 완료
`CRigidbodyStatic2D` 완료

``` powershell
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   OPGS16/Include/Component/Internal/collider2d_base.h
        modified:   OPGS16/Include/Component/Internal/collider3d_base.h
        modified:   OPGS16/Include/Component/Internal/collider_base.h
        modified:   OPGS16/Include/Component/Internal/rigidbody_base.h
        modified:   OPGS16/Include/Component/Internal/rigidbody_dynamic_base.h
        modified:   OPGS16/Include/Component/Internal/rigidbody_static_base.h
        modified:   OPGS16/Include/Component/Physics/collider2d_box.h
        modified:   OPGS16/Include/Component/Physics/rigidbody2d.h
        modified:   OPGS16/Include/Component/Physics/rigidbody2d_static.h
        modified:   OPGS16/Include/Element/Internal/physics_collider_bind_info.h
        modified:   OPGS16/Include/Element/Internal/physics_enums.h
        modified:   OPGS16/OPGS16.vcxproj
        modified:   OPGS16/OPGS16.vcxproj.filters
        modified:   OPGS16/Source/Component/Internal/aabb_renderer_base.cc
        modified:   OPGS16/Source/Component/Internal/collider_base.cc
        modified:   OPGS16/Source/Component/Internal/rigidbody_base.cc
        modified:   OPGS16/Source/Component/Internal/rigidbody_dynamic_base.cc
        modified:   OPGS16/Source/Component/Physics/collider2d_box.cc
        modified:   OPGS16/Source/Component/Physics/rigidbody2d.cc
        modified:   OPGS16/Source/Component/Physics/rigidbody2d_static.cc

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        Docs/Media/0723_resolution.png
        Docs/Media/0724_fixstretch1.png
        Docs/Media/0724_fixstretch2.png
        Docs/Media/1532386335513.png
        Docs/Media/1533002662115.png
        Docs/Media/img23_preprecomp-down.PNG
        Docs/Media/resolution_comparaison.png
        Docs/Structure/0.2.0/
        OPGS16/Include/Helper/include_vector.h
        OPGS16/setting.sav

no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "refc: Refactoring"
```

