#include <iostream>
#include <vector>
#include <queue>
#include "migong505.h"

/*
题目
由空地和墙组成的迷宫中有一个球。
 球可以向上下左右四个方向滚动，但在遇到墙壁前不会停止滚动。
 当球停下时，可以选择下一个方向。

给定球的起始位置，目的地和迷宫，找出让球停在目的地的最短距离。
 距离的定义是球从起始位置（不包括）到目的地（包括）经过的空地个数。
 如果球无法停在目的地，返回 -1。

迷宫由一个0和1的二维数组表示。 1表示墙壁，0表示空地。
 你可以假定迷宫的边缘都是墙壁。
 起始位置和目的地的坐标通过行号和列号给出。


 输入 1: 迷宫由以下二维数组表示

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

输入 2: 起始位置坐标 (rowStart, colStart) = (0, 4)
输入 3: 目的地坐标 (rowDest, colDest) = (4, 4)

输出: 12

解析: 一条最短路径 : left -> down -> left -> down -> right -> down -> right。
			 总距离为 1 + 1 + 3 + 1 + 2 + 2 + 2 = 12。


输入 1: 迷宫由以下二维数组表示

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

输入 2: 起始位置坐标 (rowStart, colStart) = (0, 4)
输入 3: 目的地坐标 (rowDest, colDest) = (3, 2)

输出: -1
解析: 没有能够使球停在目的地的路径。

注意:
迷宫中只有一个球和一个目的地。
球和目的地都在空地上，且初始时它们不在同一位置。
给定的迷宫不包括边界 (如图中的红色矩形), 但你可以假设迷宫的边缘都是墙壁。
迷宫至少包括2块空地，行数和列数均不超过100。
*/

static std::pair<int, int> dirs[4]{ {-1,0}, { 1,0 }, { 0,-1 }, { 0,1 } };

int Migongbfs(std::vector<std::vector<int>>& data, std::vector<int>& start, std::vector<int>& end) {
	int result = 0;
	std::queue<std::pair<int, int>> que;
	que.push(std::pair<int, int>(start[0], start[1]));
	std::vector<std::vector<int>>dist(data.size(), std::vector<int>(data[0].size(), INT_MAX));
	dist[start[0]][start[1]] = 0;
	while (!que.empty()) {
		int size = que.size();
		//std::cout << "queue size:" << size << std::endl;
		while (size-- > 0) {
			std::pair<int, int> pos = que.front();
			que.pop();
			for (int i = 0; i < 4; i++) {
				int row = pos.first;
				int col = pos.second;
				int delta = 0;
				//能走的时候才改变row和col的值
				while (row + dirs[i].first >= 0 && row + dirs[i].first < data.size() 
					&& col + dirs[i].second >= 0 && col + dirs[i].second < data[0].size()
					&& data[row + dirs[i].first][col + dirs[i].second] == 0) {
					//这里最重要的一点，一个方向可以走，就要走到低
					row = row + dirs[i].first;
					col = col + dirs[i].second;
					delta++;
				}
				//该方向不通
				if (row == pos.first && col == pos.second) {
					continue;
				}
				//靠墙,可能有多方方式能到
				//包括两种情况，第一次赋值，或者有更短的路径
				if (dist[pos.first][pos.second] + delta < dist[row][col]) {
					dist[row][col] = dist[pos.first][pos.second] + delta;
					que.push(std::pair<int, int>(row, col));
				}
			}
		}
	}
	return dist[end[0]][end[1]] == INT_MAX ? -1 : dist[end[0]][end[1]];
}

struct cmp
{
	bool operator()(const std::vector<int>& a, const std::vector<int>& b) const
	{
		return a[2] > b[2];//距离小的在顶
	}
};

