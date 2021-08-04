#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <mutex>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARESIZE 1000

using namespace std;

mutex zoomReport;

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
  shm = shmat(shmid, (void*)0, 0);
  if (shm == (char*)-1) {
    cout << "error in shmat\n";
    exit(1);
  }
  ifstream zoomreport;
  string line;
  zoomReport.lock();
  zoomreport.open(argv[1]);
  int number_of_lines = 0;
  while (getline(zoomreport, line)) {
    ++number_of_lines;
  }
  zoomreport.close();
  zoomreport.open(argv[1]);
  string newStudent[number_of_lines];
  int i = 0;
  while(getline(zoomreport, line)) {
    newStudent[i] = line;
    newStudent[i].push_back('\n'); //for parsing purposes when we retrieve
    i++;
  }
  char* pusher;
  pusher = (char*)shm;
  for (int i = 0; i < number_of_lines; i++) {
    char* cstr = new char[newStudent[i].length()];
    strcpy (cstr, newStudent[i].c_str());
    memcpy((void*)pusher, cstr, newStudent[i].length());
    pusher+=newStudent[i].length();
    delete[] cstr;
  }
  zoomreport.close();
  zoomReport.unlock();
  shmdt(shm);
  return 0;
}
