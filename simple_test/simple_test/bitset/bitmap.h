#pragma once


#include <vector>
#include <cstdint>

class Bitmap {
public:
  // 最大的数字为size-1
  explicit Bitmap(size_t size) : size_(size) {
    // 计算所需整数个数：向上取整
    array_size_ = (size + 31) / 32;
    bits_.resize(array_size_, 0);
  }

  // 设置指定位为 1
  void set(size_t pos) {
    if (pos >= size_) return;
    size_t index = pos / 32;      // 确定数组下标
    size_t offset = pos % 32;     // 确定比特偏移
    bits_[index] |= (1U << offset);
  }

  // 设置指定位为 0
  void reset(size_t pos) {
    if (pos >= size_) return;
    size_t index = pos / 32;
    size_t offset = pos % 32;
    bits_[index] &= ~(1U << offset);
  }

  // 检查指定位是否为 1
  bool test(size_t pos) const {
    if (pos >= size_) return false;
    size_t index = pos / 32;
    size_t offset = pos % 32;
    return (bits_[index] & (1U << offset)) != 0;
  }

private:
  std::vector<uint32_t> bits_;
  size_t size_;       // 总比特位数
  size_t array_size_; // 底层数组大小
};
