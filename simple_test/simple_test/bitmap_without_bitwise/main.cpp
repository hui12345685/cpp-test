#include <iostream>
#include "bitmap.h"

void test_bitmap2(const std::vector<std::string>& data);

// ²âÊÔº¯Êı
void test_bitmap() {
  UnsignedBitmap32 bm(512);

  std::cout << "Initial state:\n";
  bm.print();

  std::cout << "\nSetting bits 31£¬3, 50, 70, 330:\n";
  bm.set(31);
  bm.set(3);
  bm.set(50);
  bm.set(70);
  bm.set(330);
  bm.print();

  std::cout << "\nTesting bits:\n";
  std::cout << "Bit 31: " << bm.test(31) << " (expected: 1)\n";
  std::cout << "Bit 3: " << bm.test(3) << " (expected: 1)\n";
  std::cout << "Bit 4: " << bm.test(4) << " (expected: 0)\n";
  std::cout << "Bit 330: " << bm.test(330) << " (expected: 1)\n";
  std::cout << "Bit 30: " << bm.test(30) << " (expected: 0)\n";

  std::cout << "\nResetting bits 50 and 330:\n";
  bm.reset(50);
  bm.reset(330);
  bm.print();

  std::cout << "\nTesting after reset:\n";
  std::cout << "Bit 50: " << bm.test(50) << " (expected: 0)\n";
  std::cout << "Bit 330: " << bm.test(330) << " (expected: 0)\n";

  std::cout << "\nSetting bit 511:\n";
  bm.set(511);
  bm.print();

  std::cout << "\nTesting bits:\n";
  std::cout << "Bit 511: " << bm.test(511) << " (expected: 1)\n";

  try {
    std::cout << "\nTesting out-of-range access (512): ";
    bm.test(512);
    std::cout << "Failed to throw exception!\n";
  }
  catch (const std::out_of_range& e) {
    std::cout << "Successfully caught exception: " << e.what() << "\n";
  }

  std::vector<std::string> data = bm.GetBitIntString();
  std::cout << "\nbit map data:\n";
  std::cout << "[";
  for (int id = 0; id < data.size(); id++) {
    std::cout << data[id] << " ";
  }
  std::cout << "]\n";
  test_bitmap2(data);
}

void test_bitmap2(const std::vector<std::string>& data) {
  UnsignedBitmap32 bm(data);

  std::cout << "\ntest_bitmap2 Testing bits:\n";
  std::cout << "Bit 31: " << bm.test(31) << " (expected: 1)\n";
  std::cout << "Bit 3: " << bm.test(3) << " (expected: 1)\n";
  std::cout << "Bit 4: " << bm.test(4) << " (expected: 0)\n";
  std::cout << "Bit 30: " << bm.test(30) << " (expected: 0)\n";
  std::cout << "Bit 50: " << bm.test(50) << " (expected: 0)\n";
  std::cout << "Bit 330: " << bm.test(330) << " (expected: 0)\n";
  std::cout << "Bit 511: " << bm.test(511) << " (expected: 1)\n";
}

int main() {
  test_bitmap();
  return 0;
}