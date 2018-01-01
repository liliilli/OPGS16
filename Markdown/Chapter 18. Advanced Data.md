# Chapter 18. Advanced Data

OpenGL 은 버퍼를 조작할 수 있는 여러가지 방법들이 함수로 내장되어 있다. 또한 텍스쳐를 통해서 많은 양의 데이터를 쉐이더로 전달할 수 있는 방법이 존재한다.

OpenGL 의 버퍼는 사실, 거창한 것은 아니고 그저 **메모리 공간을 관리하는 객체**에 불과하다. 이 버퍼는 단지 값을 저장하고만 있기 때문에 이 버퍼를 쓸려면 **Buffer target** 을 지정해줘서 어디에다가 쓰겠다라고 명시적으로 해야한다.

`glBufferData` 는 버퍼를 만들어서 값을 관리하고자 할 때 쓰인다. 첫 인자가 Target 이기 때문에 동시에 용도도 지정한다. 그런데 이 값에 `nullptr` 도 지정하는게 가능한데, 그러면 이 버퍼는 메모리를 할당만 하고 나중에 값을 채울 수 있게 된다.

``` c++
glBufferData(GL_ARRAY_BUFFER, sizeof(720), nullptr, GL_STATIC_DRAW);
```

또는 `glBufferSubData` 을 통해서 버퍼의 일부 부분을 갱신할 수도 있다. 그러나 부분을 채우는데는 버퍼가 미리 할당되어 있어야 하며, 또한 버퍼의 사이즈를 넘을 수는 없다.

``` c++
glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data); 
// Range: [24, 24 + sizeof(data)]
```

또는 약간 원시적인 방법으로, `glMapBuffer` 로 버퍼의 값의 포인터를 받아서 **직접** 저수준의 메모리 복사를 할 수도 있다. 이 방법은 나중에 메모리 복사를 끝냈을 떄 `glUnmapBuffer`을 써서 포인터를 해제시키고 제대로 맵핑이 되었는가 확인해야 한다.

``` c++
float data[] = {
  0.5f, 1.0f, -0.35f
  ...
};
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// get pointer
void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
// now copy data into memory
memcpy(ptr, data, sizeof(data));
// make sure to tell OpenGL we're done with the pointer
glUnmapBuffer(GL_ARRAY_BUFFER);
```

## Batching vertex attributes

