#include <iostream>  
#include <vector>  
using namespace std;

// 调整堆的函数  
void heapify(vector<int>& arr, int n, int i) {
  int largest = i; // 初始化最大为根  
  int left = 2 * i + 1; // 左 = 2*i + 1  
  int right = 2 * i + 2; // 右 = 2*i + 2  

  // 如果左子节点大于根  
  if (left < n && arr[left] > arr[largest])
    largest = left;

  // 如果右子节点大于目前最大  
  if (right < n && arr[right] > arr[largest])
    largest = right;

  // 如果最大不是根  
  if (largest != i) {
    swap(arr[i], arr[largest]);

    // 递归地调整受影响的子树  
    heapify(arr, n, largest);
  }
}

// 堆排序函数  
void heapSort(vector<int>& arr) {
  int n = arr.size();

  // 构建堆（重排数组）  
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  // 一个个从堆顶取出元素  
  for (int i = n - 1; i > 0; i--) {
    // 移动当前根到末尾  
    swap(arr[0], arr[i]);

    // 调用max heapify在减少的堆上  
    heapify(arr, i, 0);
  }
}

// 打印数组的函数  
void printArray(const vector<int>& arr) {
  for (int i = 0; i < arr.size(); ++i)
    cout << arr[i] << " ";
  cout << endl;
}

// 主函数来测试上述函数  
int main() {
  vector<int> arr = { 12, 11, 13, 5, 6, 7 ,10};
  int n = arr.size();

  heapSort(arr);

  cout << "Sorted array is \n";
  printArray(arr);
  return 0;
}
