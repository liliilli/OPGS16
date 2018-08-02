# 사운드 (Effect, Bgm) 볼륨 조절

> 2018-07-24 제안
> 2018-07-25 추가 (함수)

## 문제

`OPGS16` 의 버전 `0.1.0` 에서 제공하고 있는 사운드는 볼륨 조절을 지원하지 않을 뿐더러, 채널이라는 개념이 없다. 채널이라는 것은 임의 사운드가 출력이 될 때 공통적으로 지나가는 곳이며, 이 채널을 통해서 해당 효과음의 제어권을 가질 수 있게 된다.

## 제안

#### 채널 레이어 구현

채널 레이어를 구현한다. 일반 상용 게임의 경우, 효과음 및 배경음 뿐만 아니라 세부적으로 채널을 나뉘어서 해당 채널마다 각기의 볼륨을 조절할 수 있도록 하고 있다.

예를 들면, "차 경적 소리", "주위 사람들 소리", "총기 소리", "배경음", "빗소리" 등과 같은 식으로...

그래서 여타 다른 레이어들과 마찬가지로 사운드 채널 역시 `setting.meta` 에 다음과 같이 구현한다.

``` json
{
    "sound":
    [
        "Car", "Gun", "People", "Weather", "Background"
    ]
}
```

그러고 나서, `resource.meta` 을 사용해서 사운드 정보를 가져올 때는 다음과 같이 써서, 어떤 채널을 사용할지를 정한다.

``` json
"sound":
{
   "meta": { },
   "list":
   {
      "BgmSchool":
      {
        "path": "Resource/Audio/Bgm/Schooldays.mp3",
        "type": "back",
        "channel": "Background"
      },
      "EffDecision":
      {
        "path": "Resource/Audio/Effect/decision.mp3",
        "type": "effect",
        "channel": "People"
      }
   }
}
```

#### 채널의 `Pause` 및 `Resume` 및 `Volume` 설정을 구현함.

1. 각 채널 그룹에서 재생하고 있는 것들을 일시정지한다.
2. 각 채널 그룹에서 일시정지 하고 있는 것들을 다시 재생시킨다.
3. 각 채널 그룹의 볼륨을 0 에서 100 까지 설정할 수 있다.
   만약 설정 값이 해당 범위를 넘으면 $$ < 0 $$ 의 경우 $$ 0 $$ 으로, $$ > 100 $$ 의 경우 $$ 100 $$ 의 값으로 재설정한다.
   * 왜 `float` 이 아닌가? 하면... 소수점으로 설정하면 원하는대로 나오지가 않을 가능성이 나오니까...

## 구현 사항

#### 채널 레이어를 구현

현재 위와 같은 방식으로 채널 레이어를 구현하는 것은 시간 비용이 많이 들 뿐더러 그냥 미리 만들어진 `Effect` 채널과 `Background` 채널, 그리고 이를 통합하는 `Master` 채널을 만들어서 `json` 형식의 `"type"` 에 따라 채널을 달리 삽입하게 하는 것이 더 좋을 것 같다는 생각을 했다.

* `setting.meta` 에서 새로운 헤더를 만들지 않는다.
* `resource.meta` 의 사운드 형식은 `0.1.0` 을 따른다.

다만 Sound manager 가 초기화될 때, 성공적으로 초기화가 되면 `Effect` 와 `Background` , 그리고 `Master` 채널을 만들고 음량, 등등을 관리하게 한다.

#### 채널의 `Pause` 및 `Resume` 및 `Volume` 설정을 구현함

``` c++
bool PauseSoundChannel(opgs16::resource::ESoundType sound_channel_type);
bool ResumeSoundChannel(opgs16::resource::ESoundType sound_channel_type);
bool MuteSoundChannel(opgs16::resource::ESoundType sound_channel_type);
bool UnmuteSoundChannel(opgs16::resource::ESoundType sound_channel_type);
bool PauseMasterSoundChannel();
bool ResumeMasterSoundChannel();
bool MuteMasterSoundChannel();
bool UnmuteMasterSoundChannel();
void SetVolumeSoundChannel(opgs16::resource::ESoundType sound_channel_type,
                           int32_t volume_value);
void SetVolumeMasterChannel(int32_t volume_value);
bool IsSoundChannelMuted(opgs16::resource::ESoundType sound_channel_type);
bool IsSoundChannelPaused(opgs16::resource::ESoundType sound_channel_type);
bool IsMasterSoundChannelMuted();
bool IsMasterSoundChannelPaused();
```

각 사운드 채널은 다음과 같은 기능을 얻는다.

* 채널 뮤트 / 언뮤트
* 채널 볼륨 조절
* 채널 사운드 정지
* 채널 사운드 일시정지 / 재개

#### 음향 테스트 화면 설정



## 결과

``` powershell
PS D:\Development\OPGS16> git commit -m "feat: Add effect/background channel, and control functions"
[feat_sound_feature 24b82f0] feat: Add effect/background channel, and control functions
 9 files changed, 747 insertions(+), 161 deletions(-)
```

향후 지켜봐야 할 가능성 있음...

