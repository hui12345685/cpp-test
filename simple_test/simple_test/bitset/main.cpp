#include <iostream>
#include "bitmap.h"

int main() {
  Bitmap bitmap(100000); // ����һ�� 100 bit �� Bitmap

  bitmap.set(10);     // ���õ� 10 λΪ 1
  bitmap.set(20);     // ���õ� 20 λΪ 1
  bitmap.set(30);     // ���õ� 30 λΪ 1


  std::cout << "Test bit 10: " << bitmap.test(10) << std::endl; // 1 (true)
  std::cout << "Test bit 15: " << bitmap.test(15) << std::endl; // 0 (false)

  bitmap.reset(20);     // ���õ� 20 λΪ 0
  std::cout << "Test bit 20 after reset: " << bitmap.test(20) << std::endl; // 0 (false)

  bitmap.set(99999);     // ���õ� 99999 λΪ 1������ɹ����ٽ�ֵ��������
  std::cout << "Test bit 99999: " << bitmap.test(99999) << std::endl; // 0 (true)

  bitmap.set(100000);     // ���õ� 100000 λΪ 1������ɹ����ٽ�ֵ��������
  std::cout << "Test bit 100000: " << bitmap.test(100000) << std::endl; // 0 (false)

  bitmap.set(0);     // ���õ� 0 λΪ 1
  std::cout << "Test bit 0: " << bitmap.test(0) << std::endl; // 0 (true)

  return 0;
}
