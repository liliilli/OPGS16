# 컴파일된 쉐이더 에러로그 확인

> 2018-08-11 문서 작성

## 문제

`glsl` 쉐이더를 컴파일 하다보면, 쉐이더가 컴파일이 성공적으로 됬음에도 불구하고 에러 로그가 튀어나오는 경우가 있다.

``` powershell
[2018-08-11 20:09:32.616] [OPGS16] [error] Failed to compile shader. oW
[2018-08-11 20:09:32.617] [OPGS16] [info] Uniform #0 Type: 5126 Name: opAlpha Id : 0
[2018-08-11 20:09:32.618] [OPGS16] [info] Uniform #1 Type: 35676 Name: opModel Id : 1
[2018-08-11 20:09:32.618] [OPGS16] [info] Uniform #2 Type: 35664 Name: opOffset Id : 2
[2018-08-11 20:09:32.621] [OPGS16] [info] Uniform #3 Type: 35676 Name: opProj Id : 3
[2018-08-11 20:09:32.621] [OPGS16] [info] Uniform #4 Type: 35664 Name: opScale Id : 4
[2018-08-11 20:09:32.622] [OPGS16] [info] Uniform #5 Type: 35664 Name: opTexelLD Id : 5
[2018-08-11 20:09:32.622] [OPGS16] [info] Uniform #6 Type: 35664 Name: opTexelRU Id : 6
[2018-08-11 20:09:32.623] [OPGS16] [info] Uniform #7 Type: 35678 Name: opTexture0 Id : 7
[2018-08-11 20:09:32.623] [OPGS16] [info] Uniform #8 Type: 35676 Name: opView Id : 8
[2018-08-11 20:09:32.628] [OPGS16] [error] Failed to compile shader. ̊oW
```

이 에러로그를 수정하고자 한다.

## 제안 & 구현 사항

1. [Link](https://www.khronos.org/opengl/wiki/Shader_Compilation#Program_linking) 을 참고하여 쉐이더 로깅을 다시 구현한다.
2. `Shader` 타입 안의 요소들과 강하게 바인딩되지 않은 함수 및 요소들은 `.cc` 파일의 무명 스페이스로 이동시킨다.
3. 브랜치 명은 `fix_shader_error_log` 이라고 한다.

## 결과

#### 2018-08-11 PM 08:55

우선 각 쉐이더 별 컴파일 및 쉐이더 실패 시 할당 해제 및 빌트인 에러 쉐이더로 바꿔치기 등을 수행하려면, 다음과 같은 사항이 필요하다.

1. 빌트인 에러 쉐이더의 실장
2. 함수 리턴 값 형식 시그니쳐를 `bool` 등으로 바꿔서 실패했는가 아닌가를 확인할 수 있도록 함.
3. 루틴 자체에 break 등을 사용해 컴파일 혹은 링크 실패 시 사용할 수 있도록 함.

그리고 다음과 같이 프로그램 링크 시에 발생하는 로그를 보여주는 함수를 구현해서 대신 갈아 끼웠다.

``` c++
void PrintShaderProgramErrorLog(GLuint program_id) {
  GLint max_length = 0;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);

  std::vector<GLchar> info_log(max_length);
  glGetProgramInfoLog(program_id, max_length, &max_length, &info_log[0]);
  PUSH_LOG_CRITICAL_EXT("Failed to compile shader. Log : {}", &info_log[0]);
}
```

``` c++
  GLint is_linked = 0;
  glGetProgramiv(m_program_id, GL_LINK_STATUS, &is_linked);
  if (!is_linked) {
    PrintShaderProgramErrorLog(m_program_id);
  }
```

일단 고치고 나니 에러로그가 사라졌다.

#### 2018-08-11 PM 09:01

위 3 개를 구현해서 안전한 쉐이더 기능을 구현한다.