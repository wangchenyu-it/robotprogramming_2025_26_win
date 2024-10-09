#include <iostream> // we will discover this later
using namespace std; // we will discover this later

// structs in c++ can hold methods
// in fact the only difference between a struct and a class
// is the default access policy
// for classes is private, for structs is public
// we start our journey with structs

struct A {
  int my_value;
  
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

int main(int argc, char** argv) {
  A a;
  a.my_value=7;
  a.addOne();
  a.print();
  a.addSomething(3);
  a.print();

  // as in C structs can be assigned.
  // assignment, by default copies the content of the
  // member variables.
  // this behavior can be overridden (see later int the examples)
  A b;
  b=a;
  b.addOne();
  cerr << "B:" << endl;
  b.print();
  cerr << "A:" << endl;
  a.print();
}

