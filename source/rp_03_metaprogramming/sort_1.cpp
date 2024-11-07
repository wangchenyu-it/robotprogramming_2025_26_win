#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

using MyType = double;

void printV(MyType* v, MyType* v_end) {
  while (v<v_end) {
    cout << *v << " ";
    ++v;
  }
  cout << endl;
}

void myMerge(MyType* dest,
             MyType* begin,
             MyType* middle,
             MyType* end) {
  MyType* b1=begin;
  MyType* b2=middle;
  while (b1<middle && b2<end){
    if (*b1<*b2) {
      *dest=*b1;
      ++ b1;
    } else {
      *dest=*b2;
      ++ b2;
    }
    ++dest;
  }
  while (b1<middle){
    *dest=*b1;
    ++b1;
    ++dest;
  }
  while (b2<end){
    *dest=*b2;
    ++b2;
    ++dest;
  }
};

void mySort(MyType* begin, MyType* end, MyType* buf=0) {
  int size=end-begin;
  if (size<2)
    return;
  bool destroy_buffer=false;
  if (! buf) {
    buf = new MyType[size];
    destroy_buffer=true;
  }
  int middle=size/2;
  mySort(begin, begin+middle, buf);
  mySort(begin+middle, end, buf);
  
  myMerge(buf, begin, begin+middle, end);
  memcpy(begin, buf, sizeof(MyType)*size);
  if (destroy_buffer)
    delete [] buf;
};

int main(int argc, char** argv) {
  int size=atoi(argv[1]);
  MyType *v=new MyType[atoi(argv[1])];
  MyType *v_end=v+size;
  for (int i=0; i<size; ++i) {
    v[i]=drand48();
  }

  cout << "begin: " << endl;
  printV(v, v_end);

  cout << "sorted: " << endl;
  mySort(v, v_end);
  printV(v, v_end);

}
