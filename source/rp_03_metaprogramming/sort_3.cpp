#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

template <typename MyType>
void printV(MyType* v, MyType* v_end) {
  while (v<v_end) {
    cout << *v << " ";
    ++v;
  }
  cout << endl;
}

template <typename MyType, typename Compare>
void myMerge(MyType* dest,
             MyType* begin,
             MyType* middle,
             MyType* end,
             const Compare& comp) {
  MyType* b1=begin;
  MyType* b2=middle;
  while (b1<middle && b2<end){
    if (comp(*b1, *b2)) {
      *dest=*b1;
      ++ b1;
    } else {
      *dest=*b2;
      ++ b2;
    }
    ++dest;

  }
};

template <typename MyType, typename Compare>
void mySort(MyType* begin, MyType* end, const Compare& comp, MyType* buf=0) {
  int size=end-begin;
  if (size<2)
    return;
  bool destroy_buffer=false;
  if (! buf) {
    buf = new MyType[size];
    destroy_buffer=true;
  }
  int middle=size/2;
  mySort(begin, begin+middle, comp, buf);
  mySort(begin+middle, end, comp, buf);
  
  myMerge(buf, begin, begin+middle, end, comp);
  memcpy(begin, buf, sizeof(MyType)*size);
  if (destroy_buffer)
    delete [] buf;
}

struct compareGreaterThan {
  bool operator() (const double& v1, const double& v2) const {
    return v1>v2;
  }
};

struct compareLessThan {
  bool operator() (const double& v1, const double& v2) const {
    return v1<v2;
  }
};

int main(int argc, char** argv) {
  int size=atoi(argv[1]);
  double *v=new double[atoi(argv[1])];
  double *v_end=v+size;
  for (int i=0; i<size; ++i) {
    v[i]=drand48();
  }

  cout << "begin: " << endl;
  printV(v, v_end);

  cout << "sorted: " << endl;
  mySort(v, v_end, compareGreaterThan());
  printV(v, v_end);

}
