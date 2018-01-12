# Chapter 4 수학

### 반사와 굴절

반사와 굴절을 구할 때, 들어가는 방향의 벡터가 $$ R_{in} $$ 이며, 반사되는 방향의 벡터가 $$ R_{reflect} $$, 표면의 법선 벡터가 $$ N $$ 이라고 하자.

* 반사 공식은
  $$
  \hat{R_{in}} = \frac{R_{in} \cdot N}{N \cdot N}N \\
  R_{reflect} = R_{in} - 2\hat{R_{in}} \\
  Z = R_{in} - \hat{R_{in}} \\
  R_{reflect} = Z - \hat{R_{in}} 
  $$
  이다.

* 굴절 공식은 주어진 $ \eta $ 가 있다고 할 때,
  $$
   k = 1 - \eta^2(1 - \hat{R_{in}}^2) \\
   R_{refract} = \eta(Z - \sqrt{k}N)
  $$
  이다. $ \eta $ 가 높아지면, 표면으로의 굴절 정도는 심해진다. (스넬의 법칙에서 굴절의 계수가 높아지면 망하는 거랑 같다.)

### 오일러 각

* Euler Angle 은 3차원 공간 좌표 상에서 회전을 나타내는 세 각도의 집합이다.
* 오일러 각은, 한 축이 다른 한 축에 겹쳐지면, 두 축의 회전이 동일한 변환이 되기 때문에 다른 한 축 좌표계의 자유도가 줄어버리는 **짐벌 락** 문제를 안는다.
  * **쿼터니언** 방식으로 해결이 가능하다.

### 쿼터니언

* 쿼터니언은 **하나의 실수부, 세개의 허수부** 로 존재한다. ($$ i, j, k $$)
* 쿼터니언의 허수부는 복소수의 허수부와 거의 유사한 속성을 갖는다.
  * $ i^2 = j^2 = k^2 = ijk = -1 $
  * $ i = jk \\ k = ij \\ j = ik $
* 이를 활용해서 오일러 각도 에서의 **짐벌 락**을 해결할 수 있다.
* $ q = x + yi + zj + wk \\ q = (r, \overrightarrow{v}) $ 
  이다.
* 쿼터니언은 스칼라를 제외한 곱셈의 교환 법칙이 성립하지 않는다.

> Real time rendering 발췌...
>
> 쿼터니언은 오일러 각처럼 회전을 할 때 쓰이는 사원수 각이다. 
> 쿼터니언 각은 다음과 같이 나타낼 수 있다.
> $$
> \hat{\mathbf{q}} = (\mathbf{q}_{v}, \mathbf{q}_{w}) \\
> \mathbf{q}_{v} = i\mathbf{q}_x + j\mathbf{q}_y + k\mathbf{q}_z
> $$
> 이 때, 오일러 각으로 적용하면 다음과 같다.
> $$
> \hat{\mathbf{q}} = (\sin{\phi\mathbf{u}_q} \cos{\phi}) = \sin{\phi\mathbf{u}_q} + \cos{\phi} \\
> ||\hat{\mathbf{q}}|| = n(\hat{\mathbf{q}}) = \sqrt{\sin^2{\phi(\mathbf{u}_q\cdot\mathbf{u}_q)} + \cos^2{\phi}} = 1 \\
> \text{because } ||\mathbf{u}_q|| = 1 \text{ (unit quaternion)} \\
> \text{ } \\
> \hat{\mathbf{q}} = e^{\phi\mathbf{u}_q}
> $$
>

* 쿼터니언은 회전의 성분을 가지고 있기 때문에, $$ \hat{\mathbf{q}}^-1 = \hat{\mathbf{q}}^{*} $$ 이다.
* 쿼터니언이 만약 $ \phi $ 만큼의 회전각을 가지고 있으면, $$ \hat{\mathbf{q}}\hat{\mathbf{p}}\hat{\mathbf{q}}^{*} $$ 을 적용해 $$ 2\phi $$ 만큼 회전시킬 수 있다. 여기서 $ \hat{\mathbf{p}} $ 는 쿼터니언에서의 좌표 성분이다.

### 모델-뷰 변환

