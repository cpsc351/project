#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include "consumer.hpp"
using namespace std;

mutex mtx;

struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};
vector<student> stu;
void parseText(string line, vector<student>& stu);

int main(int argc, char *argv[]) {
  if (argc < 4) { cout << "Provide enough command line arguments"; }
  consumer threaded(stoi(argv[3]), argv[2]);
  ifstream zoomreport;
  ofstream studentreport;
  string line;

  zoomreport.open("Zoomreport.txt");
  studentreport.open("studentreport.txt");

  while(getline(zoomreport, line)) {
      parseText(line, stu);
  }
  return 0;
}
void parseText(string line, vector<student>& stu) {
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

    stu.push_back(studentStruct);
}
