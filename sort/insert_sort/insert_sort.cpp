#include <iostream>  
#include <vector>  

using namespace std;

// 插入排序函数  
void insertionSort(vector<int>& arr) {
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    int key = arr[i]; // 当前需要插入的元素  
    int j = i - 1;

    // 将arr[i]插入到arr[0]...arr[i-1]已排序的序列中  
    // 移动元素，为key找到合适的位置  
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

// 主函数，用于测试插入排序  
int main() {
  vector<int> arr = { 12, 11, 13, 5, 6, 7 };
  insertionSort(arr);

  cout << "Sorted array: \n";
  for (int i = 0; i < arr.size(); i++)
    cout << arr[i] << " ";
  cout << endl;

  return 0;
}
