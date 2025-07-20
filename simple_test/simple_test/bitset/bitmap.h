#pragma once


#include <vector>
#include <cstdint>

class Bitmap {
public:
  // ��������Ϊsize-1
  explicit Bitmap(size_t size) : size_(size) {
    // ����������������������ȡ��
    array_size_ = (size + 31) / 32;
    bits_.resize(array_size_, 0);
  }

  // ����ָ��λΪ 1
  void set(size_t pos) {
    if (pos >= size_) return;
    size_t index = pos / 32;      // ȷ�������±�
    size_t offset = pos % 32;     // ȷ������ƫ��
    bits_[index] |= (1U << offset);
  }

  // ����ָ��λΪ 0
  void reset(size_t pos) {
    if (pos >= size_) return;
    size_t index = pos / 32;
    size_t offset = pos % 32;
    bits_[index] &= ~(1U << offset);
  }

  // ���ָ��λ�Ƿ�Ϊ 1
  bool test(size_t pos) const {
    if (pos >= size_) return false;
    size_t index = pos / 32;
    size_t offset = pos % 32;
    return (bits_[index] & (1U << offset)) != 0;
  }

private:
  std::vector<uint32_t> bits_;
  size_t size_;       // �ܱ���λ��
  size_t array_size_; // �ײ������С
};
