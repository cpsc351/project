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
class consumer {
private:
  int _n = 1;
  string _email;
  vector<student>& _stu;
public:
  consumer(int n, string email, vector<student>& stu) : _n(n), _email(email), _stu(stu) {}
  //consumer() {}
  ~consumer() {}
  // void* threaded_pass(void* me) {
  //   size_t x = _stu.size();
  //   cout << x;
  //   pthread_exit(0);
  // }
  // static void* threaded_pass_wrapper(consumer* object){
  //       static_cast<consumer>(object).threaded_pass;
  //       return 0;
  // }
  void execute() {
    cout << _stu.size();
    // pthread_t threads[_n];
    // for (int i = 0; i <= _n; i++) {
    //   pthread_create(&threads[i], NULL, threaded_pass_wrapper, this);
    // }
    // for (int i = 0; i <= _n; i++) {
    //   pthread_join(threads[i], NULL);
    //}
  }
};
