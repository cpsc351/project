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
array<int, 4> myarray = {0, 0, 0, 0};

class consumer {
private:
  int _n = 1;
  string _email;
  static constexpr vector<student>* _stu = &stu;
  static constexpr array<int, 4>* _rangeValues = &myarray;
  static string email;
public:
  consumer(int n, string emailInput) : _n(n), _email(emailInput) {
    email = emailInput;
  }
  ~consumer() {}

  static void* threaded_pass(void* arg) {
    //cout << _rangeValues->at(2) << "\n";
    //processing here
    // int maxSize = _stu->size();
    // maxSize--;
    int maxIndex = 0;
    int minIndex = 0;
    maxIndex = (_rangeValues->at(3)) * (_rangeValues->at(0));
    minIndex = maxIndex - (_rangeValues->at(0));
    maxIndex--;
    int lastThreadChecker = _rangeValues->at(3);
    
    cout << lastThreadChecker;
    cout << "compared to " << _rangeValues->at(2) << "\n";
    if (lastThreadChecker == _rangeValues->at(2)) {

      maxIndex += _rangeValues->at(1);

    }
    // int currentIndex = 0;
    // currentIndex = () * (_rangeValues->at(0));
    //for (int i = 1; i <= _rangeValues->at(0); i++) {
      //scan the stu
    for (int j = minIndex; j <= maxIndex; j++) {

      // maxIndex = (i) * (_rangeValues->at(0));
      // minIndex = (i - 1) * (_rangeValues->at(0));
      // if (i * _rangeValues->at(0) == _stu.size()) {
      //   maxIndex
      // }
      //maxIndex--; //inclusive
      // at 0 corresponts to how many elements we must search
      // at 1 corresponds to the elemnts left after n equal searches (remainder)
      // at 2 corresponds to the number of threads and searches to do

        if (_stu->at(j).email == email) {
          int foundMinutes = stoi(_stu->at(j).endTime) - stoi(_stu->at(j).startTime);
          //cout << foundMinutes << "\n";
        }
        cout << "just searched vect index = " << j << "\n";
      }
      // if ((_rangeValues->at(3) - 1) == _rangeValues->at(2)) {
      //   //last thread
      //   for (int k = maxIndex; k <= (maxSize - 1); k++) {
      //     if (_stu->at(k).email == email) {
      //       int foundMinutes = stoi(_stu->at(k).endTime) - stoi(_stu->at(k).startTime);
      //       //cout << foundMinutes << "\n";
      //       cout << "k = " << k << "\n";
      //     }
      //   }
      // }
      (_rangeValues->at(3))++;
      rangeValuesLock.unlock();
      //pthread_exit(0);
      return 0;
        //     int remainderSearchIndexStart = _rangeValues->at(0) * _rangeValues->at(2);
        //     int remainderSearchIndexEnd = (_stu->size()) - 1;
        //     if (_stu->at(remainderSearchIndexStart + k - 1).email == email) {
        //       int foundMinutes = stoi(_stu->at(k).endTime) - stoi(_stu->at(k).startTime);
        //       //cout << foundMinutes << "\n";
        //     }
        //     cout << "this should print equivalent to rangeValues(1) times\n";
        //   }

      // if (i == 1) { minIndex = i * (_rangeValues->at(0)); }
      // else { minIndex = (i - 1) * (_rangeValues->at(0)); }

      // cout << maxIndex << " " << minIndex << _rangeValues->at(3) << "\n";
      // for (int j = minIndex; j <= maxIndex; j++) {
      //   if (_stu->at(j).email == email) {
      //     int foundMinutes = stoi(_stu->at(j).endTime) - stoi(_stu->at(j).startTime);
      //     //cout << foundMinutes << "\n";
      //   }
      // } //i think this is the glitch RIGHT HERE FIX MEEEEE PLEASE FIX ME IM an AWWFUL BUG
      // if ( (_rangeValues->at(0)) * (_rangeValues->at(2)) + (_rangeValues->at(1)) == _stu->size() ) {
      //   //we are on the last thread
      //   //do the remaining searches equal to _rangeValues->at(1)
      //   for (int k = 1; k <= _rangeValues->at(1); k++) {
      //     int remainderSearchIndexStart = _rangeValues->at(0) * _rangeValues->at(2);
      //     int remainderSearchIndexEnd = (_stu->size()) - 1;
      //     if (_stu->at(remainderSearchIndexStart + k - 1).email == email) {
      //       int foundMinutes = stoi(_stu->at(k).endTime) - stoi(_stu->at(k).startTime);
      //       //cout << foundMinutes << "\n";
      //     }
      //     cout << "this should print equivalent to rangeValues(1) times\n";
      //   }
      //}


  }
  void execute() {
    int baseRange = _stu->size()/_n;
    int remainderRange = _stu->size()%_n;
    *_rangeValues = {baseRange, remainderRange, _n, 1};
    pthread_t threads[_n];
    for (int i = 1; i <= _n; i++) {
      rangeValuesLock.lock();
      pthread_create(&threads[i], NULL, &consumer::threaded_pass, NULL);
      //_rangeValues->at(2) = i;
    }
    for (int i = 1; i <= _n; i++) {
      pthread_join(threads[i], NULL);
    }
    cout << _rangeValues->at(0) << _rangeValues->at(1) << _rangeValues->at(2) << _rangeValues->at(3) << "\n";
  }
};
string consumer::email = "";
