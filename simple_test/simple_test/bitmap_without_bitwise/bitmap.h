#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <cstdint>  // 包含uint32_t定义
#include <string>

class UnsignedBitmap32 {
public:
  explicit UnsignedBitmap32(size_t size)
    : size_(size),
    blocks_((size + 31) / 32, 0) {}  // 使用uint32_t存储

  // 设置位（置1）
  void set(size_t pos) {
    check_bounds(pos);
    auto [block_idx, bit_idx] = position_to_indices(pos);

    // 计算当前位值
    uint32_t current_value = get_bit_value(block_idx, bit_idx);

    // 只有当前为0时才需要更新
    if (current_value == 0) {
      // 计算需要增加的值：2^(bit_idx)
      uint32_t add_value = power_of_two(bit_idx);
      blocks_[block_idx] += add_value;
    }
  }

  // 清除位（置0）
  void reset(size_t pos) {
    check_bounds(pos);
    auto [block_idx, bit_idx] = position_to_indices(pos);

    // 计算当前位值
    uint32_t current_value = get_bit_value(block_idx, bit_idx);

    // 只有当前为1时才需要更新
    if (current_value == 1) {
      // 计算需要减少的值：2^(bit_idx)
      uint32_t subtract_value = power_of_two(bit_idx);
      // 安全减法（避免下溢）
      if (blocks_[block_idx] >= subtract_value) {
        blocks_[block_idx] -= subtract_value;
      }
      else {
        // 理论上不应发生，但安全处理
        blocks_[block_idx] = 0;
      }
    }
  }

  // 测试位值
  bool test(size_t pos) const {
    check_bounds(pos);
    auto [block_idx, bit_idx] = position_to_indices(pos);
    return get_bit_value(block_idx, bit_idx) == 1;
  }

  // 打印位图状态（调试用）
  void print() const {
    std::cout << "Bitmap (size: " << size_ << " bits)\n";
    std::cout << "Block count: " << blocks_.size() << "\n";

    for (size_t i = 0; i < blocks_.size(); ++i) {
      std::cout << "Block " << i << " uint32 " << blocks_[i] << std::endl;
      // std::hex 设置2进制输出，std::dec恢复十进制输出
      std::cout << "Block " << i << ": hex 0x"
        << std::hex << std::setw(8) << std::setfill('0')
        << blocks_[i] << std::dec << " = ";

      // 打印块内的位（0-31）
      for (int j = 31; j >= 0; --j) {
        if (j % 8 == 7) std::cout << " ";
        std::cout << ((blocks_[i] / power_of_two(j)) % 2);
      }
      std::cout << "\n";
    }

    // 打印整体位图
    std::cout << "\nAll bits:\n";
    for (size_t i = 0; i < size_; ++i) {
      if (i % 32 == 0) {
        std::cout << "\n" << i << "-" << std::min(i + 31, size_ - 1) << ": ";
      }
      else if (i % 8 == 0) {
        std::cout << " ";
      }
      std::cout << (test(i) ? '1' : '0');
    }
    std::cout << "\n";
  }

private:
  // 检查位置是否越界
  void check_bounds(size_t pos) const {
    if (pos >= size_) {
      throw std::out_of_range("Position " + std::to_string(pos) +
        " out of range [0, " +
        std::to_string(size_ - 1) + "]");
    }
  }

  // 将位置转换为块索引和块内位索引
  std::pair<size_t, size_t> position_to_indices(size_t pos) const {
    size_t block_idx = pos / 32;
    size_t bit_idx = pos % 32;
    return { block_idx, bit_idx };
  }

  // 获取特定位的值（使用除法和取模）
  uint32_t get_bit_value(size_t block_idx, size_t bit_idx) const {
    uint32_t block_value = blocks_[block_idx];
    uint32_t divisor = power_of_two(bit_idx);
    return (block_value / divisor) % 2;
  }

  // 计算2的幂（0-31）
  uint32_t power_of_two(size_t exponent) const {
    // 预计算的2^0到2^31（无符号）
    static const uint32_t powers[32] = {
        1,              // 2^0
        2,              // 2^1
        4,              // 2^2
        8,              // 2^3
        16,             // 2^4
        32,             // 2^5
        64,             // 2^6
        128,            // 2^7
        256,            // 2^8
        512,            // 2^9
        1024,           // 2^10
        2048,           // 2^11
        4096,           // 2^12
        8192,           // 2^13
        16384,          // 2^14
        32768,          // 2^15
        65536,          // 2^16
        131072,         // 2^17
        262144,         // 2^18
        524288,         // 2^19
        1048576,        // 2^20
        2097152,        // 2^21
        4194304,        // 2^22
        8388608,        // 2^23
        16777216,       // 2^24
        33554432,       // 2^25
        67108864,       // 2^26
        134217728,      // 2^27
        268435456,      // 2^28
        536870912,      // 2^29
        1073741824,     // 2^30
        2147483648      // 2^31（无符号）
    };

    if (exponent < 32) {
      return powers[exponent];
    }

    throw std::out_of_range("Exponent must be less than 32");
  }

private:
  size_t size_;                 // 位图总大小（位数）
  std::vector<uint32_t> blocks_; // 32位无符号整数存储块
};

