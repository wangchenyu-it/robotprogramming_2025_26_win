#include <iostream>
#include "vec_f.h"

using namespace std;

int main() {
  VecF v1(3);
  for (int i=0; i<v1.dim; ++i)
    v1.at(i)=i;

  cerr << "v1: " << v1 << endl;

  VecF v2(v1);
  cerr << "v2: " << v2 << endl;

  VecF v3;
  v3=v2;
  cerr << "v3: " << v3 << endl;


  cerr << "sum: " << v1+v2 << endl;

  cerr << "diff: " << v1-v2 << endl;

  v1.at(2)+=4;
  cerr << v1+(v2*2.f) << endl;
}
