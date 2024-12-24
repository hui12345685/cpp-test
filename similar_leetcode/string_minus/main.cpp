#include <iostream>

/*
给两个字符串a和b，求按照字典序比较，在a和b之间的字符串的数量。
例：字符串a为“az”，字符串b为“bb”，他们之间的字符串只有“ba”，输出为1.思路：类似26进制减法。
*/

// 本质就是实现26进制的减法
// 如果a大于b，则他们之间不能有字符串，返回0
// 如果位数相同，则直接从后忘前面减
// 如果位数不同：
//  1: 当a的长度小于b的长度时，如"a"，"bc",需要在a前面补齐'a'-1表示0，让ab长度相等
//  2: 当a的长度大于b的长度时，如"abc","z",这时候从减法来看"z"是小于"abc"的，直接返回0即可

int StringMinus(std::string a, std::string b) {
  if (b.size() < a.size()) {
    return 0;
  }

  char add = 'a' - 1;
  std::string add_str(1, add);

  int diff_len = b.size() - a.size();
  while (diff_len-- > 0) {
    a = add_str + a;
  }

  int result = 0;
  int jiewei = 0;
  for (int i = a.size() - 1; i >= 0; i--) {
    int diff = 0;
    if ((b[i] - jiewei) < a[i]) {
      diff = b[i] + 26 - a[i] - jiewei - 1; //求间隔，所以要多减个1
      jiewei++;
    } else if ((b[i] - jiewei) == a[i]) {
      // 相等要特殊处理下
      diff = 0;
      jiewei = 0;
    } else {
      diff = b[i] - a[i] - jiewei;
      jiewei = 0; // 复位
    }
    result += diff * pow(26, a.size() - 1 - i);
  }
  return result;
}

int main() {
  int res = StringMinus("az", "bb");
  std::cout << res << std::endl; // 1;

  res = StringMinus("azc", "bb");
  std::cout << res << std::endl; // 0;

  res = StringMinus("z", "bb"); //ax开头的26个加上ba，一共27个
  std::cout << res << std::endl; // 27;
}
