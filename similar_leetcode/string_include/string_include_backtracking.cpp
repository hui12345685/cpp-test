#include "string_include_backtracking.h"
#include <iostream>

namespace solution1 {

  static int times = 0;

  bool IsEqual(std::string& str, std::vector<std::string>& path) {
    std::string path_str;
    for (auto s : path) {
      path_str += s;
    }

    // std::cout << "str:" << str << times << ",path:" << path_str << std::endl;
    return str == path_str;
  }

  void BackTracking(
    std::string& str, std::vector<std::string>& words, std::vector<bool>& is_used,
    std::vector<std::string>& path, bool& is_fromed) {
    if (is_fromed) {
      return;
    }
    if (IsEqual(str, path)) {
      is_fromed = true;
      return;
    }
    for (int i = 0; i < words.size(); i++) {
      if (is_used[i]) {
        continue;
      }
      path.push_back(words[i]);
      is_used[i] = true;
      BackTracking(str, words, is_used, path, is_fromed);
      path.pop_back();
      is_used[i] = false;
    }
  }

  /* 题目描述：
  str是一个字符串，words是一个字符串列表。
  实现一个函数，如果str能由words里面的单词拼接而成，返回true，否则返回false 
  如str为abcde，words列表为"ab", "abcd", "de"，返回false
  如str为abcde，words列表为"ab", "abcd", "cde"，返回true 
  下面是回溯的解法 */
  bool StringIncludeBacking::is_concatenation_of_words(std::string& str, std::vector<std::string>& words) {
    bool is_fromed = false;
    std::vector<bool> is_used(words.size(), false);
    std::vector<std::string> path;
    BackTracking(str, words, is_used, path, is_fromed);
    times++;
    std::cout << "str:" << str << ",is_fromed:" << is_fromed << std::endl;
    return is_fromed;
  }

  void StringIncludeBacking::Solution1Test() {
    std::cout << "StringIncludeBacking::Solution1Test()--------------" << std::endl;
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
