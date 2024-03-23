#include <iostream>

const int score = 90;
const int num = 10;

uint64_t loop_times = 0;

void BackTracking(int start, int sum, int& count) {
	if (sum > score|| start > num) {
		return;
	}
	//��֦
	if ((sum + 10 * (num - start)) < score) {
		return;
	}
	if (start == num && sum == score) {
		count++;
	}
	//forѭ�����ƻ���
	for (int i = 0; i <= 10; i++) {
		//���ݿ��ƴ���
		BackTracking(start + 1, sum + i, count);
	}
}

int Cacl() {
	int count = 0;
	BackTracking(0, 0, count);
	return count;
}

//1�����10�Σ��ܴ���90��������ж�����(�����ܴ���0��10���е�һ�����)
//2������ÿ�ֻ������еĸ�����ȣ��ʴ���90���ĸ����Ƕ���
void Function1() {
	int count = Cacl();
	std::cout << "count:" << count << std::endl;
	std::cout << "rate:" << (count / pow(11, 10)) * 100 << std::endl;
}

int main() {
	Function1();
	return 0;
}
