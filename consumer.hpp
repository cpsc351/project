#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
//#include "producer.cpp"
using namespace std;

//mutex mtx;
mutex zoomReport, studentReport, stuLock;

//void* threaded_pass(void* argument);
struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};
vector<student> stu;

class consumer {
private:
  int _n = 1;
  string _email;
  vector<student>* _stu = &stu;
public:
  consumer(int n, string email) : _n(n), _email(email) {}
  //consumer() {}
  ~consumer() {}
  static void* threaded_pass(void* me) {
    int x = 1;
    cout << x;
    pthread_exit(0);
  }
  // static void* threaded_pass_wrapper(void* object){
  //       static_cast<consumer>(object).threaded_pass;
  //       return 0;
  // }
  void execute() {
    cout << _stu->size();
    pthread_t threads[_n];
    for (int i = 1; i <= _n; i++) {
      pthread_create(&threads[i], NULL, &consumer::threaded_pass, this);
    }
    for (int i = 1; i <= _n; i++) {
      pthread_join(threads[i], NULL);
    }
  }
};
