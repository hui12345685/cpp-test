#include <iostream>

const int score = 90;
const int num = 10;

uint64_t loop_times = 0;

void BackTracking(int start, int sum, int& count) {
	if (sum > score|| start > num) {
		return;
	}
	//剪枝
	if ((sum + 10 * (num - start)) < score) {
		return;
	}
	if (start == num && sum == score) {
		count++;
	}
	//for循环控制环数
	for (int i = 0; i <= 10; i++) {
		//回溯控制次数
		BackTracking(start + 1, sum + i, count);
	}
}

int Cacl() {
	int count = 0;
	BackTracking(0, 0, count);
	return count;
}

//1：打靶10次，能打中90环的情况有多少中(假如能打中0到10环中的一种情况)
//2：假如每种环数打中的概率相等，问打中90环的概率是多少
void Function1() {
	int count = Cacl();
	std::cout << "count:" << count << std::endl;
	std::cout << "rate:" << (count / pow(11, 10)) * 100 << std::endl;
}

int main() {
	Function1();
	return 0;
}
