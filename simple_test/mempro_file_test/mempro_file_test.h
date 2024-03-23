#pragma once

class Test {
public:
  void TestA1() {
  }

  void TestA2() {
  }

  void TestA3() {
  }
};

class TestA1 {
public:
  TestA1() {
    a1 = 0;
  }

  int a1;
};

class TestA1A2 {
public:
  TestA1A2() {
    a1 = 0;
    a2 = 0;
  }

protected:
  int a1;
private:
  int a2;
};

class TestA3 {
public:
  TestA3(int a) {
    a1 = a;
  }
private:
  int a1;
};

template<typename T>
class TplTestA4 {
public:
  TplTestA4(T a) {
    a1 = a;
  }
private:
  T a1;
};

template<typename T1, typename T2>
class TplTestA5 {
public:
  TplTestA5(T1 a,T2 b) {
    a1 = a;
    a2 = b;
  }
private:
  T1 a1;
  T2 a2;
};
