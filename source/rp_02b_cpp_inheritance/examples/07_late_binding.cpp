#include <iostream> // we will discover this later
#include <cassert>  // assert.h
#include <string>   // string from stl
using namespace std; // we will discover this later


// in c++ late binding applies only to the methods that are declared
// virtual!
// late binding might be expensive, so c++ restricts late binding
// only to those methods that really need it

// simple (non late binding example)
class A {
public:
  A(int value_):
    _value(value_){
  }
  void print() {
    cerr << "A: [" << this << "], value: " << _value << endl;
  }
protected:
  int _value;
};

class B: public A {
public:
  B(int value_):
    A(value_) {}
  void print(){
    cerr << "B: [" << this << "], value: " << _value << endl;
  }
};



// simple (late binding example)
class A_lb {
public:
  A_lb(int value_):
    _value(value_){
  }
  virtual void print() {
    cerr << "A_lb: [" << this << "], value: " << _value << endl;
  }
protected:
  int _value;
};

class B_lb: public A_lb {
public:
  B_lb(int value_):
    A_lb(value_) {}
  void print() override {
    cerr << "B_lb: [" << this << "], value: " << _value << endl;
  }
};


  
int main(int argc, char ** argv) {
  cerr <<  "no late binding" << endl;
  {
    B b(10);
    A& a_ref=b;
    b.print();
    a_ref.print();
  }

  cerr <<  "late binding" << endl;
  {          
    B_lb b(10);
    A_lb& a_ref(b);
    b.print();
    a_ref.print();

    // a is a copy of b
    A_lb a2(b);
    a2.print();
  }

  
}
