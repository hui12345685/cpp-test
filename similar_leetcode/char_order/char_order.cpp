#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>

//��һ���ַ������ַ�����˳��(˳����order��������abcde....xyz)���ж���һ���ַ����ǲ��ǰ������е�
bool IsbyOrder(std::vector<std::string>& words, std::string& order) {
  if (words.size() <= 1){
    return true;
  }

  int idx = 0;
  std::unordered_map<char,int>ch_seq;
  for (auto ch: order){
    ch_seq.insert(std::pair<char, int>(ch,idx++));
  }

  //�����Ƚϰ�
  bool ret = true;
  std::string pre = words[0];
  for (int idx = 1; idx < words.size(); idx++) {
    std::string cur = words[idx];
    for (int jdx = 0; jdx < pre.size() || jdx < cur.size(); jdx++) {
      if (jdx < pre.size() && jdx < cur.size()){
        std::unordered_map<char, int>::iterator pit = ch_seq.find(pre[jdx]);
        std::unordered_map<char, int>::iterator cit = ch_seq.find(cur[jdx]);
        if (pit == ch_seq.end()) {//�Ƿ��ַ�
          return false;
        }
        if (cit == ch_seq.end()) {//�Ƿ��ַ�
          return false;
        }
        if (pit->second > cit->second) {//����������
          return false;
        }
        if (pit->second < cit->second){//������ֱ�ӱȽ���һ��
          break;
        }
      }
      //apple > app
      //app < apple
      if ((pre.size() > cur.size() && jdx >= cur.size())){//ǰ��Ķ���ȣ�cur����
        //��apple��appǰ���ʱ��
        ret = false;
      }
      if (!ret){
        break;
      }
    }
    pre = cur;
    if (!ret) {
      break;
    }
  }
  return ret;
}

//��һ���ַ������ǰ���˳������ģ�Ҫ��������е��ַ��ֵ���(�������ַ���˳��)
//���ַ�ʽ�Ľ��Ӧ�ò��ԣ�����
std::string test2(std::vector<std::string>& words) {
  std::string sret;
  if (words.empty()) {
    return sret;
  }
  std::list<std::pair<char, char>> greater;//�������е�С�ڹ�ϵ
  std::string pre = words[0];
  for (int idx = 1; idx < words.size(); idx++) {
    std::string cur = words[idx];
    int minlen = pre.size() < cur.size() ? pre.size(): cur.size();
    for (int jdx = 0; jdx < minlen; ++jdx) {
      if (cur[jdx] != pre[jdx]) {
        greater.push_back(std::pair<char, char>(pre[jdx], cur[jdx]));
        break;
      }
    }
    pre = cur;
  }
  std::list<char> ret;
  for (auto& ls:greater){
    if (ret.empty()){
      ret.emplace_back(ls.first);
      ret.emplace_back(ls.second);
      continue;
    }
    //a<b && b<c && b < d && c<d
    std::list<char>::iterator chit = ret.begin();
    for (; chit != ret.end(); chit++) {
      if (ls.first == *chit){
        ret.insert(++chit, ls.second);
        break;
      }
    }
  }
  for(auto& ch:ret){
    sret += ch;
  }
  return sret;
}

