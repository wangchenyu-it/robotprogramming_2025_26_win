#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

/*
  the stl (standard template library) offers a collection of ready to use templated data structures,
  such as vectors, vectors, maps and many others, and algorithms
  
  The sintax is designed to be symmetric, so that you can replace in your program a container with another
  
*/

using namespace std;

class MyElement {
public:
  // These initializations are done before the ctor is called
  // and are formally part of the constructors
  int a = 0; 
  float b = 0;
  MyElement(int a_, float b_): a(a_), b(b_){} // initializer vector
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
  MyDerivedElement(int a_, float b_, double c_): MyElement(a_, b_), c(c_){} // initializer vector
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
  using MyElementVector = std::vector<MyElement>; // vector of elements
  using MyDerivedElementVector = std::vector<MyDerivedElement>; // vector of elements

  MyElementVector vector_1;
  vector_1.push_back(MyElement(1, 0.1));
  vector_1.push_back(MyElement(2, 0.2));
  vector_1.push_back(MyElement(3, 0.3));

  cerr << "vector 1 copy" << endl;
  MyElementVector vector_1_copy = vector_1;
  print(vector_1_copy);

  cerr << "vector 2, derived"  << endl;
  MyDerivedElementVector vector_2;
  vector_2.push_back(MyDerivedElement(1, 0.1, 0.01));
  vector_2.push_back(MyDerivedElement(2, 0.2, 0.02));
  vector_2.push_back(MyDerivedElement(3, 0.3, 0.03));
  print(vector_2);

  // copy does not work, items different
  // MyElementVector vector_2_as_a=vector_2; // << this does not work
  // we need a manual copy
  MyElementVector vector_2_as_a;
  for (const auto& i: vector_2) {
    vector_2_as_a.push_back(i); // calls the copy ctor
  }
  cerr << "vector 2 as a, derived"  << endl;
  print(vector_2_as_a);

  // play with pointers

  using MyElementRawPtrVector = std::vector<MyElement*>; // vector of my_element_pointers


  // we declare a vector of raw pointers, we can mix the suff
  MyElementRawPtrVector vector_1_ptrs;
  vector_1_ptrs.push_back(new MyElement(1, 0.1));
  vector_1_ptrs.push_back(new MyElement(2, 0.2));
  vector_1_ptrs.push_back(new MyElement(3, 0.3));
  vector_1_ptrs.push_back(new MyDerivedElement(4, 0.4, 0.04));
  vector_1_ptrs.push_back(new MyDerivedElement(5, 0.5, 0.05));
  vector_1_ptrs.push_back(new MyDerivedElement(6, 0.6, 0.06));

  MyElementRawPtrVector vector_1_ptrs_copy=vector_1_ptrs;

  cerr << "vector 1 ptrs"  << endl;
  printPtr(vector_1_ptrs_copy);

  
  // the vector is of raw pointers. We need to manually clear the memory
  for (const auto& i: vector_1_ptrs) {
    delete i; // manual delete
  }
  vector_1_ptrs.clear();

  // we delete the pointers. Uncomment this to get a segfault
  //printPtr(vector_1_ptrs_copy);


  //managed ptrs
  using MyElementSharedPtrVector =  std::vector<MyElementPtr>;
  MyElementSharedPtrVector vector_1_mptrs;
  vector_1_mptrs.push_back(MyElementPtr(new MyElement(1, 0.1)));
  vector_1_mptrs.push_back(MyElementPtr(new MyElement(2, 0.2)));
  vector_1_mptrs.push_back(MyElementPtr(new MyElement(3, 0.3)));
  vector_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(4, 0.4, 0.04)));
  vector_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(5, 0.5, 0.05)));
  vector_1_mptrs.push_back(MyElementPtr(new MyDerivedElement(6, 0.6, 0.06)));

  MyElementSharedPtrVector vector_1_mptrs_copy=vector_1_mptrs;

  cerr << "vector 1 mptrs"  << endl;
  printPtr(vector_1_mptrs_copy);

  vector_1_mptrs.clear();
  
  // we delete the pointers. This will not segfault
  cerr << "vector 1 mptrs, after delete"  << endl;
  printPtr(vector_1_mptrs_copy);

  /* look at it next week :)

  cerr << "vector 1 mptrs, after sort"  << endl;
  // we sort the vecto, from higher to lower
  std::sort(vector_1_mptrs_copy.begin(),
            vector_1_mptrs_copy.end(),
            [&](const MyElementPtr& first,
                const MyElementPtr& second) -> bool {
              return first->a > second->a;
            });
  printPtr(vector_1_mptrs_copy);
  */
}
