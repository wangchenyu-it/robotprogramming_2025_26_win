#include <iostream>
#include <cstdlib>
#include <cstring>

template <typename T>
struct MyClass_ {

  struct MyInnerClass {
    static int myMethod();
  };
};


MyClass_<T> my_class;
MyClass_<T>::MyInnerClass my_inner_class;

MyClass_<T>::MyInnerClass::myMethod();



using namespace std;

template  <typename T>
struct CompareSmallerThan_{
  inline bool operator()(const T& a, const T& b) const {return a<b;}  
};

template  <typename T>
struct CompareGreaterThan_{
  inline bool operator()(const T& a, const T& b) const {return a>b;}  
};

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
             MyType* end) {
  MyType* b1=begin;
  MyType* b2=middle;
  Compare comp;
  while (b1<middle && b2<end){
    if (comp(*b1,*b2)) {
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

template <typename MyType, typename Compare>
void mySort(MyType* begin, MyType* end, MyType* buf=0) {
  Compare comp;
  int size=end-begin;
  if (size<2)
    return;
  bool destroy_buffer=false;
  if (! buf) {
    buf = new MyType[size];
    destroy_buffer=true;
  }
  int middle=size/2;
  mySort<MyType, Compare>(begin, begin+middle, buf);
  mySort<MyType, Compare>(begin+middle, end, buf);
  
  myMerge<MyType, Compare>(buf, begin, begin+middle, end);
  memcpy(begin, buf, sizeof(MyType)*size);
  if (destroy_buffer)
    delete [] buf;
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
  mySort<double, CompareGreaterThan_<double> >(v, v_end);
  printV(v, v_end);

}
