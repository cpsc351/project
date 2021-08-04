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

int arraySize = 0;
array<int, 4> myarray = {0, 0, 0, 0};
int foundMinutes = 0;
ofstream studentreport;
int _n = 1;
string _email;
static constexpr array<int, 4>* _rangeValues = &myarray;
static constexpr int* _foundMinutes = &foundMinutes;
string email;

int readTime(string time);
string getEndDate(string line);
string getEmail(string line);
string getStartTime(string line);
string getEndTime(string line);

void* threaded_pass(void* arg) {
  int maxIndex = 0;
  int minIndex = 0;
  string* _stu = (string*) arg;
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
      if (getEmail(_stu[j]) == email) {
        //studentReport.lock();
        studentReport.try_lock();
        int endTime1 = readTime(getEndTime(_stu[j]));
        int startTime1 = readTime(getStartTime(_stu[j]));
        *_foundMinutes += endTime1 - startTime1;
        studentreport << getEmail(_stu[j]) << " " << getEndDate(_stu[j]) << " " << endTime1 - startTime1 << " minutes.\n";
        studentReport.unlock();
      }
      cout << "just searched array index = " << j << "\n";
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
  char* shmCopy;
  string memoryReader = "";
  int index = 0;
  for (shmCopy = (char*)shm; *shmCopy != EOF; shmCopy++) {                    //works on hello world basic
    //printf("%c", *shmCopy);                                        //works on hello world basic
    memoryReader.push_back(*shmCopy);
    if (*shmCopy == '\n') {
      index++;
    }
  }                                                           //works on hello world basic
  string inputArray[index];
  stringstream memoryRead(memoryReader);
  string line;
  int y = 0;
  while (getline(memoryRead, line)) {
    inputArray[y] = line;
    y++;
  }
  string (*_stu)[index];
  _stu = &inputArray;
  arraySize = index;
  email = argv[1];
  _n = stoi(argv[2]);
  int baseRange = arraySize/_n;
  int remainderRange = arraySize%_n;
  *_rangeValues = {baseRange, remainderRange, _n, 1};
  pthread_t threads[_n];
  studentreport.open("studentreport.txt");

  for (int i = 1; i <= _n; i++) {
    rangeValuesLock.lock();
    pthread_create(&threads[i], NULL, threaded_pass, _stu);
  }
  for (int i = 1; i <= _n; i++) {
    pthread_join(threads[i], NULL);
  }
  cout << *_foundMinutes << " minutes found.\n";
  studentreport << *_foundMinutes << " total minutes found.\n";
  studentreport.close();
  shmdt(shm);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}

int readTime(string hours) {
  string minutes = hours;
  hours.erase(2, 3);
  minutes.erase(0, 3);
  int hoursInt = stoi(hours);
  int minutesInt = stoi(minutes);
  hoursInt = hoursInt * 60;
  minutesInt += hoursInt;
  return minutesInt;
}
string getEmail(string line) {
  string delimiter = " ";
  string arr[5];
  int i = 0;
  stringstream ssin(line);
  while(ssin.good() && i < 5) {
    ssin >> arr[i];
    ++i;
  }
  return arr[0];
}
string getStartTime(string line) {
  string delimiter = " ";
  string arr[5];
  int i = 0;
  stringstream ssin(line);
  while(ssin.good() && i < 5) {
    ssin >> arr[i];
    ++i;
  }
  return arr[2];
}
string getEndTime(string line) {
  string delimiter = " ";
  string arr[5];
  int i = 0;
  stringstream ssin(line);
  while(ssin.good() && i < 5) {
    ssin >> arr[i];
    ++i;
  }
  return arr[4];
}
string getEndDate(string line) {
  string delimiter = " ";
  string arr[5];
  int i = 0;
  stringstream ssin(line);
  while(ssin.good() && i < 5) {
    ssin >> arr[i];
    ++i;
  }
  return arr[3];
}
