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

//REGULAR STRUCT
struct student {
    string startDate;
    string email;
    string startTime;
    string endDate;
    string endTime;
};

//REGULAR PARSE FUNCTION
void parseText(string line, student stu[], int i);

//*NEW* EMAIL EXTRACTION FUNCTION FROM ARR OF STRINGS
void getEmailFromArrOfStrings(string line);

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

  //NORMAL STUDENT STRUCT VARIABLE
  student* stu = new student[number_of_lines];

  //*NEW* CREATE ARRAY OF STRINGS OF FIXED SIZE SINCE VECTOR CANT BE STORED IN 
  //MEMORY WITHOUT BOOST LIBRARY
  string newStudent[number_of_lines];

  int i = 0;
  while(getline(zoomreport, line)) {
    //*NEW* PUSH TO ARRAY OF STRINGS
    newStudent[i] = line;

    //REGULAR CODE
    parseText(line, stu, i);
    i++;
  }

  //*NEW* EXTRACT THE EMAIL FOR EACH LINE FROM THE SAMPLE DATA
  for (int i = 0; i < number_of_lines; i++) {
    getEmailFromArrOfStrings(newStudent[i]);
  }
  

  //put it in shared memory here
  //student* copy = (student*) shm;
  for (int i = 0; i < number_of_lines; i++) {
    memcpy(shm, &(stu[i]), 64);
    cout << stu[i].email << " " << &stu[i] << "\n";
  }
  //memcpy(shm, stu, number_of_lines);

  zoomreport.close();
  zoomReport.unlock();
  shmdt(shm);

  //todo implement safely:
  //shmctl(shmid, IPC_RMID, NULL);
  return 0;
}

//REGULAR PARSE FUNCTION
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
    stu[index] = (studentStruct);
}


//*NEW* FUNCTION TO BE USED IN CONSUMER AFTER DATA IS RETREIVED 
//FROM SHARED MEM,ONLY PRINTS FOR TESTING PURPOSES INSTEAD OF RETURNING
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
