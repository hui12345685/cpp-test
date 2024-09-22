#include <iostream>  
#include <vector>  

using namespace std;

// ��������ķ�������  
int partition(vector<int>& arr, int low, int high) {
  // ѡ�����һ��Ԫ����Ϊ��׼ֵ  
  int pivot = arr[high];
  int i = (low - 1); // ��СԪ�ص�����  

  for (int j = low; j <= high - 1; j++) {
    // �����ǰԪ��С�ڻ���ڻ�׼ֵ  
    if (arr[j] <= pivot) {
      i++;    // �ƶ���СԪ�ص�����  
      swap(arr[i], arr[j]);
    }
  }
  swap(arr[i + 1], arr[high]);
  return (i + 1);
}

// ����������  
void quickSort(vector<int>& arr, int low, int high) {
  if (low < high) {
    // pi �Ƿ����������׼ֵ����ȷλ��  
    int pi = partition(arr, low, high);

    // �ݹ�������׼ֵ��ߵ�������  
    quickSort(arr, low, pi - 1);

    // �ݹ�������׼ֵ�ұߵ�������  
    quickSort(arr, pi + 1, high);
  }
}

// ��ӡ����ĺ���  
void printArray(const vector<int>& arr) {
  for (int i = 0; i < arr.size(); i++)
    cout << arr[i] << " ";
  cout << endl;
}

// ��������������������  
int main() {
  vector<int> arr = { 10, 7, 8, 9, 1, 2, 5 };
  int n = arr.size();

  quickSort(arr, 0, n - 1);

  cout << "Sorted array: \n";
  printArray(arr);
  return 0;
}
