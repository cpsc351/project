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
    for (int j = minIndex; j <= maxIndex; j++) {
        if (_stu->at(j).email == email) {
          int foundMinutes = stoi(_stu->at(j).endTime) - stoi(_stu->at(j).startTime);
        }
        cout << "just searched vect index = " << j << "\n";
      }
      (_rangeValues->at(3))++;
      rangeValuesLock.unlock();
      return 0;
  }
  void execute() {
    int baseRange = _stu->size()/_n;
    int remainderRange = _stu->size()%_n;
    *_rangeValues = {baseRange, remainderRange, _n, 1};
    pthread_t threads[_n];
    for (int i = 1; i <= _n; i++) {
      rangeValuesLock.lock();
      pthread_create(&threads[i], NULL, &consumer::threaded_pass, NULL);
    }
    for (int i = 1; i <= _n; i++) {
      pthread_join(threads[i], NULL);
    }
    cout << _rangeValues->at(0) << _rangeValues->at(1) << _rangeValues->at(2) << _rangeValues->at(3) << " rangeValues\n";
  }
};
string consumer::email = "";
