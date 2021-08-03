#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <mutex>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARESIZE 1000

//#include "consumer.cpp"
using namespace std;

mutex zoomReport;


struct student {
    string startDate;
    string email;
    string startTime;
    string endDate;
    string endTime;
};
void parseText(string line, student stu[], int i);

int main(int argc, char *argv[]) {
  if (argc < 2) { cout << "Provide enough command line arguments"; }
  int shmid;
  key_t key;
  void* shm;
  key = 886699586;
  shmid = shmget(key, SHARESIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
    cout << "error in shmget\n";
    exit(1);
  }
  shm = shmat(shmid, NULL, 0);
  if (shm == (char*)-1) {
    cout << "error in shmat\n";
    exit(1);
  }

  // consumer threaded(stoi(argv[3]), argv[2]);
  ifstream zoomreport;
  //ofstream studentreport;
  string line;
  zoomReport.lock();
  zoomreport.open(argv[1]);
  //studentReport.lock();
  // studentreport.open("studentreport.txt");
  //stuLock.lock();
  int number_of_lines = 0;
  while (getline(zoomreport, line)) {
    ++number_of_lines;
  }
  zoomreport.close();
  zoomreport.open(argv[1]);
  student* stu = new student[number_of_lines];
  int i = 0;
  while(getline(zoomreport, line)) {
      parseText(line, stu, i);
      i++;
  }
  memcpy(shm, stu, number_of_lines);

  //stuLock.unlock();
  zoomreport.close();
  zoomReport.unlock();
  // stuLock.lock();
  // for(size_t i = 0; i < stu.size(); i++)
  //   studentreport << stu[i].email << stu[i].startDate << stu[i].startTime << stu[i].endDate << stu[i].endTime << '\n';
  // stuLock.unlock();
  // studentreport.close();
  // studentReport.unlock();
  //threaded.execute();
  return 0;
}
void parseText(string line, student stu[], int index) {
    string delimiter = " ";
    student studentStruct;
    string arr[5];
    int i = 0;
    stringstream ssin(line);
    while(ssin.good() && i < 5) {
        ssin >> arr[i];
        ++i;
    }

    studentStruct.email = arr[0];
    studentStruct.startDate = arr[1];
    studentStruct.startTime = arr[2];
    studentStruct.endDate = arr[3];
    studentStruct.endTime = arr[4];

    //cout << "email: " << studentStruct.email << endl << "start date: " << studentStruct.startDate << endl << "start time: " << studentStruct.startTime << endl << "end date: " << studentStruct.endDate << endl << "end time: " << studentStruct.endTime << endl << endl;

    stu[index] = (studentStruct);
}
