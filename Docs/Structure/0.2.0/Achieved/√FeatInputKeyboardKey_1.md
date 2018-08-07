# 키보드 인풋 키 바인딩 처리

> 2018-08-03 문서 작성

## 문제

`OPGS16` 에서는 `v.0.1.0` 에서 키 인풋을 골라야 할 때 다음과 같이 적지 않으면 안됬다.

``` c++
    "YAxis": {
      "+": 265,
      "-": 264,
      "gravity": 1000,
      "stick": false
    },
    "XAxis": {
      "+": 262,
      "-": 263,
      "gravity": 1000,
      "stick": false
    },
//...
```

265, 264 이런식으로 적는 이유는 `GLFW` 의 키바인딩 enum 이 265, 264 와 같은 값을 가졌기 때문이다. 하지만 이렇게 하면 프로젝트를 만들 때 일일히 어떤 키가 어떤 키인지를 확인해야 하기 때문에 매우 번거롭고 안 좋다.

## 제안 & 구현 사항

따라서, 다음처럼 키 세팅을 쓸 수 있도록 제안한다.

``` c++
    "YAxis": {
      "+": "KB_Up",
      "-": "KB_Down",
      "gravity": 1000,
      "stick": false
    },
    "XAxis": {
      "+": "KB_Left",
      "-": "KB_Right",
      "gravity": 1000,
      "stick": false
    },
//...
```

현재 OPGS16 `v.0.2.0` 에서 바인딩으로 지원할 키는 다음과 같다.

그리고 해당 키들에 따라서 세팅 파일에서 바인딩되는 스트링들은 다음과 같다.

#### 키 : Setting String 배열표

> 키보드 키는 항상 "KB_" 로 시작한다.

| Keyboard Key | String    | UID  |
| ------------ | --------- | ---- |
| LEFT         | KB_LEFT   | 263  |
| DOWN         | KB_DOWN   | 264  |
| UP           | KB_UP     | 265  |
| RIGHT        | KB_RIGHT  | 262  |
| A            | KB_A      | 65   |
| B            | KB_B      | 66   |
| ...          | ...       | ...  |
| Y            | KB_Y      | 89   |
| Z            | KB_Z      | 90   |
| 0            | KB_0      | 48   |
| 1            | KB_1      | 49   |
| ...          | ...       | ...  |
| 8            | KB_8      | 56   |
| 9            | KB_9      | 57   |
| ESCAPE       | KB_ESCAPE | 256  |
| ENTER        | KB_ENTER  | 257  |

일차로 우선 이것들을 구현한다.

만약 JSON 라이브러리를 사용해서 불러올 때 스트링이 아니거나 해당되지 않는 것들이 있다면 바로 에러를 발생시킨다.

6시까지 하도록 할 것. 마우스 및 기타 키들은 실장된 후에 함.

#### 2018-08-03 PM 04:37

추가로 더 바인딩하자.

| Keyboard Key  | String        | UID  |
| ------------- | ------------- | ---- |
| LEFT_CTRL     | KB_LCTRL      | 341  |
| LEFT_ALT      | KB_LALT       | 342  |
| LEFT_SHIFT    | KB_LSHIFT     | 340  |
| INSERT        | KB_INSERT     | 260  |
| DELETE        | KB_DELETE     | 261  |
| HOME          | KB_HOME       | 268  |
| END           | KB_END        | 269  |
| PAGE_UP       | KB_PAGEUP     | 266  |
| PAGE_DOWN     | KB_PAGEDOWN   | 267  |
| MINUS         | KB_MINUS      | 45   |
| EQUAL         | KB_EQUAL      | 61   |
| LEFT_BRACKET  | KB_LBRACKET   | 91   |
| RIGHT_BRACKET | KB_RBRACKET   | 93   |
| SEMICOLON     | KB_SEMICOLON  | 59   |
| APOSTROPHE    | KB_APOSTROPHE | 39   |
| COMMA         | KB_COMMA      | 44   |
| PERIOD        | KB_PERIOD     | 46   |
| SLASH         | KB_SLASH      | 47   |

#### 2018-08-03 PM 04:57

3차로 F1~F12 와 캡스락, Tab, ~, Backspace 및 역슬래쉬를 추가한다. 
키패드와 우측 쉬프트, ALT, CTRL은 지원하지 않기로 한다.

| Keyboard Key | String       | UID  |
| ------------ | ------------ | ---- |
| F1           | KB_F1        | 290  |
| F2           | KB_F2        | 291  |
| ...          | ...          | ...  |
| F12          | KB_F12       | 301  |
| CAPS_LOCK    | KB_CAPSLOCK  | 280  |
| TAB          | KB_TAB       | 258  |
| BACKSLASH    | KB_BACKSLASH | 92   |
| GRAVE_ACCENT | KB_GBACCENT  | 96   |
| BACKSPACE    | KB_BACKSPACE | 259  |

## 결과

#### 2018-08-03 PM 04:32

방향키, A~Z, 0~9 및 ESCAPE, ENTER 를 바인딩하는데 성공함.

#### 2018-08-03 PM 04:55

2차 키 세팅 스트링 바인딩 완료

#### 2018-08-03 PM 05:07

위 기본 77 Key 스트링 세팅 완료