* 뷰 변환을 통해서 모델을 **frustum** 시야 안으로 이동시킬 수 있다. 모델-월드 변환 행렬과 월드-뷰 변환 행렬을 곱해서 모델-뷰 변환을 만들 수 있다.
* 월드가 큰 경우에, 월드 공간에서 수행되는 계산의 정밀도는 월드 원점으로부터 얼마나 멀리 떨어져 있는가에 따라 달라질 수 있지만, 카메라 시점은 여러 위치에 존재할 수 있으므로 신뢰성이 떨어진다. 하지만 뷰 변환을 통해서 동일 계산을 수행하면, **정밀도의 기준이 뷰어 위치** 가 되기 때문에 보이는 화면에서의 연산 신뢰성이 높아진다.

#### 룩앳 Matrix

* 카메라가 바라보는 **방향**과, **위치**와, 월드 공간 상에서의 **위** 방향 벡터가 주어졌을 때, 카메라를 올바르게 보게 하고 월드 공간에서의 원점을 카메라의 중심으로 이동하게 하는 매트릭스이다.

  * 이 행렬로 카메라 중심의 공간 구현이 가능하다.

* 매트릭스는 다음과 같다.
  우선, 방향 벡터 $ f $, 월드 공간에서의 위 방향 $ u $가 있다고 하면, 카메라를 기준으로 하는 옆방향 벡터 $ s $ 는 다음과 같다.
  $$
  s = f \times u
  $$
  그리고 위 방향 $ u $ 가 카메라 대해서는 위 방향이 아닐 수도 있기 때문에 ($s$, $f$ 에 대해 수직이 아닐 수도 있으므로) 카메라 기준 위 방향 벡터 $ u' $ 을 구한다.
  $$
  u' = s \times f
  $$
  그러면 카메라를 회전시킬 수 있는 매트릭스가 구해진다.
  $$
  R = \begin{bmatrix}s.x & u'.x & f.x & 0 \\ s.y & u'.y & f.y & 0 \\ s.z & u'.z & f.z & 0 \\ 0 & 0 & 0 & 1\end{bmatrix}
  $$
  주의해야할 점은, 아직 카메라 위치 $ e $ 가 반영이 되지 않았다는 것이다. 위 역시 아핀 변환을 하는 매트릭스이므로 다음과 같이 매트릭스를 갱신하다. 그러면 카메라 위치가 원점이 된다. (카메라 위치가 원점이 되야 하므로 음의 방향으로 이동한다)
  $$
  R = \begin{bmatrix}s.x & u'.x & f.x & -e.x \\ s.y & u'.y & f.y & -e.y \\ s.z & u'.z & f.z & -e.z \\ 0 & 0 & 0 & 1\end{bmatrix}
  $$

* `glm` 라이브러리에는 `glm::lookAt` 을 이용해 변환 행렬을 쉽게 구할 수 있다.

### 프로젝션 변환

* 프로젝션 변환은 **뷰 볼륨을 정의하고 클리핑 평면**을 정의한다. 클리핑 평면은 OpenGL 이 지오메트리가 보이는가 판단하기 위해서 사용하는 3D 공간 상의 평면이다.

  * 즉, 모든 모델링이 끝난 카메라 뷰 상의 장면이 실제 최종 이미지로 어떻게 투영되는가를 결정한다.

* **원근 행렬 (frustum)**
  버텍스가 뷰 공간 안에 있을 때, 가상의 절두체 모양을 띄는 원근 투영 행렬을 이용해 버텍스가 클리핑 공간 안에 있는가, 아닌가를 확인할 수 있다.
  $$
  \begin{bmatrix} 
  	\frac{2 \cdot near}{right - left} & 0 & \frac{right + left}{right - left} & 0 \\
  	0 & \frac{2 \cdot near}{top - bottom} & \frac{top + bottom}{top - bottom} & 0 \\
  	0 & 0 & \frac{near + far}{near - far} & \frac{2 \cdot near \cdot far}{near - far} \\
  	0 & 0 & -1 & 0
  \end{bmatrix}
  $$
  또는, 시야각 (FOV) 와 종횡비를 이용해서 L,R,T,B 을 대신할 수도 있다.

* **정사영 행렬 (Orthogonal)**
  정사영 행렬은 뷰 공간을 클립 공간으로 단순히 맵핑하는 행렬이다.
  $$
  \begin{bmatrix}
  	\frac{2}{right - left} & 0 & 0 & \frac{right + left}{right - left} \\
  	0 & \frac{2}{top - bottom} & 0 & \frac{top + bottom}{top - bottom} \\
  	0 & 0 & \frac{2}{far - near} & \frac{far + near}{far - near} \\
  	0 & 0 & 0 & 1
  \end{bmatrix}
  $$


