#include <iostream>  
#include <vector>  
#include <queue>  
#include <random>

using namespace std;

std::mt19937 generator(std::random_device{}()); // 使用硬件生成器来初始化随机数生成器  
std::uniform_int_distribution<int> distribution(1, 10000000); // 定义分布范围  

int generateRandomNumber() {
  return distribution(generator);
}

/*int generateRandomNumber(int min, int max) {
  return min + rand() % (max - min + 1);
}*/

int main() {
  srand(time(0));

  const int N = 100000;
  const int K = 10;

  priority_queue<int, vector<int>, greater<int>> pq;

  for (int i = 0; i < N; ++i) {
    int num = generateRandomNumber();
    if (pq.size() < K || num > pq.top()) {
      if (pq.size() == K) pq.pop();
      pq.push(num);
    }
  }

  cout << "The top " << K << " numbers are:" << endl;
  for (int i = 0; i < K; ++i) {
    cout << pq.top() << (i < K - 1 ? ", " : ".") << endl;
    pq.pop();
  }

  return 0;
}
