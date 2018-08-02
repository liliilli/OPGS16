# Min 과 Max 를 반환하는 math:: 함수

## 문제

일반 `std::Vector` 혹은 `std::array` 혹은 일반 Array 에서 `min` 과 `max` 수치를 구하려면 다음과 같이 수행한다.

``` c++
auto [min, max] = std::minmax_element(v.begin(), v.end());
// min and max is iterator.
```

그런데 `opgs16::DVector3` 과 같은, `opgs16` 자료형은 이터레이터를 지원하지 않을 뿐만 아니라 이터레이터가 구현하는 `algorithm` 의 각종 펑션을 사용하기 매우 힘들다. 따라서 `minmax_element` 을 `opgs16::math` 네임스페이스에 구현하고자 한다.

## 제안

지원할 타입은 다음과 같다.

* `std::list` OK
* `std::vector` OK
* `opgs16::DVector3` OK
* `opgs16::DVector2` OK
* `opgs16::DVector4` NOT IMPLEMENTED
* `opgs16::DVectorInt3` OK
* `opgs16::DVectorInt2` OK
* `opgs16::DVectorInt4` NOT IMPLEMENTED
* 일반 C Array OK
* `std::array`  OK

그리고 C++11 에서 구현된 `std::minmax_element` 은 쓰지 않는다. 이터레이터라는 레이어를 덧씌우기 때문에 성능이 보다 덜 나온다고 함.

> 근데 `std::list` 와 `std::vector` 만 할 것이 아니라, forward iterator 을 가지고 있는 모든 컨테이너에 대해서 구현을 시키면 되지 않을까? 싶은데. 하지만 `std::map` 계열에서는 해당 minmax 을 쓰지 말도록 하고 싶음. 게다가 `iterator` 조건부를 검색하려고 하면 `template` 을 써야하는데 그러면 쓸데없이 많이 복잡해진다.
>
> 결론은 그래서 지정된 `std::list` 와 `std::vector` 타입만 오버로딩으로 구현하자. 이 오버로딩은 컴파일 타임에 이루어지기 때문에 별 상관 없음.

## 구현 사항

1. `feat_math_getminmax` 브랜치를 생성해서 위 제안의 타입들을 구현한다.
2. 구현 시그니쳐는 다음과 같다.

``` c++
///       min    max
std::pair<float, float> GetMinMax(const TType& value);
```

3. 문제 발생. `std::vector<>` `std::list` 의 경우에는 해당 컨테이너의 타입에 따라 다르기 때문에 이를 템플릿으로 반환형에 바인딩을 해줘야함.

``` c++
template <typename TValueType>
std::pair<TValueType, TValueType> 
GetMinMax(const std::vector<TValueType>& value);
```

## 결과

``` c++
PS D:\Development\OPGS16> git status
On branch 0.2.0
Your branch is ahead of 'origin/0.2.0' by 3 commits.
  (use "git push" to publish your local commits)

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

        modified:   OPGS16/Include/Helper/Math/math.h
        modified:   OPGS16/Source/Helper/Math/math.cc

Untracked files:
  (use "git add <file>..." to include in what will be committed)

        Docs/Media/0723_resolution.png
        Docs/Media/0724_fixstretch1.png
        Docs/Media/0724_fixstretch2.png
        Docs/Media/1532386335513.png
        Docs/Media/img23_preprecomp-down.PNG
        Docs/Media/resolution_comparaison.png
        Docs/Structure/0.2.0/
        OPGS16/Include/Helper/Type/axis.h
        OPGS16/Include/Helper/include_vector.h
        OPGS16/setting.sav

no changes added to commit (use "git add" and/or "git commit -a")
PS D:\Development\OPGS16> git add -u
PS D:\Development\OPGS16> git commit -m "feat: Add math::GetMinMax function series"
[0.2.0 b1b3e2b] feat: Add math::GetMinMax function series
 2 files changed, 151 insertions(+), 1 deletion(-)
```

`DVector4` 와 `DVectorInt4` 을 제외하고는 구현 완료.