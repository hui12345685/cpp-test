#include <iostream>  
#include <vector>  

using namespace std;

// ����������  
void insertionSort(vector<int>& arr) {
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    int key = arr[i]; // ��ǰ��Ҫ�����Ԫ��  
    int j = i - 1;

    // ��arr[i]���뵽arr[0]...arr[i-1]�������������  
    // �ƶ�Ԫ�أ�Ϊkey�ҵ����ʵ�λ��  
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

// �����������ڲ��Բ�������  
int main() {
  vector<int> arr = { 12, 11, 13, 5, 6, 7 };
  insertionSort(arr);

  cout << "Sorted array: \n";
  for (int i = 0; i < arr.size(); i++)
    cout << arr[i] << " ";
  cout << endl;

  return 0;
}
