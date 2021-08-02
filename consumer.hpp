#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
using namespace std;

//mutex mtx;

class consumer {
private:
  int _n;
  string _email;
public:
  consumer(int n, string email) : _n(n), _email(email) {}
  consumer() {}
  ~consumer() {}

};
