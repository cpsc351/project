#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include <array>
//#include "producer.cpp"
using namespace std;

//mutex mtx;
mutex zoomReport, studentReport, stuLock, tresLock;

//void* threaded_pass(void* argument);
struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};
vector<student> stu;
array<int, 3> myarray = {0, 0, 0};

class consumer {
private:
  int _n = 1;
  string _email;
  static constexpr vector<student>* _stu = &stu;
  static constexpr array<int, 3>* _tres = &myarray;
public:
  consumer(int n, string email) : _n(n), _email(email) {}
  //consumer() {}
  ~consumer() {}
  static void* threaded_pass(void* arg) {
    //size_t x = _stu->size();
    //cout << x << "\n";
    //cout << _tres->at(0) << _tres->at(1) << _tres->at(2) << "\n";
    //tresLock.lock();
    cout << _tres->at(2) << "\n";
    tresLock.unlock();
    //pthread_exit(0);
    return 0;
  }
  // static void* threaded_pass_wrapper(void* object){
  //       static_cast<consumer>(object).threaded_pass;
  //       return 0;
  // }
  void execute() {
    //cout << _stu->size();
    int baseRange = _stu->size()/_n;
    int remainderRange = _stu->size()%_n;
    *_tres = {baseRange, remainderRange, 1};
    pthread_t threads[_n];
    for (int i = 1; i <= _n; i++) {
      tresLock.lock();
      pthread_create(&threads[i], NULL, &consumer::threaded_pass, NULL);
      //tresLock.unlock();
      _tres->at(2) = i;
    }
    for (int i = 1; i <= _n; i++) {
      pthread_join(threads[i], NULL);
    }
    cout << _tres->at(0) << _tres->at(1) << _tres->at(2) << "\n";
  }
};
