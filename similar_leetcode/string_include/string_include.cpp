// string_include.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "string_include_backtracking.h"
#include "string_include_dp.h"

int main()
{
  solution1::StringIncludeBacking s1;
  s1.Solution1Test();

  solution2::StringIncludeDp s2;
  s2.Solution2Test();
}
