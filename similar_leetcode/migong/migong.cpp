#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "migong.h"

static std::pair<int, int> dir[4]{{-1, 0},{1, 0}, {0, -1}, {0, 1}};

int Migong2(std::vector<std::string>& data, std::vector<int>& start, std::vector<int>& end) {
	std::vector<std::vector<int>> dist(data.size(), std::vector<int>(data[0].size(), INT_MAX));
	std::queue<std::pair<int, int>> que;
	que.push(std::pair<int, int>(start[0], start[1]));
	data[start[0]][start[1]] = '#';
	dist[start[0]][start[1]] = 0;
	while (!que.empty()) {
		std::pair<int, int> pos = que.front();
		que.pop();
		for (int i = 0; i < 4; i++) {
			int x = pos.first + dir[i].first;
			int y = pos.second + dir[i].second;
			if (x < 0 || x >= data.size() || y < 0 || y >= data[0].size() || data[x][y] == '#') {
				//不合法的位置，跳过
				continue;
			}
			dist[x][y] = dist[pos.first][pos.second] + 1;
			if (x == end[0] && y == end[1]) {
				//找到结束位置了
				return dist[x][y];
			}
			que.push(std::pair<int, int>(x, y));
			data[x][y] = '#';
		}
	}
	return -1;
}


void Test2() {
	time_t tbegin = time(nullptr);
	//S表示开始的位置，G表示结束的位置
	//#表示墙，不能走，.表示可以走
	std::vector<std::string>data{
			"#S######.#",
			"......#..#",
			".#.##.##.#",
			".#........",
			"##.##.####",
			"....#....#",
			".#######.#",
			"....#.....",
			".####.###.",
			"....#...G#"};
	std::vector<int> start{0, 1};
	std::vector<int> end{9, 8};
	int min_path = Migong2(data, start, end);
	std::cout << "Test2 min_path:" << min_path << std::endl;
	time_t tend = time(nullptr);
	std::cout << "Test2 cost time:" << (tend - tbegin) << " second." << std::endl;
}

int Migong(std::vector<std::string>& data, std::vector<int>& start, std::vector<int>& end) {
	std::cout << "migong size:" << data.size() * data[0].size() << std::endl;
	int loop_cnt = 0;
	int min_path = 0;
	std::queue<std::pair<int, int>> que;
	que.push(std::pair<int, int>(start[0], start[1]));
	//走过的路要设置为强，不要再走了，这样才能提高效率,否则效率很差
	data[start[0]][start[1]] = '#';
	while (!que.empty()) {
		int size = que.size();
		loop_cnt += size;
		min_path++;
		//std::cout << "queue size:" << size << ",min_path:" << min_path << std::endl;
		while (size--) {
			std::pair<int, int> pos = que.front();
			que.pop();
			for (int i = 0; i < 4; i++) {
				int x = pos.first + dir[i].first;
				int y = pos.second + dir[i].second;
				if (x == end[0] && y == end[1]) {
					//找到结束位置了
					std::cout << "loop count :" << loop_cnt << std::endl;
					return min_path;
				}

				//到达上下左右，四个位置最短路劲都是min_path
				if (x >= 0 && x < data.size() && y >= 0 && y < data[0].size()) {
					if (data[x][y] == '#') {
						//遇到墙，不通
						continue;
					}
					que.push(std::pair<int, int>(x, y));
					//走过的路要设置为强，不要再走了，这样才能提高效率,否则效率很差
					data[x][y] = '#';
				}
			}
		}
	}
	std::cout << "loop count :" << loop_cnt << std::endl;
	return -1;
}

void Test1() {
	//S表示开始的位置，G表示结束的位置
	//#表示墙，不能走，.表示可以走
	time_t tbegin = time(nullptr);
	std::vector<std::string>data{
			"#S######.#",
			"......#..#",
			".#.##.##.#",
			".#........",
			"##.##.####",
			"....#....#",
			".#######.#",
			"....#.....",
			".####.###.",
			"....#...G#"};
	std::vector<int> start{0,1};
	std::vector<int> end{9,8};
	int min_path = Migong(data, start, end);
	std::cout << "Test1 min_path:" << min_path << std::endl;
	time_t tend = time(nullptr);
	std::cout << "Test1 cost time:" << (tend - tbegin) << " second." << std::endl;
}


int Migong::Test() {
	//两种方法的效率差不多，只是方法二的空间复杂度更高
	Test1(); 
	Test2();
	return 0;
}
