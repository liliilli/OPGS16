# DVectorInt2

* [x] `int32_t` 의 x, y 멤버 설정

* [x] 생성자 설정

  * [x] glm `glm::ivec2` 생성자 구현
  * [x] int32_t `const int32_t` 생성자 구현
  * [x] 일반 `int32_t 2` 생성자 구현
  * [x] 일반 `DVectorInt2` Copy CTOR 구현

* [x] 대입 연산자 구현

  * [x] glm `glm::ivec2` 구현

* [x] 일반 연산자 오버로딩 구현

  * [x] `operator+(DVectorInt2)`
  * [x] `operator-(DVectorInt2)`

  * [x] `operator*(int32_t)` `Float` 으로 곱하면 소수점이 잘려나간다.
  * [x] `operator*(DVectorInt2)`
  * [x] `operator/(int32_t)`
    * 만약 value 가 $$0$$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator/(DVectorInt2)`
    * 만약 `DVectorInt2` 의 값 중 하나라도 $$ 0 $$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator+=(DVectorInt2)`
  * [x] `operator-=(DVectorInt2)`
  * [x] `operator*=(int32_t)`
  * [x] `operator*=(DVectorInt2)`
  * [x] `operator/=(int32_t)`
    * 만약 value 가 $$0$$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator/=(DVectorInt2)`
    * 만약 `DVectorInt2` 의 값 중 하나라도 $$ 0 $$ 이면 아무것도 안하고 기존 벡터를 넘김.
  * [x] `operator<(DVectorInt2, DVectorInt2)` private
  * [x] `operator>(DVectorInt2, DVectorInt2)` private
  * [x] `operator<=(,)` private
  * [x] `operator>=(,)` private
  * [x] `operator==(,)`

* [x] 일반 멤버 함수 구현

  * [x] `GetLength()`
  * [x] `GetSquareLength()` 
  * [x] ~~`Normalize()`~~ 정수형만을 담는 벡터에 노멀라이즈는 필요없는 것 같다.
  * [x] `Data()` :: 일차원 데이터 뭉치를 전달함.

* [ ] `Static` 함수 구현

  * [x] `Dot` 구현

  * [x] ~~`Max`~~ 구현 (DVectorInt2 와 DVectorInt2 을 비교해서 무엇이 더 큰가를 반환)

    * `CompMaxLength` 로 바꿈. `Max` 는 의도를 전혀 파악하지 못 하고 있음.

  * [x] ~~`Min`~~ 구현 (DVectorInt2 와 DVectorInt2 을 비교해서 무엇이 더 작은가를 반환)

    * `CompoMinLength` 로 바꿈. `Min` 은 의도를 전혀 파악하지 못 하고 있음.

  * [ ] 빌트인 객체 제공 구현
    * [ ] `Up` $$ (0, 1) $$
    * [ ] `Down` $$ (0, -1) $$
    * [ ] `Right` $$ (1, 0) $$
    * [ ] `Left` $$ (-1, 0) $$
    * `One` 이나 `Zero` 는 기본 생성자로 만드는 편이 간단하기 때문에 구현하지 않는다.

* [x] 변환 연산자 구현

  * [x] `DVector2` 으로 명시적인 변환 구현