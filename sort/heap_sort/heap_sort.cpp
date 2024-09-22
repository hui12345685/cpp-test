#include <iostream>  
#include <vector>  
using namespace std;

// �����ѵĺ���  
void heapify(vector<int>& arr, int n, int i) {
  int largest = i; // ��ʼ�����Ϊ��  
  int left = 2 * i + 1; // �� = 2*i + 1  
  int right = 2 * i + 2; // �� = 2*i + 2  

  // ������ӽڵ���ڸ�  
  if (left < n && arr[left] > arr[largest])
    largest = left;

  // ������ӽڵ����Ŀǰ���  
  if (right < n && arr[right] > arr[largest])
    largest = right;

  // �������Ǹ�  
  if (largest != i) {
    swap(arr[i], arr[largest]);

    // �ݹ�ص�����Ӱ�������  
    heapify(arr, n, largest);
  }
}

// ��������  
void heapSort(vector<int>& arr) {
  int n = arr.size();

  // �����ѣ��������飩  
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);

  // һ�����ӶѶ�ȡ��Ԫ��  
  for (int i = n - 1; i > 0; i--) {
    // �ƶ���ǰ����ĩβ  
    swap(arr[0], arr[i]);

    // ����max heapify�ڼ��ٵĶ���  
    heapify(arr, i, 0);
  }
}

// ��ӡ����ĺ���  
void printArray(const vector<int>& arr) {
  for (int i = 0; i < arr.size(); ++i)
    cout << arr[i] << " ";
  cout << endl;
}

// ��������������������  
int main() {
  vector<int> arr = { 12, 11, 13, 5, 6, 7 ,10};
  int n = arr.size();

  heapSort(arr);

  cout << "Sorted array is \n";
  printArray(arr);
  return 0;
}
