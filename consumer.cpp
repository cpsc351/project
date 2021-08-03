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
//vector<student> stu;
student* _stu;

array<int, 4> myarray = {0, 0, 0, 0};
int foundMinutes = 0;
ofstream studentreport;

int _n = 1;
string _email;
//student* _stu[] = &stu;
static constexpr array<int, 4>* _rangeValues = &myarray;
static constexpr int* _foundMinutes = &foundMinutes;
//static string email;
string email;




int readTime(string time1) {
  // time.erase(2, 1);
  // int result = stoi(time);
  // string endTime2 = endTime1;
  string time2 = time1;
  time1.erase(2, 3);
  time2.erase(0, 3);
  cout << time1 << ":" << time2 << "\n";
  int time11 = stoi(time1);
  int time22 = stoi(time2);
  time11 = time11 * 60;
  time22 += time11;
  cout << time22 << " minutes\n";
  return time22;
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
        // *_foundMinutes += readTime(_stu[j].endTime) - readTime(_stu[j].startTime);
        //studentReport.lock();
        studentReport.try_lock();
        int endTime1 = readTime(_stu[j].endTime);
        // string endTime2 = endTime1;
        // endTime1.erase(2, 2);
        // endTime2.erase(0, 2);
        // endTime1 = endTime1 * 60;
        // endTime2 += endTime1;
        int startTime1 = readTime(_stu[j].startTime);
        // int startTime2 = endTime1;
        // startTime1.erase(2, 2);
        // startTime2.erase(0, 2);
        // startTime1 = startTime1 * 60;
        // startTime2 += startTime1;
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
  //cout << "hello world";
  int shmid;
  //cout << "next line works";
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
  //cout << "next line fails";
  //memcpy(shm, _stu, 6);                                  // hardcoded 6
  //(student*)copy = (student*) shm;
  student* shmCopy = (student*)shm;
  for (int i = 0; i < 7; i++) {                            //hardcoded 7
    memcpy(&_stu[i], shmCopy, 64);
    cout << _stu[i].email << " " << &_stu[i] << "\n";
    shmCopy++;
  }
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
  pthread_t threads[_n];
  studentreport.open("studentreport.txt");

  for (int i = 1; i <= _n; i++) {
    rangeValuesLock.lock();
    pthread_create(&threads[i], NULL, threaded_pass, NULL);
  }
  for (int i = 1; i <= _n; i++) {
    pthread_join(threads[i], NULL);
  }
  //cout << _rangeValues->at(0) << _rangeValues->at(1) << _rangeValues->at(2) << _rangeValues->at(3) << " rangeValues\n";
  cout << *_foundMinutes << " minutes found.\n";
  studentreport << *_foundMinutes << " total minutes found.\n";
  studentreport.close();
  shmdt(shm);
  return 0;
}
