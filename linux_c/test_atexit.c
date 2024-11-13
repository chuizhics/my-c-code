#include <stdlib.h>
#include <stdio.h>

void free_memory(void* ptr) {
  // 释放动态分配的内存
  free(ptr);
}

int main() {
  // 动态分配内存
  int* ptr = (int*)malloc(sizeof(int));

  // 注册释放内存的函数
  // F atexit((void (*)(void))free_memory, ptr);

  // ... 程序其余的代码 ...
  free(ptr);

  return 0;
}
