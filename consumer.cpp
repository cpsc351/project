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
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARESIZE 1000

using namespace std;

mutex studentReport, stuLock, rangeValuesLock;
struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};
student* _stu;
array<int, 4> myarray = {0, 0, 0, 0};
int foundMinutes = 0;
ofstream studentreport;
int _n = 1;
string _email;
static constexpr array<int, 4>* _rangeValues = &myarray;
static constexpr int* _foundMinutes = &foundMinutes;
string email;


int readTime(string hours) {
  string minutes = hours;
  hours.erase(2, 3);
  minutes.erase(0, 3);
  cout << hours << ":" << minutes << "\n";
  int hoursInt = stoi(hours);
  int minutesInt = stoi(minutes);
  hoursInt = hoursInt * 60;
  minutesInt += hoursInt;
  cout << minutesInt << " minutes\n";
  return minutesInt;
}
void getEmailFromArrOfStrings(string line) {
  string delimiter = " ";
  string arr[5];
  int i = 0;
  stringstream ssin(line);

  while(ssin.good() && i < 5) {
    ssin >> arr[i];
    ++i;
  }
  cout << arr[0] << "\n";
}
void* threaded_pass(void* arg) {
  int maxIndex = 0;
  int minIndex = 0;
  maxIndex = (_rangeValues->at(3)) * (_rangeValues->at(0));
  minIndex = maxIndex - (_rangeValues->at(0));
  maxIndex--;
  int lastThreadChecker = _rangeValues->at(3);
  cout << lastThreadChecker;
  cout << " is the current thread compared to total of " << _rangeValues->at(2) << "\n";
  if (lastThreadChecker == _rangeValues->at(2)) {
    maxIndex += _rangeValues->at(1);
  }
  for (int j = minIndex; j <= maxIndex; j++) {
      if (_stu[j].email == email) {
        //studentReport.lock();
        studentReport.try_lock();
        int endTime1 = readTime(_stu[j].endTime);
        int startTime1 = readTime(_stu[j].startTime);
        *_foundMinutes += endTime1 - startTime1;
        studentreport << _stu[j].email << " " << _stu[j].endDate << " " << endTime1 - startTime1 << " minutes.\n";
        studentReport.unlock();
      }
      cout << "just searched vect index = " << j << "\n";
      cout << *_foundMinutes << " total minutes found.\n";
    }
    (_rangeValues->at(3))++;
    rangeValuesLock.unlock();
    return 0;
}
int main(int argc, char* argv[]) {
  int shmid;
  key_t key = 886699586;
  void* shm;
  shmid = shmget(key, SHARESIZE, 0666);
  if (shmid < 0) {
    cout << "error in shmget\n";
    exit(1);
  }
  shm = shmat(shmid, NULL, 0);
  if (shm == (char*)-1) {
    cout << "error in shmat\n";
    exit(1);
  }
  //memcpy(shm, _stu, 6);                                  // hardcoded 6
  //(student*)copy = (student*) shm;
  //student* shmCopy = (student*)shm;
  char* shmCopy;
  // for (int i = 0; i < 7; i++) {                            //hardcoded 7
  //   memcpy(&_stu[i], shmCopy, 64);
  //   cout << _stu[i].email << " " << &_stu[i] << "\n";
  //   shmCopy++;
  // }
  //string memoryReader = "";
  //int memoryReader[6] = {0,0,0,0,0,0};

  // int x = 0;
  // for (shmCopy = (char*)shm; *shmCopy != EOF; shmCopy++) {
  //   //memoryReader.push_back(*shmCopy);
  //   // char y = *shmCopy;
  //   // memoryReader[x] = atoi(y);
  //   cout << *shmCopy;
  //   if (*shmCopy == '\n') {
  //       x++;
  //   }
  // }

  for (shmCopy = (char*)shm; *shmCopy != EOF; shmCopy++) {
    printf("%c", *shmCopy);
  }
  cout << " end.\n";

  //for (x = 0; x < 6; x++) { cout << memoryReader[x]; }
  //cout << x << '\n';
  //cout << memoryReader;
  // for (int i = 0; i < number_of_lines; i++) {
  //   getEmailFromArrOfStrings(newStudent[i]);
  // }

  //cout << (int*)shm;
  //cout << _stu->endDate << " result\n";
  //int arraySize = *_stu.size();
  email = argv[1];
  _n = stoi(argv[2]);

  // int baseRange = arraySize/_n;
  // int remainderRange = arraySize%_n;
  int baseRange = 6/_n;                                //hardcoded 6
  int remainderRange = 6%_n;                           //hardcoded 6

  *_rangeValues = {baseRange, remainderRange, _n, 1};
  // pthread_t threads[_n];
  // studentreport.open("studentreport.txt");
  //
  // for (int i = 1; i <= _n; i++) {
  //   rangeValuesLock.lock();
  //   pthread_create(&threads[i], NULL, threaded_pass, NULL);
  // }
  // for (int i = 1; i <= _n; i++) {
  //   pthread_join(threads[i], NULL);
  // }
  // cout << *_foundMinutes << " minutes found.\n";
  // studentreport << *_foundMinutes << " total minutes found.\n";
  // studentreport.close();
  shmdt(shm);
  return 0;
}
