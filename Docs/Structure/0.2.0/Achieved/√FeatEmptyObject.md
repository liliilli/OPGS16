# EmptyCanvas & EmptyObject 넣기

> 2018-07-25 문서 작성

## 문제

`OPGS16` 은 `Element` 로 `CCanvas` 와 `CObject` 타입을 가지고 있다. 하지만 이 둘을 `CreateGameObject` 으로 넣기에는 베이스 클래스일 뿐더러 내부 구조가 노출될 수 있는 단점을 지닌다.

따라서 이를 랩핑하는 `EmptyCanvas` 와 `EmptyObject` 클래스를 만들어, 특별한 동작을 가지지 않고 인스턴싱 후 기믹들을 추가할 캔버스, 오브젝트에 대해서는 해당 타입을 사용해 `CreateGameObject` 을 사용할 수 있도록 한다.

## 제안

기존 프로젝트에서 임의로 구현해서 사용한 `EmptyCanvas` 와 `EmptyObject` 을 `Element/Builtin` 폴더에 삽입한다.

## 구현 사항

기존 프로젝트에서 임의로 구현해서 사용한 `EmptyCanvas` 와 `EmptyObject` 을 `Element/Builtin` 폴더에 삽입한다.

## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add EmptyCanvas, EmptyObject"
[feat_empty_object a776f7e] feat: Add EmptyCanvas, EmptyObject
 4 files changed, 33 insertions(+)
 create mode 100644 OPGS16/Include/Element/Bulitin/empty_canvas.h
 create mode 100644 OPGS16/Include/Element/Bulitin/empty_object.h
```

