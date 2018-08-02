# Quadratic Bezier Curve 구현

> 2018-07-28 문서 작성

## 문제

`Bezier Curve` 는 한 점과 한 점 사이에 다수의 점과 그 점들 사이의 보간된 점을 사용해 곡선을 그리는 방법 중 하나이다. 비교적 구현이 간단하고, 다수의 점이 있을 경우에도 재귀로 간단하게 해결이 가능하다는 점에서 많이 사용된다.

하지만 `OPGS16` `v0.1.0` 에서는 기본적인 베지어 커브를 구현하고 있지 않다. 따라서 `math` 네임스페이스에서 `DVector2` 와 `DVector3` 에 대해 Quadratic 베이저 커브를 구현하는 함수를 제공하고자 한다.

## 제안

1. `opgs16::math` 함수에서 `DVector2` 와 `DVector3` 에 대한 Quadratic Bazier Curve 함수를 구현해서 제공하도록 한다.
2. offset 이 0 이하이거나 1 이상이어도 상관하지 않는다.
3. `DVectorInt2` `DVectorInt3` 에 대해서는 아직 구현하지 않는다...

## 구현 사항

1. `feat_math_quadbezier` 브랜치를 생성한다.
2. 다음과 같이 시그니쳐를 구현한다.

``` c++
DVector2 GetQuadBezierCurvePoint(const DVector2& lhs, 
                                 const DVector2& rhs, 
                                 const DVector2& ctl1,
                                 float offset);

DVector3 GetQuadBezierCurvePoint(const DVector3& lhs, 
                                 const DVector3& rhs, 
                                 const DVector3& ctl1,
                                 float offset);
```

3. 함수 바디를 실장한다.

## 결과

시그니쳐에서 `ctl1` 은 이미 다른 헤더에 의해 `#define` 이 되어있기 때문에 쓸 수 없었다. 대신에 `control` 이라고 사용햇다.

``` c++
DVector2 GetQuadBezierCurvePoint(const DVector2& lhs,
                                 const DVector2& rhs,
                                 const DVector2& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset),
              Lerp(control, rhs, offset),
              offset);
}

DVector3 GetQuadBezierCurvePoint(const DVector3& lhs,
                                 const DVector3& rhs,
                                 const DVector3& control,
                                 float offset) {
  return Lerp(Lerp(lhs, control, offset),
              Lerp(control, rhs, offset),
              offset);
}
```

기존에 구현한 `Lerp` 을 사용해서 간단하게 구현 완료.

``` powershell
no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "feat: Add quadratic bezier curve function"
[feat_math_quadbezier c7e5564] feat: Add quadratic bezier curve function
 2 files changed, 34 insertions(+)
PS D:\Development\OPGS16> git checkout -
Switched to branch '0.2.0'
Your branch is ahead of 'origin/0.2.0' by 4 commits.
  (use "git push" to publish your local commits)
PS D:\Development\OPGS16> git merge -
Updating e70ea3a..c7e5564
Fast-forward
 OPGS16/Include/Helper/Math/math.h | 16 ++++++++++++++++
 OPGS16/Source/Helper/Math/math.cc | 18 ++++++++++++++++++
 2 files changed, 34 insertions(+)
PS D:\Development\OPGS16> git push
```

