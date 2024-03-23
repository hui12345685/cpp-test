#include "mempro_file.h"
#include "mempro_file_test.h"

int main(){
  std::cout << sizeof(bdf::monitor::PerfHeader) << std::endl;
  /*std::cout << std::is_pod<Test>::value << std::endl;
  std::cout<< std::is_pod<TestA1>::value <<std::endl;
  std::cout << std::is_pod<TestA1A2>::value << std::endl;
  std::cout << std::is_pod<TestA3>::value << std::endl;
  std::cout << std::is_pod<TplTestA4<int>>::value << std::endl;
  std::cout << std::is_pod<TplTestA5<int,int>>::value << std::endl;*/

  std::cout << (std::is_trivial <Test>::value && std::is_standard_layout  <Test>::value) << std::endl;
  std::cout << (std::is_trivial <TestA1>::value && std::is_standard_layout  <Test>::value) << std::endl;
  std::cout << (std::is_trivial <TestA1A2>::value && std::is_standard_layout  <Test>::value) << std::endl;
  std::cout << (std::is_trivial <TestA3>::value && std::is_standard_layout  <Test>::value) << std::endl;
  std::cout << (std::is_trivial <TplTestA4<int>>::value && std::is_standard_layout  <Test>::value) << std::endl;
  std::cout << (std::is_trivial <TplTestA5<int, int>>::value && std::is_standard_layout  <Test>::value) << std::endl;

  Test* test = BDF_NEW(Test);
  TestA1* test_a1 = BDF_NEW(TestA1);
  TestA1A2* test_a1a2 = BDF_NEW(TestA1A2);
  TestA3* test_a3 = BDF_NEW(TestA3,10);
  TplTestA4<int>* test_a4 = BDF_NEW(TplTestA4<int>, 10);
  TplTestA5<int, int> test_a5(10, 20);

  TplTestA5<int, int>* ptest_a5 = new TplTestA5<int, int> ( 10, 20);
  delete ptest_a5;

  //const int a1 = 10;
  //const int a2 = 20;
  //TplTestA5<const int, const int>* pptest_a5 = 
  //  BDF_NEW((TplTestA5<const int, const int>), a1, a2);

  BDF_DELETE(test);
  BDF_DELETE(test_a1);
  BDF_DELETE(test_a1a2);
  BDF_DELETE(test_a3);
  BDF_DELETE(test_a4);
  //BDF_DELETE(pptest_a5);
  return 0;
}
