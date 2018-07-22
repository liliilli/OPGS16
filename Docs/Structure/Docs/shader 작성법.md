# OPGS16 GLSL Shader 작성법

`OPGS16` 은 각종 다양한 빌트인 `built-in` 변수를 사용하고 있다. `OPGS16` 에서 직접 작성한 쉐이더를 문제없이 쓰기 위해서는 다음 변수들이 미리 쓰인다는 것을 알아야 한다.

| 변수 이름     | 변수 타입 | 설명                                                         |
| ------------- | --------- | ------------------------------------------------------------ |
| opProj        | mat4      | 투영 행렬 $$ P $$                                            |
| opView        | mat4      | 뷰 행렬 $$ V $$                                              |
| opProjView    | mat4      | 투영 행렬 $$ P $$ 와 뷰 행렬 $$ V $$ 을 곱한 행렬            |
| opModel       | mat4      | 현재 렌더러에 바인딩된 오브젝트의 모델 행렬 $$ M $$          |
| uTexelLD (op) | vec2      | $$ (u, v) $$ 로 Atlas 가 있는 텍스쳐의 일부 텍스쳐의 시작 지점 |
| uTexelRU (op) | vec2      | $$ (u, v) $$ 로 Atlas 가 있는 텍스쳐의 일부 텍스쳐의 끝 지점 |
| opPosition    | vec3      | VAO 속성 id 가 0 인, 각 정점을 나타내는 $$(x, y, z) $$ 좌표  |
| opNormal      | vec3      | VAO 속성 id 가 1 이며 정점의 법선 좌표 $$ (x, y, z) $$       |
| opTangent     | vec3      | VAO 속성 id 가 2 이며 정점 탄젠트 공간의 좌표 $$(x, y, z) $$ |
| opTexCoord    | vec2      | VAO 속성 id 가 3 이며 해당 정점의 텍셀 좌표 $$(u, v) $$      |
| opBoneId      | vec3      | VAO 속성 id 가 4 이며 메쉬의 본 ID                           |
| opWeight      | vec3      | VAO 속성 id 가 5 이며 메쉬의 그거.                           |
| opScale       | vec2      | 텍스쳐의 기본 스케일 값                                      |
| opOffset      | vec2      | 텍스쳐의 기본 오프셋 값                                      |
| opAlpha       | float     | 투명도 값                                                    |

또한 `OpenGL` 의 버전은 4.3 을 사용하고 있다.

`OPGS16` 은 정점 쉐이더 `Vertex Shader` 와 픽셀 쉐이더 `Fragment Shader` 만 사용 가능하며, `Geometry Shader` `TCS` `TES` 는 컴포넌트 상에서 렌더링 지원을 하지 않고 있다.

---

## Vertex Shader 예시

``` glsl
#version 430 core

layout (location = 0) in vec3 opPosition;

out VS_OUT {
	vec2 texCoord;
} vs_out;

uniform mat4	opProj;

uniform int		uNumber;
uniform int		uPos[64];
uniform float   uYScale;

void main() {
	const float width   = 178.f;
	const float x_width = 1.0f / uNumber;

	mat4 model;
	model[0][0]			= width / 2; model[1][1] = uYScale;
	float world_x		= width / (uNumber << 1) * ((gl_InstanceID << 1) + 1);
	model[3]			= vec4(39 + world_x, 112 + uPos[gl_InstanceID], 0, 1);

	const vec2 local	= vec2(opPosition.x / uNumber, opPosition.y);
	gl_Position			= opProj * model * vec4(local, 0, 1);

	const vec2 offset	= ((opPosition.xy + 1) / 2) * vec2(x_width, 1);
	const vec2 texel	= vec2(x_width * gl_InstanceID, 0) + offset;
	vs_out.texCoord		= texel;
}
```

## Fragment Shader 예시

``` glsl
#version 430 core

out vec4 color;
in VS_OUT { vec2 texCoord; } fs_in;

uniform vec2 uTexelLD;
uniform vec2 uTexelRU;

layout (binding = 0) uniform sampler2D uTexture1;
uniform float opAlpha;

void main() {
	const vec4 alpha_vec = vec4(1, 1, 1, opAlpha);
	color = texture(uTexture1, uTexelLD + (fs_in.texCoord * (uTexelRU - uTexelLD))) * alpha_vec;
}
```

