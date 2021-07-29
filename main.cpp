#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
using namespace std;

int sum = 0;
mutex mtx;
void *runner(void *param);
void parseText(string line);

struct student {
    string email;
    string startDate;
    string startTime;
    string endDate;
    string endTime;
};

vector<student> stu;

int main(int argc, char *argv[]) {
    //Parse text file data
    ifstream zoomreport;
    ofstream studentreport;
    string line;

    zoomreport.open("Zoomreport.txt");
    studentreport.open("studentreport.txt");

    while(getline(zoomreport, line)) {
        parseText(line);
    }

    studentreport << "test";
    
    zoomreport.close();
    studentreport.close();

    //create threads and search vector
    //pthread_t tid;
    pthread_t ths[atoi(argv[1])];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    for(int t = 0; t != atoi(argv[1]); t++) {
        pthread_create(&ths[t], &attr, runner, argv[1]);
    }

    for(int t = 0; t != atoi(argv[1]); t++) {
        pthread_join(ths[t], NULL);
    }  

    if (sum != atoi(argv[1])) {
        cout << sum << endl;
        return 0;
    }


    //printf("sum= %d\n", sum);

}

void parseText(string line) {
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

    cout << "email: " << studentStruct.email << endl << "start date: " << studentStruct.startDate << endl << "start time: " << studentStruct.startTime << endl << "end date: " << studentStruct.endDate << endl << "end time: " << studentStruct.endTime << endl << endl;

    stu.push_back(studentStruct);
}

void *runner(void *param) {
    //int uppper;
    mtx.lock();

    cout << sum;

    //for (int i = 1; i <= upper; i++) {
    sum++;
    //}

    mtx.unlock();
    pthread_exit(0);
}

