# 파일 로깅 기능

> 2018-08-11 문서 작성

## 문제

현재 로깅 기능은 콘솔에만 출력할 수 있도록 하고 있다.
이를 파일 로깅기능을 사용해서 출력하게 하면 좋을 것 같다.

## 제안 & 구현 사항

1. 브랜치 명을 `file_logging` 으로 사용한다.
2. `spdlog` 의 기능을 사용해 간단하게 파일 로그를 구현한다. 직접 구현하지 말 것.

#### IN DETAIL



## 결과

``` c++
What now> 7
Bye.
PS D:\Development\OPGS16> git commit -m "feat: Add manifest setting option and logging feature option, log to file"
[fix_shader_error_log d6e1bbc] feat: Add manifest setting option and logging feature option, log to file
 6 files changed, 164 insertions(+), 62 deletions(-)
```

#### 2018-08-11

* `manifest.h` 파일에서 `OP16_SETTING_ENABLE_LOGGING` `OP16_SETTING_ENABLE_LOGGING_TO_CONSOLE` `OP16_SETTING_ENABLE_LOGGING_TO_FILE` `OP16_SETTING_LOGGING_FILE_PATH` 옵션을 추가했다.
* `core_setting.h` 파일에서 로깅 옵션들에 대한 활성화 여부를 받아올 수 있는 API 들을 만들었다.
* `InitiateLogger()` 라는, 로깅 관리자를 초기화하는 함수를 만들어, 그 안에 각종 옵션들에 대해 유동적으로 로깅 싱크와 로거를 바인딩 할 수 있도록 하였다.

``` c++
if (!setting::IsEnableLoggingFeature())
  return;

if (setting::IsEnableLoggingConsole()) {
fdef _WIN32
  s_log_sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
lse
  s_log_sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
ndif
}

if (setting::IsEnableLoggingToFile()) {
  // Caution :: To let spdlog create log file to specific path,
  // directories must be created in advance.
  s_log_sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>("./logs/debug_log.txt", false));
}

s_logger = std::make_shared<spdlog::logger>("opgs16", s_log_sinks.begin(), s_log_sinks.end());
s_logger->set_level(spdlog::level::trace);

spdlog::register_logger(s_logger);
```

#### 앞으로 구현해야 할 것들

* 기간 별 로깅 파일
* 디렉터리 자동 인식 로깅 파일 기능
* 표시 레벨 조정 기능