//��һ���ַ������ǰ���˳������ģ�Ҫ��������е��ַ��ֵ���(�������ַ���˳��)
/*std::list<char>*/std::string TopSortBfs(std::vector<std::string>& words) {
  std::list<char> str_ret;
  std::string sret;
  if (words.empty()) {
    //return str_ret;
    return sret;
  }
  std::list<std::pair<char, char>> greater;//�������е�С�ڹ�ϵ
  std::string pre = words[0];
  for (int idx = 1; idx < words.size(); idx++) {
    std::string cur = words[idx];
    int minlen = pre.size() < cur.size() ? pre.size() : cur.size();
    for (int jdx = 0; jdx < minlen; ++jdx) {
      if (cur[jdx] != pre[jdx]) {
        greater.push_back(std::pair<char, char>(pre[jdx], cur[jdx]));
        break;
      }
    }
    pre = cur;
  }

  std::unordered_map<char, int> in_cnt;//��ʾÿһ���ַ����������ַ�������
  std::unordered_map<char, std::list<char>> edges;//��ʾÿ���ַ�С�����ļ����ַ�
  for (auto& ls : greater) {
    auto it = in_cnt.find(ls.first);
    //��ʼ�����ڵ�����Ϊ0��
    if (it == in_cnt.end()) {
      in_cnt.insert(std::pair<char, int>(ls.first, 0));
    }

    auto it1 = in_cnt.find(ls.second);
    //��ʼ�����ڵ�����Ϊ��0��
    if (it1 == in_cnt.end()) {
      in_cnt.insert(std::pair<char, int>(ls.second, 1));
    }
    else {
      it1->second++;
    }

    //a<c
    auto eit = edges.find(ls.first);
    if (eit != edges.end()) {
      eit->second.push_back(ls.second);
    }
    else {
      std::list<char>tmp;
      tmp.push_back(ls.second);
      edges.insert(std::pair<char, std::list<char>>(ls.first, tmp));
    }
  }

  std::queue<char>que;
  for (auto& in:in_cnt){
    if (0 == in.second){
      que.push(in.first);
    }
  }
  while (!que.empty()){
    char ch = que.front();
    que.pop();
    in_cnt.erase(ch);//�ŵ����еļǵ�Ҫɾ����������ѭ���ˣ�������
    str_ret.push_back(ch);
    const auto& it = edges.find(ch);
    if (it != edges.end()){
      //����������
      for (const auto lsch:it->second){
        auto bit = in_cnt.find(lsch);
        if (bit != in_cnt.end()){
          bit->second--;
        }
      }
    }
    for (auto& in : in_cnt) {
      if (0 == in.second) {
        que.push(in.first);
      }
    }
  }

  for (auto& ch : str_ret) {
    sret += ch;
  }
  return sret;
  //return str_ret;
}

int main() {
  std::vector<std::string>vec{ "hello", "world" , "apple", "app"};
  std::string order("hlabcdefgijkmnopqrstuvwxyz");
  std::cout << "test:" << IsbyOrder(vec, order) << std::endl;//0

  std::vector<std::string>vec1{"hello","app", "apple", "appz", "world"};
  std::string order1("hlabcdefgijkmnopqrstuvwxyz");
  std::cout << "test1:" << IsbyOrder(vec1, order1) << std::endl;//1

  std::vector<std::string>vec2{ "word","world","row" };
  std::string order2("worldabcefghijkmnpqstuvxyz");
  std::cout << "test2:" << IsbyOrder(vec2, order2) << std::endl;//0

  std::vector<std::string>vec3{ "apple","app" };
  std::string order3("abcdefghijklmnopqrstuvwxyz");
  std::cout << "test3:" << IsbyOrder(vec3, order3) << std::endl;//0

  std::vector<std::string>vec4{ "app","apple" };
  std::string order4("abcdefghijklmnopqrstuvwxyz");
  std::cout << "test4:" << IsbyOrder(vec4, order4) << std::endl;//1

  std::vector<std::string>vec5{ "baa", "abcd", "abca", "cab", "cad" };
  std::cout << "test5:" << test2(vec5) << std::endl;// ['b', 'd', 'a', 'c']

  std::vector<std::string>vec6{ "caa", "aaa", "aab" };
  std::cout << "test6:" << test2(vec6) << std::endl;//['c', 'a', 'b']

  std::vector<std::string>vec7{ "baa", "abcd", "abca", "cab", "cad" };
  std::cout << "test7:" << TopSortBfs(vec7) << std::endl;// ['b', 'd', 'a', 'c']

  std::vector<std::string>vec8{ "caa", "aaa", "aab" };
  std::cout << "test8:" << TopSortBfs(vec8) << std::endl;//['c', 'a', 'b']
}
