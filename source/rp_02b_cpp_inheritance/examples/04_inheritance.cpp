#include <iostream> // we will discover this later
using namespace std; // we will discover this later

// classes (and structs) can be extended

struct A {
  int my_value;
  A(int v) {
    cerr << "A [" << this << "]" << " ctor" << endl;
    my_value=v;
  }

  ~A(){
    cerr << "A [" << this << "]" << " dtor" << endl;
  }

  void print() {
    cerr << "A::print() [" << this << "]: my_value=" << my_value << endl;
  }
};


struct B: public A {
  int my_other_value;
  
  // use the initializer list between the parameter list and the {
  // it would not be possible to initialize
  // objects whose default ctor has been deleted
  B():        
    A(0),  // base class initialization
    my_other_value(-1)
  {
    cerr << "B [ " << this << "] ctor " << endl;
  }

  ~B(){
    cerr << "B [" << this << "]" << " dtor" << endl;
  }

  void print() {
    A::print(); // call method of the base class using scoping <classname>::<method_or_variable>
    cerr << "B::print() [" << this << "]: my_other_value=" << my_other_value << endl;
  }

};

int main(int argc, char** argv) {
  cerr << endl;
  cerr << "create A " << endl;
  A a(3);
  a.print();
  
  cerr << "create B " << endl;
  B b;
  b.print();

  // legal assignment a is base class of b, only common member vars are copied
  A a1=b; 
  a1.print();
}

