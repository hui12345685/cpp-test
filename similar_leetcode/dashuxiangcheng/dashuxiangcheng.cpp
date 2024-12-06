#include <string>
#include <iostream>
#include <vector>

//大数相加
std::string addStrings(const std::string& num1, const std::string& num2) {
  //num1 和num2 都只包含数字 0-9,所以不在额外check，否则需要check下
  std::string ret_reverse;
  int len1 = num1.size();
  int len2 = num2.size();
  int jingwei = 0;
  int idx = len1 - 1;
  int jdx = len2 - 1;
  while (idx >= 0 || jdx >= 0) {
    int tmp = 0;
    if (idx >= 0 && jdx >= 0) {
      tmp = (num1[idx] - '0') + (num2[jdx] - '0') + jingwei;
    }
    else if (idx >= 0 && jdx < 0) {
      //ascii码9+1=:，不是10，所以要转为int来操作
      tmp = num1[idx] - '0' + jingwei;
    }
    else if (idx < 0 && jdx >= 0) {
      //ascii码9+1=:，不是10，所以要转为int来操作
      tmp = num2[jdx] - '0' + jingwei;
    }
    //std::cout << ".........." << tmp << std::endl;

    if (tmp >= 10) {
      tmp -= 10;
      ret_reverse += '0' + tmp; //需要减去10之后再加
      jingwei = 1; //给下次循环用，需要放到+后面
    }
    else {
      ret_reverse += '0' + tmp;
      jingwei = 0; //给下次循环用，需要放到+后面
    }
    idx--;
    jdx--;
    //std::cout << ".........." << ret_reverse << std::endl;
  }
  //最后一个进位不要丢了
  if (1 == jingwei) {
    ret_reverse += '0' + 1;
  }

  //转置一下
  std::string ret;
  for (int idx = ret_reverse.size() - 1; idx >= 0; idx--) {
    ret += ret_reverse[idx];
  }

  return ret;
}

//多位数乘以一位数
std::string multiply_siglenum(const std::string& num1, char num2) {
  if (num2 == '0') {
    return "0";
  }

  int num2_int = num2 - '0';

  std::string ret_reverse;
  int add = 0;
  for (int idx = num1.size() - 1; idx >= 0; idx--) {
    int ji = (num1[idx] - '0') * num2_int;
    char tmp = (ji % 10 + add) % 10 + '0';
    add = ji / 10 + ((ji % 10 + add) >= 10 ? 1 : 0);
    ret_reverse += tmp;
  }

  if (add > 0){
    ret_reverse += add + '0';
  }

  //转置一下
  std::string ret;
  for (int idx = ret_reverse.size() - 1; idx >= 0; idx--) {
    ret += ret_reverse[idx];
  }
  //std::cout << num1 << " * " << num2 << " = " << ret << std::endl;
  return ret;
}

//大数相乘
std::string multiply(const std::string& num1, const std::string& num2) {
  if (num1 == "0" || num2 == "0") {
    return "0"; //有一个为0，则返回0
  }
  //由于num1和num2只能由数字组成，所以不再另外check，否则需要check
  int len1 = num1.size();
  int len2 = num2.size();
  //这里长度随便取len1还是len2，这里取len2
  std::vector<std::string> bit_ans(len2);
  int add_zero = 0;
  for (int idx = len2 - 1; idx >= 0; idx--) {
    bit_ans[idx] = multiply_siglenum(num1, num2[idx]);
    //从第二位开始要补0
    for (int aa = 0; aa < add_zero; aa++) {
      bit_ans[idx] += '0';
    }
    add_zero++;
  }

  std::string str_ret = bit_ans[0];
  for (int idx = 1; idx < bit_ans.size(); idx++) {
    str_ret = addStrings(str_ret, bit_ans[idx]);
  }

  return str_ret;
}

/*
大数相乘标记的是中等，但是这题实际很难，很费时间,而且要考虑各种条件和边界，需要debug
需要实现:
1：大数相加
2：多位数陈一位数
*/
int main() {
  std::string num1("123");
  std::string num2("456");
  std::string result = multiply(num1, num2);
  std::cout << num1 << " * " << num2 << " = " << result << std::endl;

  num1 = "123456789";
  num2 = "9";
  result = multiply(num1, num2);
  std::cout << num1 << " * " << num2 << " = " << result << std::endl;

  num1 = "123456789123456789123456789123456789";
  num2 = "123456789123456789123456789123456789";
  result = multiply(num1, num2);
  std::cout << num1 << " * " << num2 << " = " << result << std::endl;
  return 0;
}