int shortestDistance(std::vector<std::vector<int>>& maze, std::vector<int>& start, std::vector<int>& destination) {
	int m = maze.size(), n = maze[0].size(), i, j, k, x, y, curstep, delta;
	std::vector<std::vector<int>> dir = { {1,0},{0,1},{0,-1},{-1,0} };
	std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, cmp> q;
	std::vector<std::vector<int>> dis(m, std::vector<int>(n, INT_MAX));
	dis[start[0]][start[1]] = 0;
	start.push_back(0);//第三个参数是距离
	q.push(start);
	while (!q.empty())
	{
		i = q.top()[0];
		j = q.top()[1];
		curstep = q.top()[2];
		q.pop();
		if (dis[i][j] < curstep)//距离不是最短的
			continue;
		for (k = 0; k < 4; ++k)
		{
			x = i;
			y = j;
			delta = 0;
			while (x + dir[k][0] >= 0 && x + dir[k][0] < m && y + dir[k][1] >= 0 && y + dir[k][1] < n
				&& maze[x + dir[k][0]][y + dir[k][1]] == 0)
			{
				x += dir[k][0];
				y += dir[k][1];
				delta++;//走的步数增量
			}
			if (dis[i][j] + delta < dis[x][y])//该方向下一个位置为墙壁，停在这
			{
				dis[x][y] = dis[i][j] + delta;
				q.push({ x,y,dis[x][y] });
			}
		}
	}
	return dis[destination[0]][destination[1]] == INT_MAX ? -1 : dis[destination[0]][destination[1]];
}

int shortestDistance1(std::vector<std::vector<int>>& maze, std::vector<int>& start, std::vector<int>& destination) {
	int m = maze.size(), n = maze[0].size(), i, j, k, x, y, delta;
	std::vector<std::vector<int>> dir = { {1,0},{0,1},{0,-1},{-1,0} };
	std::queue<std::vector<int>> q;
	std::vector<std::vector<int>> dis(m, std::vector<int>(n, INT_MAX));
	dis[start[0]][start[1]] = 0;
	q.push(start);
	while (!q.empty())
	{
		i = q.front()[0];
		j = q.front()[1];
		q.pop();
		for (k = 0; k < 4; ++k)
		{
			x = i;
			y = j;
			delta = 0;
			while (x + dir[k][0] >= 0 && x + dir[k][0] < m && y + dir[k][1] >= 0 && y + dir[k][1] < n
				&& maze[x + dir[k][0]][y + dir[k][1]] == 0)
			{
				x += dir[k][0];
				y += dir[k][1];
				delta++;//走的步数增量
			}
			if (dis[i][j] + delta < dis[x][y])//该方向下一个位置为墙壁，停在xy
			{
				dis[x][y] = dis[i][j] + delta;
				q.push({ x,y });
			}
		}
	}
	return dis[destination[0]][destination[1]] == INT_MAX ? -1 : dis[destination[0]][destination[1]];
}

int Test505() {
	std::vector<std::vector<int>>data{
		{0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0},
		{1, 1, 0, 1, 1},
		{0, 0, 0, 0, 0},
	};
	std::vector<int> start{0, 4};
	std::vector<int> end{4, 4};
	int dist = Migongbfs(data, start, end);
	std::cout << "test1 bfs is:" << dist << std::endl << std::endl;

	std::vector<std::vector<int>>data1{
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 },
		{ 0, 0, 0, 0, 0 },
	};
	std::vector<int> start1{0, 4};
	std::vector<int> end1{3, 2};
	dist = Migongbfs(data1, start1, end1);
	std::cout << "test2 bfs is:" << dist << std::endl << std::endl;

	std::vector<std::vector<int>>data2{
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 1, 1, 0, 1, 1 },
		{ 0, 0, 0, 0, 0 },
	};
	dist = Migongbfs(data2, start, end);
	//dist = shortestDistance(data2, start, end);
	//dist = shortestDistance1(data2, start, end);
	std::cout << "test3 bfs is:" << dist << std::endl << std::endl;

	return 0;
}

int Migong505::Test() {
	Test505();
	return 0;
}
