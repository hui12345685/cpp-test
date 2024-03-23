#include <iostream>
#include <vector>
#include <queue>
#include "migong505.h"

/*
��Ŀ
�ɿյغ�ǽ��ɵ��Թ�����һ����
 ����������������ĸ������������������ǽ��ǰ����ֹͣ������
 ����ͣ��ʱ������ѡ����һ������

���������ʼλ�ã�Ŀ�ĵغ��Թ����ҳ�����ͣ��Ŀ�ĵص���̾��롣
 ����Ķ����������ʼλ�ã�����������Ŀ�ĵأ������������Ŀյظ�����
 ������޷�ͣ��Ŀ�ĵأ����� -1��

�Թ���һ��0��1�Ķ�ά�����ʾ�� 1��ʾǽ�ڣ�0��ʾ�յء�
 ����Լٶ��Թ��ı�Ե����ǽ�ڡ�
 ��ʼλ�ú�Ŀ�ĵص�����ͨ���кź��кŸ�����


 ���� 1: �Թ������¶�ά�����ʾ

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

���� 2: ��ʼλ������ (rowStart, colStart) = (0, 4)
���� 3: Ŀ�ĵ����� (rowDest, colDest) = (4, 4)

���: 12

����: һ�����·�� : left -> down -> left -> down -> right -> down -> right��
			 �ܾ���Ϊ 1 + 1 + 3 + 1 + 2 + 2 + 2 = 12��


���� 1: �Թ������¶�ά�����ʾ

0 0 1 0 0
0 0 0 0 0
0 0 0 1 0
1 1 0 1 1
0 0 0 0 0

���� 2: ��ʼλ������ (rowStart, colStart) = (0, 4)
���� 3: Ŀ�ĵ����� (rowDest, colDest) = (3, 2)

���: -1
����: û���ܹ�ʹ��ͣ��Ŀ�ĵص�·����

ע��:
�Թ���ֻ��һ�����һ��Ŀ�ĵء�
���Ŀ�ĵض��ڿյ��ϣ��ҳ�ʼʱ���ǲ���ͬһλ�á�
�������Թ��������߽� (��ͼ�еĺ�ɫ����), ������Լ����Թ��ı�Ե����ǽ�ڡ�
�Թ����ٰ���2��յأ�������������������100��
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
				//���ߵ�ʱ��Ÿı�row��col��ֵ
				while (row + dirs[i].first >= 0 && row + dirs[i].first < data.size() 
					&& col + dirs[i].second >= 0 && col + dirs[i].second < data[0].size()
					&& data[row + dirs[i].first][col + dirs[i].second] == 0) {
					//��������Ҫ��һ�㣬һ����������ߣ���Ҫ�ߵ���
					row = row + dirs[i].first;
					col = col + dirs[i].second;
					delta++;
				}
				//�÷���ͨ
				if (row == pos.first && col == pos.second) {
					continue;
				}
				//��ǽ,�����ж෽��ʽ�ܵ�
				//���������������һ�θ�ֵ�������и��̵�·��
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
		return a[2] > b[2];//����С���ڶ�
	}
};

int shortestDistance(std::vector<std::vector<int>>& maze, std::vector<int>& start, std::vector<int>& destination) {
	int m = maze.size(), n = maze[0].size(), i, j, k, x, y, curstep, delta;
	std::vector<std::vector<int>> dir = { {1,0},{0,1},{0,-1},{-1,0} };
	std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, cmp> q;
	std::vector<std::vector<int>> dis(m, std::vector<int>(n, INT_MAX));
	dis[start[0]][start[1]] = 0;
	start.push_back(0);//�����������Ǿ���
	q.push(start);
	while (!q.empty())
	{
		i = q.top()[0];
		j = q.top()[1];
		curstep = q.top()[2];
		q.pop();
		if (dis[i][j] < curstep)//���벻����̵�
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
				delta++;//�ߵĲ�������
			}
			if (dis[i][j] + delta < dis[x][y])//�÷�����һ��λ��Ϊǽ�ڣ�ͣ����
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
				delta++;//�ߵĲ�������
			}
			if (dis[i][j] + delta < dis[x][y])//�÷�����һ��λ��Ϊǽ�ڣ�ͣ��xy
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
