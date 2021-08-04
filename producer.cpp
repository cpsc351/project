#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARESIZE 1000

using namespace std;


int main(int argc, char *argv[]) {
  if (argc < 2) { cout << "Provide enough command line arguments"; }
  int shmid;
  key_t key;
  void* shm;                       // pointer to shared memory, for writing into
  key = 886699586;                          // private key for the shared memory
  shmid = shmget(key, SHARESIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
    cout << "error in shmget\n";            //unable to create the shared memory
    exit(1);
  }
  shm = shmat(shmid, (void*)0, 0);
  if (shm == (char*)-1) {
    cout << "error in shmat\n";    //unable to get a handle on the shared memory
    exit(1);
  }
  ifstream zoomreport;
  string line;
  zoomreport.open(argv[1]); //opens the text file specified by the command line argument
  int number_of_lines = 0;  //line count is needed for the construction of the array of strings
  while (getline(zoomreport, line)) {
    ++number_of_lines;
  }
  zoomreport.close();
  zoomreport.open(argv[1]);
  string newStudent[number_of_lines]; //create a meaningful array from the input
  // each element of the array will correspond to one line from the input file
  int i = 0;
  while(getline(zoomreport, line)) {
    newStudent[i] = line;
    newStudent[i].push_back('\n'); //for delimiting purposes when we retrieve from shared memory
    i++;
  }
  char* pusher;
  pusher = (char*)shm;
  for (int i = 0; i < number_of_lines; i++) {
    // create a cstring from each element of the array, then copy that cstring to shared memory
    char* cstr = new char[newStudent[i].length()];
    strcpy (cstr, newStudent[i].c_str());
    memcpy((void*)pusher, cstr, newStudent[i].length());
    pusher+=newStudent[i].length();
    delete[] cstr;
  }
  zoomreport.close();
  shmdt(shm);                                        //detach from shared memory
  return 0;
}
