# Texture 텍스쳐 리팩터링

> 2018-08-08 문서 작성

## 문제



## 제안 & 구현 사항

다음과 같이 `CTextureBase` 을 베이스로 해서 텍스쳐 타입을 전개해 나간다.
지금 `CTexture2DSprite` 와 `CTexture2DPlain` 의 두 가지 타입으로 나뉜다.

#### `CTextureBase`

* `CTexture_` 시리즈의 베이스이다.
* `Protected` 로 텍스쳐의 id 을 가진다.

#### `CTexture2DSprite`

* Mipmap 을 사용하지 않는다.
* `Initiate()` 로 생성을 할 때 `resource::STexture2DAtlas` 을 인자로 받는다.
  실패할 수도 있기 때문에 `CTexture2DSprite` 의 생성자에서 생성을 하지 않는다.
* 내부에서 `RAII` 을 구현한다. 다만 복사 생성자, 이동 생성자 등을 구현할 때  멋대로 릴리즈 되지 않도록 한다.

#### `CTexture2DPlain`

* 동적 및 여러 형태로 텍스쳐를 만들기 때문에, 이 역시 Mipmap 을 사용하지 않는다.
* `Initiate()` 로 생성을 할 때, 인자를 여러개 넣어서 사용한다.
* 내부에서 `RAII` 을 구현한다. 다만 복사 생성자, 이동 생성자 등을 구현할 때  멋대로 릴리즈 되지 않도록 한다.

#### `CTexture2DNormal`

* 정적으로 2D 텍스쳐를 불러오며, `Mipmap` 을 사용한다.

#### `CTextureCubeMap`

* 

## 결과

#### 2018-08-08 PM 11:31

분리는 했지만, 렌더링이 안된다. 이게 어떻게 된 일일까...