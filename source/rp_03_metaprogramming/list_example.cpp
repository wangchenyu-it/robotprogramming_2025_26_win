#include <list>
#include <iostream>

using namespace std;

using IntList=list<int>; // list of integers;


int main() {

  IntList l;

  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_front(5);

  // use of keyword auto to determine the type of a value from the
  // return type of an expression
  for (auto it=l.begin(); it!=l.end(); ++it)
    cerr <<  *it << endl;
  auto l2=l; // deep copy, l1 and l are different objects
}