### Interpolation

* **보간**은, 알려진 점들의 특정 비율의 중간점을 찾는 작업을 지칭하는 용어이다. 점 A 에서 점 B 로 갈 때, $ p $ 점을 찾고 싶다고 하자. 이 때 방향 벡터 $ \overrightarrow{D} = (B - A) $ 이다.
  $$
  p = A \times t\overrightarrow{D} \\
  \vec{D} = B - A \\
  p = A + t(B - A) \\
  \therefore p = (1- t)A + tB 
  $$
  임을 알 수 있다. 이 때 A 와 B 는 어느 차원의 공간 상에서 선을 이루고 있기 때문에 $ p $ 을 찾는 작업을 **선형 보간** 이라고 한다.

* GLSL 에서는 선형 보간을 `mix` 함수로 제공하고 있다.

#### 곡선 (베지어 곡선)

* 위에서 언급한 선형 보간을 이용해서 곡선을 그린다.

* 대부분의 곡선은, 세 개 이상의 **Control Point(제어점)** 을 가진다. 두 개는 시작 점과 끝 점이고, 하나 이상은 선형 보정을 하는데 필요한 경유점이다. 

* 2차 베지어 곡선 ( 컨트롤 포인트에서 두 개는 시작 끝점이고, 하나는 경유점 ) 은 다음과 같이 GLSL 로 구현이 가능하다.

  ``` c++
  vec4 quadratic_bezier(vec4 A, vec4 B, vec4 C, float t) {
    vec4 D = mix(A, B, t); // D = (1 - t)A + B
    vec4 E = mix(B, C, t);
    vec4 P = mix(D, E, t);
    
    return P;
  }
  ```

* n차 베지어 곡선은 n - 2 개의 점이 경유점이고, $$ \sum^{n}{k} - 1 $$ 개의 최종 경유점을 가진다.

#### 스플라인

* 베지어 등으로 만들어진 곡선이 **연결된** 긴 곡선이다.
* 작은 곡선은 지역적인 모양을 정의한다. 이 작은 곡선과 곡선을 연결해주는 제어점을 **Weld** 라고 한다. 

![cubic bezier spline에 대한 이미지 검색결과](http://what-when-how.com/wp-content/uploads/2012/06/tmpc009320_thumb22.png)

* 위 그림은 각 곡선이 3차 베지어 곡선으로 이루어져 있기 때문에, **3차 베지어 스플라인** 혹은 **3차 B-spline (cubic B-spline)** 이라고 한다.

* 위 스플라인을 구현하는데는, 기존에 썼던 베지어 곡선을 캡슐화해서, 진행도 $ t $ 에 따른 분기가 필요하다.

  ``` c++
  vec4 cubic_bspline_7(vec4 CP[7], float t) {
    float f = t * 2.0;
    int i = int(floor(f));
    float s = fract(t);
    
    if (t <= 0) return CP[0];
    if (t >= 1) return CP[6];
    
    vec4 A, B, C, D = CP[i*3], CP[i*3+1], CP[i*3+2], CP[i*3+3];
    return cubic_bazier(A, B, C, D, s);
  }
  ```

* 보간되서 나오는 $ p $ 의 변화율 (속도) 는 $ t $ 에 관한 곡선의 **미분 방정식**이다. 즉, 불연속적이라면 방향을 갑자기 바꿀 것이다. 연속적인 1차 미분을 갖는 스플라인 함수를 $$ \mathbf{C}^1 $$ **연속적** 이라고 한다.

* $ p $ 의 가속도는 곡선의 **2차 미분 방정식**이다. 불연속적이라면 급정지를 하거나 급발진을 할 가능성이 높다.  연속적인 2차 미분을 가지면 $$ \mathbf{C}^2 $$ 연속적이다. 스플라인 곡선은 $$ \mathbf{C}^1, \mathbf{C}^2 $$ 연속적이다.

* 다만 스플라인 곡선이 컨트롤 포인트에서 부드러운 속도 및 가속도를 보이기 위해서는, 각 곡선을 제어하는 제어점과 그 제어점 다음에 오는 **Weld** 에 뒤 따라오는 다음 곡선의 제어점이 선형을 이루며, 거리 역시 같아야 한다.

  * 이렇게 되면, Weld 을 사이에 두는 제어점의 좌표는, Weld 위치에서 속도를 빼거나 더한 벡터값이다.
  * 이 3차 B-spline 을 **Cubie Hermite Spline** 이라고 한다.