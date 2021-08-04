#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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

mutex studentReport, rangeValuesLock;

array<int, 4> myarray = {0, 0, 0, 0};
int arraySize = 0;
int foundMinutes = 0;
string email;
ofstream studentreport;
int _n = 1;                                         // default value of 1 thread
static constexpr int* _foundMinutes = &foundMinutes;
//_rangeValues is used by each thread for the search algorithm and it has 4 values:
//    the first value is the number of elements that each thread is responsible for searching through
//    the second value is the number of elements that are unnacounted for during search
//              these elements are searched through by the last thread
//    the third value is the number of threads specified by the command line arguments
//    the fourth value changes, and represents the current thread index
static constexpr array<int, 4>* _rangeValues = &myarray;

//helper function that takes military time (20:56) and converts into minutes since midnight
int readTime(string time);
//various date specific helper functions for extracting data from a line of the input text
string getEndDate(string line);
string getEmail(string line);
string getStartTime(string line);
string getEndTime(string line);

//this function is executed by each thread.  the search algorithm searches an equal
//amount of indices in each thread; the remaining indices are searched by the last thread.
//the search is looking for email fields that match the email provided by command line arguments.
//when a matching email is found, that email's matching session details are written to the report.
//writing to the report is a critical section managed by a mutex.
void* threaded_pass(void* arg) {
  //this block determines which indices this thread is responsible for
  int maxIndex = 0;
  int minIndex = 0;
  string* _stu = (string*) arg;
  maxIndex = (_rangeValues->at(3)) * (_rangeValues->at(0));
  minIndex = maxIndex - (_rangeValues->at(0));
  maxIndex--;
  int lastThreadChecker = _rangeValues->at(3);
  if (lastThreadChecker == _rangeValues->at(2)) {
    maxIndex += _rangeValues->at(1); //the last thread has extra searches sometimes
  }
  //this block does the actual searching
  for (int j = minIndex; j <= maxIndex; j++) {
      if (getEmail(_stu[j]) == email) {
        studentReport.try_lock();
        int endTime1 = readTime(getEndTime(_stu[j]));
        int startTime1 = readTime(getStartTime(_stu[j]));
        *_foundMinutes += endTime1 - startTime1;
        studentreport << getEmail(_stu[j]) << " " << getEndDate(_stu[j]) << " " << endTime1 - startTime1 << " minutes.\n";
        studentReport.unlock();
      }
    }
  (_rangeValues->at(3))++;
  rangeValuesLock.unlock();
  return 0;
}

int main(int argc, char* argv[]) {
  int shmid;
  key_t key = 886699586;                    // private key for the shared memory
  void* shm;                       // pointer to shared memory, for writing into
  shmid = shmget(key, SHARESIZE, 0666);
  if (shmid < 0) {
    cout << "error in shmget\n";            //unable to create the shared memory
    exit(1);
  }
  shm = shmat(shmid, NULL, 0);
  if (shm == (char*)-1) {
    cout << "error in shmat\n";    //unable to get a handle on the shared memory
    exit(1);
  }
  //reads the contents of shared memory and places it into a string (memoryReader)
  char* shmCopy;
  string memoryReader = "";
  int index = 0;
  for (shmCopy = (char*)shm; *shmCopy != EOF; shmCopy++) {
    memoryReader.push_back(*shmCopy);
    if (*shmCopy == '\n') {
      index++;
    }
  }
  // create a meaningful array from the input
  // each element of the array will correspond to one line from the input file
  string inputArray[index];
  stringstream memoryRead(memoryReader);
  string line;
  int y = 0;
  while (getline(memoryRead, line)) {
    inputArray[y] = line;
    y++;
  }
  string (*_stu)[index]; //we need a pointer handle to the array for the threads to accept it as a parameter
  _stu = &inputArray;
  arraySize = index;
  email = argv[1];
  _n = stoi(argv[2]);
  // initialize _rangeValues for the threads to use when they conduct their searches
  int baseRange = arraySize/_n;
  int remainderRange = arraySize%_n;
  *_rangeValues = {baseRange, remainderRange, _n, 1};
  pthread_t threads[_n];
  studentreport.open("studentreport.txt");
  // create n threads as specified by command line arguements
  for (int i = 1; i <= _n; i++) {
    rangeValuesLock.lock();
    pthread_create(&threads[i], NULL, threaded_pass, _stu);
  }
  // join the threads after they finish their searches
  for (int i = 1; i <= _n; i++) {
    pthread_join(threads[i], NULL);
  }
  // prints a total count of minutes found to the output device and to the report
  cout << *_foundMinutes << " total minutes found.\n";
  studentreport << *_foundMinutes << " total minutes found.\n";
  studentreport.close();
  shmdt(shm);                                        //detach from shared memory
  shmctl(shmid, IPC_RMID, NULL); //shared memory is no longer needed, so destroys shared memory
  return 0;
}

int readTime(string hours) {                    //input is military time (20:56)
  string minutes = hours;
  hours.erase(2, 3);                   // remove the ":" and everything after it
  minutes.erase(0, 3);                // remove the ":" and everything before it
  int hoursInt = stoi(hours);
  int minutesInt = stoi(minutes);
  hoursInt = hoursInt * 60;
  minutesInt += hoursInt;
  return minutesInt;
}

// the following functions are data specific.
// they won't work if the input text file does not meet the required format:
//      email, start date, start time, end date, end time
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
