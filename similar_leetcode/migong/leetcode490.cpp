#include <iostream>
#include <vector>
#include <queue>
#include "migong490.h"

/*
题目描述:
由空地和墙组成的迷宫中有一个球。球可以向上下左右四个方向滚动，但在遇到墙壁前不会停止滚动。当球停下时，可以选择下一个方向。

给定球的起始位置，目的地和迷宫，判断球能否在目的地停下。

迷宫由一个0和1的二维数组表示。 1表示墙壁，0表示空地。你可以假定迷宫的边缘都是墙壁。起始位置和目的地的坐标通过行号和列号给出。

示例:
示例 1:

输入 1: 迷宫由以下二维数组表示

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

输入 2: 起始位置坐标 (rowStart, colStart) = (0, 4)
输入 3: 目的地坐标 (rowDest, colDest) = (4, 4)

输出: true

解析: 一个可能的路径是 : 左 -> 下 -> 左 -> 下 -> 右 -> 下 -> 右。


示例 2:

输入 1: 迷宫由以下二维数组表示

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

输入 2: 起始位置坐标 (rowStart, colStart) = (0, 4)
输入 3: 目的地坐标 (rowDest, colDest) = (3, 2)

输出: false

解析: 没有能够使球停在目的地的路径。
*/

static std::pair<int, int> dirs[4]{{-1,0}, { 1,0 }, { 0,-1 }, { 0,1 }};

bool CanStop(std::vector<std::vector<int>>& data, std::vector<int>& end) {
	std::cout << "end pos:[" << end[0] << "," << end[1] << "]" << std::endl;
	for (int i = 0; i < 4; i++) {
		int row = end[0] + dirs[i].first;
		int col = end[1] + dirs[i].second;
		if (row < 0 || row >= data.size() || col < 0 || col >= data[0].size() || data[row][col] == 1) {
			continue;
		}
		return false;
	}
	return true;
}

bool Migongbfs(std::vector<std::vector<int>>& data, std::vector<int>& start, std::vector<int>& end){
	std::queue<std::pair<int, int>> que;
	que.push(std::pair<int, int>(start[0], start[1]));
	data[start[0]][start[1]] = 1;
	while (!que.empty()) {
		int size = que.size();
		while (size-- > 0) {
			std::pair<int, int> pos = que.front();
			que.pop();
			for (int i = 0; i < 4; i++) {
				int row = pos.first + dirs[i].first;
				int col = pos.second + dirs[i].second;
				if (row < 0 || row >= data.size() || col < 0 || col >= data[0].size() || data[row][col] == 1) {
					continue;
				}

				if (row == end[0] && col == end[1]) {
					if (CanStop(data, end)) {
						return true;
					}else {
						return false;
					}
				}
				que.push(std::pair<int, int>(row, col));
				data[row][col] = 1;
			}
		}
	}
	return false;
}

int Test490() {
	std::vector<std::vector<int>>data{
		{0,0,1,0,0},
		{0,0,0,0,0},
		{0,0,0,1,0},
		{1,1,0,1,1},
		{0,0,0,0,0},
	};
	std::vector<int> start{0, 4};
	std::vector<int> end{4, 4};
	bool ok = Migongbfs(data, start, end);
	std::cout << "test1 bfs is:" << ok << std::endl;

	std::vector<std::vector<int>>data1{
		{ 0,0,1,0,0 },
		{ 0,0,0,0,0 },
		{ 0,0,0,1,0 },
		{ 1,1,0,1,1 },
		{ 0,0,0,0,0 },
	};
	std::vector<int> start1{0, 4};
	std::vector<int> end1{3, 2};
	ok = Migongbfs(data1, start1, end1);
	std::cout << "test2 bfs is:" << ok << std::endl;

	return 0;
}

bool Dfs(std::vector<std::vector<int>>& data, int x, int y, std::vector<int>& end) {
	data[x][y] = 1;
	if (x == end[0] && y == end[1]) {
		if (CanStop(data, end)) {
			return true;
		}
		else {
			return false;
		}
	}
	for (int i = 0; i < 4; i++) {
		int row = x + dirs[i].first;
		int col = y + dirs[i].second;
		if (row < 0 || row >= data.size() || col < 0 || col >= data[0].size() || data[row][col] == 1) {
			continue;
		}
		if (Dfs(data, row, col, end)) {
			//只要找到了路径，即ok
			return true;
		}
	}
	return false;
}

bool Migongdfs(std::vector<std::vector<int>>& data, std::vector<int>& start, std::vector<int>& end) {
	return Dfs(data, start[0], start[1], end);
}

int Test1() {
	std::vector<std::vector<int>>data{
		{0, 0, 1, 0, 0},
		{ 0,0,0,0,0 },
		{ 0,0,0,1,0 },
		{ 1,1,0,1,1 },
		{ 0,0,0,0,0 },
	};
	std::vector<int> start{0, 4};
	std::vector<int> end{4, 4};
	bool ok = Migongdfs(data, start, end);
	std::cout << "test1 dfs is:" << ok << std::endl;

	std::vector<std::vector<int>>data1{
		{ 0, 0, 1, 0, 0 },
		{ 0,0,0,0,0 },
		{ 0,0,0,1,0 },
		{ 1,1,0,1,1 },
		{ 0,0,0,0,0 },
	};
	std::vector<int> start1{0, 4};
	std::vector<int> end1{3, 2};
	ok = Migongdfs(data1, start1, end1);
	std::cout << "test2 dfs is:" << ok << std::endl;

	return 0;
}

int Migong490::Test() {
	Test490();
	std::cout << std::endl;
	Test1();
	return 0;
}
