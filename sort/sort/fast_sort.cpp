#include <iostream>  
#include <vector>  

using namespace std;

// 快速排序的分区函数  
int partition(vector<int>& arr, int low, int high) {
  // 选择最后一个元素作为基准值  
  int pivot = arr[high];
  int i = (low - 1); // 较小元素的索引  

  for (int j = low; j <= high - 1; j++) {
    // 如果当前元素小于或等于基准值  
    if (arr[j] <= pivot) {
      i++;    // 移动较小元素的索引  
      swap(arr[i], arr[j]);
    }
  }
  swap(arr[i + 1], arr[high]);
  return (i + 1);
}

// 快速排序函数  
void quickSort(vector<int>& arr, int low, int high) {
  if (low < high) {
    // pi 是分区操作后基准值的正确位置  
    int pi = partition(arr, low, high);

    // 递归地排序基准值左边的子数组  
    quickSort(arr, low, pi - 1);

    // 递归地排序基准值右边的子数组  
    quickSort(arr, pi + 1, high);
  }
}

// 打印数组的函数  
void printArray(const vector<int>& arr) {
  for (int i = 0; i < arr.size(); i++)
    cout << arr[i] << " ";
  cout << endl;
}

// 主函数来测试上述函数  
int main() {
  vector<int> arr = { 10, 7, 8, 9, 1, 2, 5 };
  int n = arr.size();

  quickSort(arr, 0, n - 1);

  cout << "Sorted array: \n";
  printArray(arr);
  return 0;
}
