# OPGS16 기본 동작

`OPGS16` (Open Game Simulator 16) 의 동작은 다음과 같다.

``` flow
app_begin=>start: プログラム実行
app_init=>operation: プログラム初期化 (Initiate())
manager_hello=>operation: 各種マネジャー初期化 (opgs16::manager::xx::Initaite())
resource_begin=>operation: リソースパス読み込み
run=>operation: 駆動 (Run())
update=>operation: 更新 (Update())
rendering=>operation: レンダリング (Rendering())
manager_bye=>operation: 各種マネジャーリソース削除 (Shutdown())
end=>end: プログラム終わり
cond_shutdown=>condition: 終了

app_begin->app_init->manager_hello->resource_begin->run->update->rendering->cond_shutdown
cond_shutdown(yes)->manager_bye->end
cond_shutdown(no)->update
```

## Update()

Update() 에서는 `Scene` => `Object` => `Component` 순으로 구성된 트리를 차례대로 훑어가면서 내부의 각종 요소의 갱신을 수행한다. 또한 `Rendering` 과 관련된 `Component` 는 렌더링 레이어에 따라 차후에 `Rendering()` 과정에서 우선 순위에 따라 렌더링이 될 수 있도록 격납될 수 있다.

하지만 `Object` 가 비활성화 되어있거나, `Component` 가 비활성화 되어있을 경우에는 해당 오브젝트 혹은 컴포넌트 아래로 구성된 자식 오브젝트들 역시 비활성화 되어 갱신이 되지 않는다.

`Object` 을 활성화, 비활성화 하기 위해서는 다음과 같이 쓴다.

``` c++
auto text = obj.Instantiate<CText>("Text", "Stage 01");
text->SetAlignment(IAlignable::Alignment::CENTER);
text->SetFontName("opSystem");
text->SetFontSize(8);
text->GetComponent<opgs16::component::CEmptyRenderer>()->SetRenderLayer(3);
text->SetActive(false);
```

`Component` 을 활성화, 비활성화 하기 위해서는 다음과 같이 쓴다.

``` c++
m_cursor->GetComponent<opgs16::component::CSprite2DRenderer>()->
    SetComponentActivation(phitos::enums::EActivated::Disabled);
```

### More depth...

`Update()` 안에서는 `opgs16::manager::input` 매니저를 사용해서 각 스크립트의 `Update()` 함수 바디에서 키 입력을 감지할 수 있다. 또한 `Update()` 는 인자로 이전 프레임과 현재 프레임의 시간 간격을 `float` 형태로 전달한다. 여기서 `float` 값인 `delta_value` 는 $$ 1 $$ 이 1 초로 기준이 되어있다.

`Update(float delta_time)` 안에서의 작동 메커니즘은 다음과 같다.

``` flow
update_begin=>start: Update(float delta_value)スタート 
update_callback=>operation: 内部Callback()呼び出し
update_prerender=>operation: PrerenderingのUpdate()呼び出し(manager::prerendering::Update())
update_input=>operation: インプットマネジャーのUpdate()呼び出し(manager::input::Update())
update_object_gc=>operation: オブジェクトマネジャーにて削除オブジェクトのGC処理(manager::object::Update())
update_scene=>operation: 現在場面(Scene)でのオブジェクト更新(manager::scene::GetPresentScene()->Update(delta_time))
update_physics=>operation: 物理エンジンのアップデート(manager::physics::Update())
update_postprocessing=>operation: ポストプロセッシングをアップデートする(manager::postprocessing::UpdateSequences())
end=>end: Update(float delta_value) 終了

update_begin->update_callback->update_prerender->update_input->update_object_gc->update_scene->update_physics->update_postprocessing->end
```

* `Prerendering` 매니저의 `Update()` 는 절차적 텍스쳐 등의 텍스쳐 표현에 필요한 각종 요소들을 업데이트할 수 있도록 한다.
* `Input` 매니저의 `Update()` 는 키 눌림, 키 릴리즈, 반복 입력 인지와 축 (`Axis`)로 표현되는 포지티브 축과 네거티브 축 키의 중립 상태로 돌아가려는 중력 처리를 수행한다.
* `Object` 매니저의 `Update()` 는 런타임 도중 더 이상 쓰지않게 되서 삭제될 오브젝트들의 메모리를 반환하고 이로 인해 생긴 오브젝트 트리의 빈 공간을 메꾸는 역할을 한다.
* `Scene` 매니저의 `Update()` 는 현재 작동하고 있는 장면 `Scene` 에 바인딩 된, 활성화된 오브젝트들과 해당 오브젝트들에 바인딩 된 각종 컴포넌트들을 갱신한다.
* `Physics` 매니저의 `Update()` 는 오브젝트에 바인딩된 물리 엔진의 요소들의 충돌, 물리 현상들을 계산하는 역할을 한다.
* `PostProcessing` 매니저의 `Update()` 는 후처리 모듈에 필요한 각종 요소들을 업데이트 한다.

## Rendering()

`Rendering()` 에서는 `Update()` 에서 갱신된 요소들을 가지고 화면을 렌더링한다. 물론 전체 오브젝트가 다 렌더링되지는 않고, 현재 장면 `Scene` 안에 존재하며, 오브젝트가 렌더링 가능한 컴포넌트를 가지고, 해당 오브젝트 및 컴포넌트가 비활성화가 되지 않았을 때만 렌더링 대상에 포착된다.

그 외에 `Prerendering` 부분의 렌더링, `PostProcessing` 부분의 후처리 렌더링, 디버그 모드 사용 시 `AABB` 충돌박스의 선형 렌더링 등이 `Rendering()` 함수에서 실행이 된다.

### More depth... 

#### PostProcessing 시퀀스의 렌더링 방법 (Chaining)

![1531560661698](D:\Development\OPGS16\Docs\Structure\Images\1531560661698.png)

`PostProcessing` 매니저는 여러가지 후처리 쉐이더들을 일련의 리스트로 연결시켜서 중복된 후처리 렌더링이 가능할 수 있도록 설계되어 있다. 

　예를 들면, 최종 화면을 흑백 처리 하는 `opGray` 라는 후처리 이펙트 프레임 버퍼 `CPostProcessingFrame` 과 최종 화면을 아날로그 TV 화면처럼 가장자리가 굴곡 지도록 `opConvex` 만드는 후처리 프리임 버퍼가 있다고 하자. 여기서 후처리 매니저의 기능을 활용하면 `opGray` 와 `opConvex` 을 하나의 시퀀스로 연결시켜서 최종 화면이 흑백 화면에 아날로그 TV 의 굴곡짐이 적용되는 화면으로 렌더링될 수 있도록 할 수 있다는 것이다. 이것을 바로 Chaining 이라고 한다.

![1531562188176](D:\Development\OPGS16\Docs\Structure\Images\1531562188176.png)

다만 이 Chaining 의 맨 뒤에는 `opDefault` 라고 하는, 현재 창의 사이즈에 따라 렌더링된 결과물을 확대 혹은 축소를 해주는 기본 내장 후처리 프레임 버퍼가 항상 렌더링 된다.

런타임 도중에 연결된 시퀀스를 해제하는 것도 가능하며, 다시 설정하는 것도 가능하다.

