#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include <memory>
#include <array>

using namespace std;


mutex zoomReport, studentReport, stuLock, rangeValuesLock;


struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};
vector<student> stu;
array<int, 3> myarray = {0, 0, 0};
// string email = "";


class consumer {
private:
  int _n = 1;
  string _email;
  static constexpr vector<student>* _stu = &stu;
  static constexpr array<int, 3>* _rangeValues = &myarray;
  //static shared_ptr<string> _emailHandle = make_shared<string>(email.c_str());
public:
  consumer(int n, string emailInput) : _n(n), _email(emailInput) {
    //*_emailHandle = _email;
  }
  //consumer() {}
  ~consumer() {}
  static void* threaded_pass(void* arg) {
    cout << _rangeValues->at(2) << "\n";
    //processing here
    for (int i = 1; i <= _rangeValues->at(0); i++) {
      //scan the stu
      if ( (_rangeValues->at(0)) * (_rangeValues->at(2)) + (_rangeValues->at(1)) == _stu->size() ) {
        //we are on the last thread
        //do the remaining searches equal to _rangeValues->at(1)
        for (int i = 1; i <= _rangeValues->at(1); i++) {
          cout << "this should print equivalent to rangeValues(1) times\n";
        }
      }
    }
    rangeValuesLock.unlock();
    //pthread_exit(0);
    return 0;
  }
  // static void* threaded_pass_wrapper(void* object){
  //       static_cast<consumer>(object).threaded_pass;
  //       return 0;
  // }
  void execute() {
    int baseRange = _stu->size()/_n;
    int remainderRange = _stu->size()%_n;
    *_rangeValues = {baseRange, remainderRange, 1};
    pthread_t threads[_n];
    for (int i = 1; i <= _n; i++) {
      rangeValuesLock.lock();
      pthread_create(&threads[i], NULL, &consumer::threaded_pass, NULL);
      _rangeValues->at(2) = i;
    }
    for (int i = 1; i <= _n; i++) {
      pthread_join(threads[i], NULL);
    }
    cout << _rangeValues->at(0) << _rangeValues->at(1) << _rangeValues->at(2) << "\n";
  }
};
