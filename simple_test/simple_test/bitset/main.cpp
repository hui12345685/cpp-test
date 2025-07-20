#include <iostream>
#include "bitmap.h"

int main() {
  Bitmap bitmap(100000); // 创建一个 100 bit 的 Bitmap

  bitmap.set(10);     // 设置第 10 位为 1
  bitmap.set(20);     // 设置第 20 位为 1
  bitmap.set(30);     // 设置第 30 位为 1


  std::cout << "Test bit 10: " << bitmap.test(10) << std::endl; // 1 (true)
  std::cout << "Test bit 15: " << bitmap.test(15) << std::endl; // 0 (false)

  bitmap.reset(20);     // 设置第 20 位为 0
  std::cout << "Test bit 20 after reset: " << bitmap.test(20) << std::endl; // 0 (false)

  bitmap.set(99999);     // 设置第 99999 位为 1，不会成功，临界值超过容量
  std::cout << "Test bit 99999: " << bitmap.test(99999) << std::endl; // 0 (true)

  bitmap.set(100000);     // 设置第 100000 位为 1，不会成功，临界值超过容量
  std::cout << "Test bit 100000: " << bitmap.test(100000) << std::endl; // 0 (false)

  bitmap.set(0);     // 设置第 0 位为 1
  std::cout << "Test bit 0: " << bitmap.test(0) << std::endl; // 0 (true)

  return 0;
}
