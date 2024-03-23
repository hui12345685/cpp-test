#include <string>
#include <iostream>
#include <vector>

//�������
std::string addStrings(const std::string& num1, const std::string& num2) {
  //num1 ��num2 ��ֻ�������� 0-9,���Բ��ڶ���check��������Ҫcheck��
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
      //ascii��9+1=:������10������ҪתΪint������
      tmp = num1[idx] - '0' + jingwei;
    }
    else if (idx < 0 && jdx >= 0) {
      //ascii��9+1=:������10������ҪתΪint������
      tmp = num2[jdx] - '0' + jingwei;
    }
    //std::cout << ".........." << tmp << std::endl;

    if (tmp >= 10) {
      tmp -= 10;
      ret_reverse += '0' + tmp; //��Ҫ��ȥ10֮���ټ�
      jingwei = 1; //���´�ѭ���ã���Ҫ�ŵ�+����
    }
    else {
      ret_reverse += '0' + tmp;
      jingwei = 0; //���´�ѭ���ã���Ҫ�ŵ�+����
    }
    idx--;
    jdx--;
    //std::cout << ".........." << ret_reverse << std::endl;
  }
  //���һ����λ��Ҫ����
  if (1 == jingwei) {
    ret_reverse += '0' + 1;
  }

  //ת��һ��
  std::string ret;
  for (int idx = ret_reverse.size() - 1; idx >= 0; idx--) {
    ret += ret_reverse[idx];
  }

  return ret;
}

//��λ������һλ��
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

  //ת��һ��
  std::string ret;
  for (int idx = ret_reverse.size() - 1; idx >= 0; idx--) {
    ret += ret_reverse[idx];
  }
  //std::cout << num1 << " * " << num2 << " = " << ret << std::endl;
  return ret;
}

//�������
std::string multiply(const std::string& num1, const std::string& num2) {
  if (num1 == "0" || num2 == "0") {
    return "0"; //��һ��Ϊ0���򷵻�0
  }
  //����num1��num2ֻ����������ɣ����Բ�������check��������Ҫcheck
  int len1 = num1.size();
  int len2 = num2.size();
  //���ﳤ�����ȡlen1����len2������ȡlen2
  std::vector<std::string> bit_ans(len2);
  int add_zero = 0;
  for (int idx = len2 - 1; idx >= 0; idx--) {
    bit_ans[idx] = multiply_siglenum(num1, num2[idx]);
    //�ӵڶ�λ��ʼҪ��0
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
������˱�ǵ����еȣ���������ʵ�ʺ��ѣ��ܷ�ʱ��,����Ҫ���Ǹ��������ͱ߽磬��Ҫdebug
��Ҫʵ��:
1���������
2����λ����һλ��
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
