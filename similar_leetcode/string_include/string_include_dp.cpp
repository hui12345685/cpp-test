#include "string_include_dp.h"
#include <iostream>
#include <unordered_set>

namespace solution2 {

  /* 题目描述：
  str是一个字符串，words是一个字符串列表。
  实现一个函数，如果str能由words里面的单词拼接而成，返回true，否则返回false
  如str为abcde，words列表为"ab", "abcd", "de"，返回false
  如str为abcde，words列表为"ab", "abcd", "cde"，返回true
  下面是动态规划的解法 */
  bool StringIncludeDp::is_concatenation_of_words(std::string& str, std::vector<std::string>& words) {
    // 这题比较好的方法是dp，不是回溯
    // dp[i]表示以i-1结尾的字符串是否由words拼接而成
    std::vector<bool> dp(str.size()+1);
    std::unordered_set<std::string> words_set;
    for (auto& word : words) {
      words_set.insert(word);
    }
    dp[0] = true;
    for (int j = 0; j < str.size(); j++) {
      // std::unordered_set<std::string> tmp = words_set;
      for (int i = j+1; dp[j] && i <= str.size(); i++) {
        std::string sub = str.substr(j, i - j);
        if (words_set.find(sub) != words_set.end()) {
          dp[i] = true;
          words_set.erase(sub); // 如果不erase，最后一个用例会有问题，也就是会存在一个字符串用多次的情况
        }
      }
    }
    std::cout << "str:" << str  << ",is_fromed:" << dp[str.size()] << std::endl;
    return dp[str.size()];
  }

  void StringIncludeDp::Solution2Test() {
    std::cout << "StringIncludeDp::Solution2Test()--------------" << std::endl;
    std::string str("abcde");
    std::vector<std::string> words {"ab", "abcd", "de"}; // false
    is_concatenation_of_words(str, words);

    std::string str1("abcde");
    std::vector<std::string> words1 {"ab", "bcd", "ae"}; // false
    is_concatenation_of_words(str1, words1);

    std::string str2("abcde");
    std::vector<std::string> words2 {"ab", "abcd", "cde"};  // true
    is_concatenation_of_words(str2, words2);

    std::string str3("abcde");
    std::vector<std::string> words3 {"ab", "abcd", "e"}; // true
    is_concatenation_of_words(str3, words3);

    std::string str4("abcde");
    std::vector<std::string> words4 {"ab", "abcd", "de", "c"}; // true
    is_concatenation_of_words(str4, words4);

    std::string str5("eabcd");
    std::vector<std::string> words5 {"ab", "abcd", "e"}; // true
    is_concatenation_of_words(str5, words5);

    std::string str6("cabde");
    std::vector<std::string> words6 {"ab", "abcd", "de", "c"}; // true
    is_concatenation_of_words(str6, words6);

    std::string str7("ababcde");
    std::vector<std::string> words7 {"ab", "abcd", "de"}; // false
    is_concatenation_of_words(str7, words7);

    std::string str8("ababde");
    std::vector<std::string> words8 {"ab", "abcd", "de"}; // false
    is_concatenation_of_words(str8, words8);
  }
}
