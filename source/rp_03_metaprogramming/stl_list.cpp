#include <list>
#include <algorithm>
#include <iostream>
#include <memory>

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
  MyElement(int a_, float b_): a(a_), b(b_){} // initializer list
  virtual void print();
  virtual ~MyElement() {}
};

void MyElement::print() {
  std::cerr << "MyElement| this: " << this << " a: " << a << " b: " << b << endl;
}

// this is a shared ptr, manages the memory. Handles multiple references.
// Beware to loops in the reference chain. The memory management is automated
// but not automatic
using MyElementPtr = shared_ptr<MyElement>;

class MyDerivedElement: public MyElement {
public:
  MyDerivedElement(int a_, float b_, double c_): MyElement(a_, b_), c(c_){} // initializer list
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
void print(ContainerType& container) {
  for (typename ContainerType::iterator it=container.begin(); it!=container.end(); ++it){
    typename ContainerType::value_type& item = *it;
    item.print();
  }
}

// template function that prints a container of 
template <typename ContainerType>
void printPtr(ContainerType& container) {
  for (typename ContainerType::iterator it=container.begin(); it!=container.end(); ++it){
    typename ContainerType::value_type& item = *it;
    if (item)
      item->print();
  }
}



int main() {
  using MyElementList = std::list<MyElement>; // list of elements
  using MyDerivedElementList = std::list<MyDerivedElement>; // list of elements

  MyElementList list_1;
  list_1.push_back(MyElement(1, 0.1));
  list_1.push_back(MyElement(2, 0.2));
  list_1.push_back(MyElement(3, 0.3));

  cerr << "list 1 copy" << endl;
  MyElementList list_1_copy = list_1;
  print(list_1_copy);

  cerr << "list 2, derived"  << endl;
  MyDerivedElementList list_2;
  list_2.push_back(MyDerivedElement(1, 0.1, 0.01));
  list_2.push_back(MyDerivedElement(2, 0.2, 0.02));
  list_2.push_back(MyDerivedElement(3, 0.3, 0.03));
  print(list_2);

  // copy does not work, items different
  // MyElementList list_2_as_a=list_2; // << this does not work
  // we need a manual copy
  MyElementList list_2_as_a;
  for (auto& i: list_2) {
    list_2_as_a.push_back(i); // calls the copy ctor
  }
  cerr << "list 2 as a, derived"  << endl;
  print(list_2_as_a);

  // play with pointers

  using MyElementRawPtrList = std::list<MyElement*>; // list of my_element_pointers


  // we declare a list of raw pointers, we can mix the suff
  MyElementRawPtrList list_1_ptrs;
  list_1_ptrs.push_back(new MyElement(1, 0.1));
  list_1_ptrs.push_back(new MyElement(2, 0.2));
  list_1_ptrs.push_back(new MyElement(3, 0.3));
  list_1_ptrs.push_back(new MyDerivedElement(4, 0.4, 0.04));
  list_1_ptrs.push_back(new MyDerivedElement(5, 0.5, 0.05));
  list_1_ptrs.push_back(new MyDerivedElement(6, 0.6, 0.06));

  MyElementRawPtrList list_1_ptrs_copy=list_1_ptrs;

  cerr << "list 1 ptrs"  << endl;
  printPtr(list_1_ptrs_copy);

  
  // the list is of raw pointers. We need to manually clear the memory
  for (const auto& i: list_1_ptrs) {
    delete i; // manual delete
  }
  list_1_ptrs.clear();

  // we delete the pointers. Uncomment this to get a segfault
  // printPtr(list_1_ptrs_copy);


  //managed ptrs
  using MyElementSharedPtrList =  std::list<MyElementPtr>;
  MyElementSharedPtrList list_1_mptrs;
  list_1_mptrs.push_back(MyElementPtr(new MyElement(1, 0.1)));
  list_1_mptrs.push_back(MyElementPtr(new MyElement(2, 0.2)));
  list_1_mptrs.push_back(MyElementPtr(new MyElement(3, 0.3)));
  list_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(4, 0.4, 0.04)));
  list_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(5, 0.5, 0.05)));
  list_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(6, 0.6, 0.06)));

  MyElementSharedPtrList list_1_mptrs_copy=list_1_mptrs;

  cerr << "list 1 mptrs"  << endl;
  printPtr(list_1_mptrs_copy);

  list_1_mptrs.clear();
  
  // we delete the pointers. This will not segfault
  cerr << "list 1 mptrs, after delete"  << endl;
  printPtr(list_1_mptrs_copy);

  //aaaaargh list is not a random access container, does not support sorting
  // cerr << "list 1 mptrs, after sort"  << endl;
  // // we sort the vecto, from higher to lower
  // std::sort(list_1_mptrs_copy.begin(),
  //           list_1_mptrs_copy.end(),
  //           [&](const MyElementPtr& first,
  //               const MyElementPtr& second) -> bool {
  //             return first->a > second->a;
  //           });
  // printPtr(list_1_mptrs_copy);

  
}
