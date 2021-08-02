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

class consumer {
private:
  int _n = 1;
  string _email;
  static constexpr vector<student>* _stu = &stu;
  static constexpr array<int, 3>* _rangeValues = &myarray;
  static string email;
public:
  consumer(int n, string emailInput) : _n(n), _email(emailInput) {
    email = emailInput;
  }
  ~consumer() {}

  static void* threaded_pass(void* arg) {
    cout << _rangeValues->at(2) << "\n";
    //processing here
    for (int i = 1; i <= _rangeValues->at(0); i++) {
      //scan the stu

      int maxIndex = (i) * (_rangeValues->at(0)) - 1; //inclusive
      int minIndex = (i - 1) * (_rangeValues->at(0));
      // for (int j = minIndex; j <= maxIndex; j++) {
      //   if (_stu->at(j).email == email) {
      //     int foundMinutes = stoi(_stu->at(j).endTime) - stoi(_stu->at(j).startTime);
      //     cout << foundMinutes << "\n";
      //   }
      // } //i think this is the glitch RIGHT HERE FIX MEEEEE PLEASE FIX ME IM an AWWFUL BUG
      if ( (_rangeValues->at(0)) * (_rangeValues->at(2)) + (_rangeValues->at(1)) == _stu->size() ) {
        //we are on the last thread
        //do the remaining searches equal to _rangeValues->at(1)
        for (int k = 1; k <= _rangeValues->at(1); k++) {
          int remainderSearchIndexStart = _rangeValues->at(0) * _rangeValues->at(2);
          int remainderSearchIndexEnd = (_stu->size()) - 1;
          if (_stu->at(remainderSearchIndexStart + k - 1).email == email) {
            int foundMinutes = stoi(_stu->at(k).endTime) - stoi(_stu->at(k).startTime);
            cout << foundMinutes << "\n";
          }
          cout << "this should print equivalent to rangeValues(1) times\n";
        }
      }
    }
    rangeValuesLock.unlock();
    //pthread_exit(0);
    return 0;
  }
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
string consumer::email = "";
