#pragma once
#include <string>
#include <vector>

namespace solution1 {

  class StringIncludeBacking {
  public:
    bool is_concatenation_of_words(std::string& str, std::vector<std::string>& words);
    void Solution1Test();
  };

}
