#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
using namespace std;

int sum = 0;
mutex mtx;
void *runner(void *param);

struct student {
    string email;
    string startTime;
    string endTime;
};

vector<student> stu;

int main(int argc, char *argv[]) {
    ifstream zoomreport;
    ofstream studentreport;

    string line;

    zoomreport.open("Zoomreport.txt");
    studentreport.open("studentreport.txt");

    while(getline(zoomreport, line)) {
        studentreport << line << '\n';
    }
    

    zoomreport.close();
    studentreport.close();

    //pthread_t tid;
    pthread_t ths[atoi(argv[1])];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    for(int t = 0; t != 5; t++) {
        pthread_create(&ths[t], &attr, runner, argv[1]);
    }

    for(int t = 0; t != 5; t++) {
        pthread_join(ths[t], NULL);
    }  



    if (sum != 5) {
        cout << sum << endl;
        return 0;
    }


    //printf("sum= %d\n", sum);

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

