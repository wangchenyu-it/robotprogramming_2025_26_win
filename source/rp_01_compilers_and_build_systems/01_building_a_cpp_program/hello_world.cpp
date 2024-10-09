#include <iostream>
#include <cmath>
#include <cstdio>
#include "other_file.h"

using namespace std;

int main(int argc, const char** argv) {
  cout << "hello world" << endl;
  for (int i=0; i<100; ++i) {
    printf("cose");
    cout << i << " " << sin(M_PI/100) << " " << iAmAFunctionInCamelCase((float) M_PI/100.f) << endl;
  }
}
  
