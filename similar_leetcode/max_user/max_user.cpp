#include <iostream>
#include<vector>
#include <algorithm>

struct Time {
  int time;
  bool is_start;
  /*bool operator==(const Time& time) {
    if (time.time == this->time && time.is_start == this->is_start){
      return true;
    }
    return false;
  }*/
};

int Compair(Time& t1, Time& t2) {
  if (t1.time < t2.time) {
    return  true;
  }
  if (t1.time == t2.time) {
    if (t1.is_start && !t2.is_start) {
      return true;
    }
  }
  return false;
}

int MaxUser(std::vector<std::pair<int, int>>& times) {
  if (times.size() <= 1) {
    return times.size();
  }
  //将二维数组打平
  std::vector<Time> times_start_end;
  for (auto& tm : times) {
    Time start;
    start.time = tm.first;
    start.is_start = true;
    times_start_end.push_back(start);

    Time end;
    end.time = tm.second;
    end.is_start = false;
    times_start_end.push_back(end);
  }

  int max_usesr = 0;
  int user = 0;
  std::sort(times_start_end.begin(), times_start_end.end(), &Compair);
  for (auto& tm : times_start_end) {
    if (tm.is_start) {
      user++;
    }
    else {
      user--;
    }

    if (user > max_usesr) {
      max_usesr = user;
    }
  }
  return max_usesr;
}

/*已知每个用户的登录登出时间，求在线的最大用户数
*/
int main() {
  std::vector<std::pair<int, int>> times;
  times.push_back(std::pair<int, int>(1, 6));
  times.push_back(std::pair<int, int>(2, 7));
  times.push_back(std::pair<int, int>(3, 8));
  std::cout << "test1 get max user:" << MaxUser(times) << std::endl;
  times.push_back(std::pair<int, int>(3, 9));
  times.push_back(std::pair<int, int>(4, 10));
  times.push_back(std::pair<int, int>(5, 11));
  std::cout << "test2 get max user:" << MaxUser(times) << std::endl;

  times.push_back(std::pair<int, int>(6, 11));
  std::cout << "test2 get max user:" << MaxUser(times) << std::endl;
  return 0;
}
