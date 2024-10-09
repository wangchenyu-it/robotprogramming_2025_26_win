#include <iostream> // we will discover this later
using namespace std; // we will discover this later

//Objects are by default allocated on the struct
// memory management is explicit (as in C)
// objects might have structures on the heap, that should be released upon destruction.
// you have already seen the constructors in java, the same happens in c++
// the destructors are fnuctions implicitly invoked by upon deletion of an object
// and you can override them to specialize the behavior of deletion


struct A {
  int my_value;

  
  
  // default constructors (overrides default one, if declared)
  A(){
    cerr << "A [" << this << "] default ctor" << endl;
    my_value=0;
  }

  A(int my_value) {
    this->my_value=my_value; // this is a pointer to the object itself
    // in this case the parameter my value would hide the member variable
    // but we can access it through this.
  }
  
  // copy ctor, invoked when
  // 1 .creating a new object from an old one
  //   A new_a(a); // or
  //   A new_a=a;
  //
  // 2. in the return statement of a function that returns an object
  //    of this type
  //
  //   A foo2() {
  //     A a;
  //     ...
  //     return a;
  //   }
  // 3. when calling a function that takes a parameter of type A
  //    by value
  //
  //   void foo1(A param) {
  //     ...
  //   }

  
  A(const A& other){
    cerr << "A [" << this << "] copy ctor" << endl;
    my_value=other.my_value;
  }

  // destructor: is a function called when an object os destroyed
  ~A() {
    cerr << "A [" << this << "] dtor" << endl;
  }
  void addOne() {
    ++my_value;
  }

  void addSomething(int sth) {
    my_value += sth;
  }
  
  void print() {
    cerr << "A::print() { " << my_value << "}" << endl;
  }

  int value() {return my_value;}
};

void foo1(A a){
  cerr << "foo1" << endl;
  a.addSomething(10);
  a.print();
}

A foo2(){
  cerr << "foo2" << endl;
  A a;
  a.addOne();
  a.print();
  return a;
}

int main(int argc, char** argv) {
  cerr << "create instance" << endl;
  A a;

  cerr << "assign on creation" << endl;
  A a1=a;

  cerr << "pass by value" << endl;
  foo1(a);

  a.print();
  
  cerr << "return from function" << endl;
  A a2=foo2();
  cerr << "address of a2: " << &a2;
  a2.print();
}
