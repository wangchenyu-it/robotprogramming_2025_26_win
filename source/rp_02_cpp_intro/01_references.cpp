#include <iostream> // we will discover this later
using namespace std; // we will discover this later

int main(int argc, char** argv) {
  // declare an inr
  int a = 5;
  cerr << "A is: " << a << endl;

  // declare a reference to an int, and assign it to a
  // a reference is like an alias to a variable
  // references need always to be initialized on creation
  // think as a reference as a dereferenced pointer
  int &b=a;

  // references can be accessed as if they were of the same
  // type of the reference variable
  b+=1;

  // the above operation adds to the variable b one
  
  // since a and b are the same actual variable, the value of A will be changed
  cerr << "A is: " << a << endl;
  
}
