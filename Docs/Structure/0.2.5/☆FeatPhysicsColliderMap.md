# CCollider 가 다른 CCollider 을 참조

> 2018-08-01 문서 작성

## 문제



## 제안 & 구현 사항

우선, `CCollider` 가 만들어질 때마다, 해쉬 값이 되는 값 인덱스를 가져야 한다. 이 때 해쉬 값은, `CCollider` 내부 `private` 의 `static int32_t` 값이 올라가면서 정해진다. 이 해쉬값은 다른 `CCollider` 와 충돌을 할 때 플래그 비교분석에 필요하므로 중요하다.

## 결과

