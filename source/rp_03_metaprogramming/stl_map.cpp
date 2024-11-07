#include <map>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

/*
  the stl (standard template library) offers a collection of ready to use templated data structures,
  such as lists, vectors, maps and many others, and algorithms
  
  The sintax is designed to be symmetric, so that you can replace in your program a container with another
  
*/

using namespace std;

class MyElement {
public:
  // These initializations are done before the ctor is called
  // and are formally part of the constructors
  int a = 0; 
  float b = 0;
  MyElement(int a_=0, float b_=0): a(a_), b(b_){} // initializer list
  virtual void print();
  virtual ~MyElement() {}
};

void MyElement::print() {
  std::cerr << "MyElement| this: " << this << " a: " << a << " b: " << b << endl;
}

// this is a shared ptr, manages the memory. Handles multiple references.
// Beware to loops in the reference chain. The memory management is automated
// but not automatic
using MyElementPtr = std::shared_ptr<MyElement>;

class MyDerivedElement: public MyElement {
public:
  MyDerivedElement(int a_=0, float b_=0, double c_=0): MyElement(a_, b_), c(c_){} // initializer list
  double c  = 0;
  // safety check, tells that the method print overrides the print
  // of the derived class
  void print() override;
};
  

void MyDerivedElement::print() {
  std::cerr << "MyDerivedElement| this: " << this << " a: " << a << " b: " << b << " c: " << c << endl;
}


// template function that prints a container of elements
template <typename ContainerType>
void printMap(ContainerType& container) {
  for (typename ContainerType::iterator it=container.begin(); it!=container.end(); ++it){
    typename ContainerType::mapped_type& item = it->second;
    cerr << it->first << " ";
    item.print();
  }
}

// template function that prints a container of 
template <typename ContainerType>
void printPtrMap(ContainerType& container) {
  for (typename ContainerType::iterator it=container.begin(); it!=container.end(); ++it){
    typename ContainerType::mapped_type& item = it->second;
    cerr << it->first << " "; 
    if (item)
      item->print();
  }
}



int main() {
  using MyElementMap = std::map<std::string, MyElement>; // list of elements
  using MyDerivedElementMap = std::map<std::string, MyDerivedElement>; // list of elements

  MyElementMap map_1;
  map_1["gino"] = MyElement(1, 0.1);
  map_1["pino"] = MyElement(2, 0.2);
  map_1["ugo"]  = MyElement(3, 0.3);

  cerr << "map 1 copy" << endl;
  MyElementMap map_1_copy = map_1;
  printMap(map_1_copy);

  cerr << "map 2, derived"  << endl;
  MyDerivedElementMap map_2;
  map_2["amilcare"] = MyDerivedElement(1, 0.1, 0.01);
  map_2["eusebio"]  = MyDerivedElement(2, 0.2, 0.02);
  map_2["robofante"] = MyDerivedElement(3, 0.3, 0.03);
  printMap(map_2);

  // copy does not work, items different
  // MyElementMap map_2_as_a=map_2; // << this does not work
  // we need a manual copy
  MyElementMap map_2_as_a;
  for (const auto& it: map_2) {
    map_2[it.first] = it.second; // calls the copy ctor
  }
  cerr << "map 2 as a, derived"  << endl;
  printMap(map_2_as_a);

  // play with pointers

  using MyElementRawPtrMap = std::map<std::string, MyElement*>; // map of my_element_pointers


  // we declare a map of raw pointers, we can mix the suff
  MyElementRawPtrMap map_1_ptrs;
  map_1_ptrs["gino"] = new MyElement(1, 0.1);
  map_1_ptrs["pino"] = new MyElement(2, 0.2);
  map_1_ptrs["ugo"]  = new MyElement(3, 0.3);
  map_1_ptrs["amilcare"] = new MyDerivedElement(1, 0.1, 0.01);
  map_1_ptrs["eusebio"]  = new MyDerivedElement(2, 0.2, 0.02);
  map_1_ptrs["robofante"] = new MyDerivedElement(3, 0.3, 0.03);

  MyElementRawPtrMap map_1_ptrs_copy=map_1_ptrs;

  cerr << "map 1 ptrs"  << endl;
  printPtrMap(map_1_ptrs_copy);

  
  // the map is of raw pointers. We need to manually clear the memory
  for (auto& it: map_1_ptrs) {
    delete it.second; // manual delete
  }
  map_1_ptrs.clear();

  // we delete the pointers. Uncomment this to get a segfault
  //printPtr(map_1_ptrs_copy);


  //managed ptrs
  using MyElementSharedPtrMap =  std::map<std::string, MyElementPtr>;
  MyElementSharedPtrMap map_1_mptrs;
  map_1_mptrs["gino"].reset(new MyElement(1, 0.1));
  map_1_mptrs["pino"].reset(new MyElement(2, 0.2));
  map_1_mptrs["ugo"].reset(new MyElement(3, 0.3));
  map_1_mptrs["amilcare"].reset(new MyDerivedElement(1, 0.1, 0.01));
  map_1_mptrs["eusebio"].reset(new MyDerivedElement(2, 0.2, 0.02));
  map_1_mptrs["robofante"].reset(new MyDerivedElement(3, 0.3, 0.03));

  MyElementSharedPtrMap map_1_mptrs_copy=map_1_mptrs;

  cerr << "map 1 mptrs"  << endl;
  printPtrMap(map_1_mptrs_copy);
  
  // we delete the pointers. This will not segfault
  cerr << "map 1 mptrs, after delete"  << endl;
  printPtrMap(map_1_mptrs_copy);
  
}
