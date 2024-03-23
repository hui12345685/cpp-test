#include "SFINAE_test.h"
#include "fib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

int sfinae_test() {
    std::cout << "reserve:" << has_reserve<TestReserve>::value << std::endl;//reserve: 1
    std::cout << "reserve:" << has_reserve<Bar>::value << std::endl; //reserve:  0
    return 0;
}

int count_if_test(){
    std::vector<int> test;
    for (int i = 0; i < 10; i++) {
        test.push_back(i);
    }
    int cnt = std::count_if(test.begin(), test.end(), bind2nd(std::greater<int>(), 2));
    std::cout << "big than two cnt is:" << cnt << std::endl;
    return 0;
}

int main() {
	  sfinae_test();

    count_if_test();

    Solution test;
    test.fib(10);
}
