# Chapter 5. Transformations

잠깐 3 차원 Vector 공간 상에서의 여러가지 접근 방법에 대해서 알아보자.

## Scaling & Translation

우선 2차원 평면에 $$ \overline{v} = (3, 2) $$ 가 있다고 하면, 이 좌표를 $$ x $$ 축으로는 0.5 배, 그리고 $$ y $$ 축으로는 2 배 늘릴 것이다. 그러면 결과 값 $$ \overline{s} $$ 는 $$ \overline{s} = (1.5, 4) $$ 일 것이다.

![img](https://learnopengl.com/img/getting-started/vectors_scale.png)

* OpenGL 은 2차원 혹은 1차원 좌표의 변형에 대해서도 **3차원 공간 상**에서 좌표 변환을 한 후에 최종 값을 반환한다. 
* 각 축에 대해서 다른 값으로 변형을 하는 것을 **non-uniform** scale 이라고 한다.
  각 축에 대해서 같은 인자 값 변형은 **uniform** scale 이라고 한다.

만약에 3차원 공간 좌표에서 각 축에 대해서 각각의 값으로 변형을 하고 싶다면 다음과 같은 계산식을 가지게 된다.
$$
\begin{bmatrix}
\mathbf{S}_1 & 0 & 0 & 0\\
0 & \mathbf{S}_1 & 0 & 0\\
0 & 0 & \mathbf{S}_1 & 0\\
0 & 0 & 0 & \mathbf{S}_1
\end{bmatrix}  \cdot 
\begin{pmatrix}
x \\ y \\ z \\ 1
\end{pmatrix}
=
\begin{pmatrix}
\mathbf{S}_1 \cdot x \\
\mathbf{S}_2 \cdot y \\
\mathbf{S}_3 \cdot z \\
1
\end{pmatrix}
$$

* 여기서 4번째 값인 $$ 1 $$ 은 `w` 축 값이라고 부른다. 
  이하와 같이 위치의 이동 (`translation`) 에 유용하게 쓰일 수 있다.

$$
\begin{bmatrix}
\mathbf{S}_1 & 0 & 0 & \mathbf{T}_x \\
0 & \mathbf{S}_1 & 0 & \mathbf{T}_y \\
0 & 0 & \mathbf{S}_1 & \mathbf{T}_z \\
0 & 0 & 0 & \mathbf{S}_1
\end{bmatrix}  \cdot 
\begin{pmatrix}
x \\ y \\ z \\ 1
\end{pmatrix}
=
\begin{pmatrix}
\mathbf{S}_1 \cdot x + \mathbf{T}_x \\
\mathbf{S}_2 \cdot y + \mathbf{T}_y \\
\mathbf{S}_3 \cdot z + \mathbf{T}_z \\
1
\end{pmatrix}
$$

### Homogeneous coordinates (동차 좌표계)

원점을 지나는 n 차원의 축에 대한 좌표를 가질 수 있는 직선들을 모은 *사영 공간* 을, n + 1 개의 좌표로 나타내는 좌표계이다. 위에서도 알 수 있듯이, 3차원 공간 좌표에 w 축이 들어간 좌표계 역시 *동차 좌표계*라고 말할 수 있다. 그리고 $ w $ 축을 동차 좌표축 이라고 말한다.

3D 좌표를 가지는 벡터를 얻기 위해서는, $$ x $$ , $$ y $$ , $$ z $$ 축을 $$ w $$ 축의 값으로 나눠야 한다. 대개 $$ w $$ 축의 값은 1.0 이기 때문에 보통은 알아차리지 못한다. *homogeneous coordinates* 을 씀으로써 얻을 수 있는 이점은 다음과 같다.

1. 3D vector 에 대해 `Translation` (평행 이동) 이 가능해진다.
2. $$ w $$ 값을 이용해 3D visuals 을 만들 수 있다.
3. 동차 좌표축의 값이 0 이면, `direction vector` 이다.

## Rotation

일반 2차원 좌표계에 대한 *시계 방향* 으로 회전하는 회전 매트릭스는 다음과 같다.
$$
\begin{bmatrix}
\cos\theta & -\sin\theta \\
\sin\theta & \cos\theta
\end{bmatrix}
$$
이것을 위의 동차 좌표계에 적용하면 다음과 같을 것이다.

1. $ x $ 축에 대해서 시계 방향으로 회전 
   ( $$ z $$ 축도 매트릭스를 그냥 통째로 옮겨서 적용하기만 하면 된다. )

$$
\begin{bmatrix}
1 & 0 & 0 & 0\\
0 & \cos\theta & -\sin\theta & 0\\
0 & \sin\theta & \cos\theta & 0\\
0 & 0 & 0 & 1
\end{bmatrix}  \cdot 
\begin{pmatrix}
x \\ y \\ z \\ 1
\end{pmatrix}
=
\begin{pmatrix}
x \\
\cos\theta \cdot y - \sin\theta \cdot z \\
\sin\theta \cdot z + \cos\theta \cdot y \\
1
\end{pmatrix}
$$

2. $$ y $$ 축에 대해서 시계 방향으로 회전

$$
\begin{bmatrix}
 \cos\theta & 0 & -\sin\theta & 0\\
0 & 1 & 0 & 0\\
\sin\theta & 0 & \cos\theta & 0\\
0 & 0 & 0 & 1
\end{bmatrix}  \cdot 
\begin{pmatrix}
x \\ y \\ z \\ 1
\end{pmatrix}
=
\begin{pmatrix}
\cos\theta \cdot x - \sin\theta \cdot z \\
y \\
\sin\theta \cdot z + \cos\theta \cdot x \\
1
\end{pmatrix}
$$

Euler 좌표계로 회전을 하는 것은 간단하지만 다른 문제가 발생할 수 있다.

### Gimbal Lock

3차원 공간에서 회전을 할 때, $ x $ 혹은 $ y $ , $ z $ 축 중 한 축이 다른 축에 겹쳐버리게 될 때 회전을 하기 위해 *의도치 않은 회전 궤적* 과 *전혀 관련되지 않은 다른 축이 종속되어 움직이게 되버리는* 현상을 말한다.

이 현상을 해결하기 위해서는 **Quaternion** 을 쓰는 것이 가장 좋다. (Quaternion 은 나중에 보자)

## In practice

### GLM

* OpenGL Mathematics 의 약자로, OpenGL 을 위한 Matrix 연산을 쉽게 도와주는 라이브러리이다.



