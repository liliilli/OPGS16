# component::ScriptFrame

`CObject` 게임 오브젝트의 구체적인 행동을 결정한다.

## Script 의 생명주기

``` flow
_bg=>start: 시작
_in=>operation: Initiate()
_st=>operation: Start()
_up=>operation: Update()
_en=>operation: OnEnabled()
_di=>operation: OnDisabled()
_dt=>operation: Destroy()
_en_cond=>condition: SetActive(Enable)?
_di_cond=>condition: SetActive(Disable)? 
_dt_cond=>condition: RemoveComponent<Script>()?
_e=>end: 끝

_bg->_in->_st->_up->_di_cond
_di_cond(yes)->_di->_en_cond
_di_cond(no)->_dt_cond
_dt_cond(yes)->_dt->_e
_dt_cond(no)->_up
_en_cond(yes)->_en->_st
_en_cond(no)->_en_cond
```

