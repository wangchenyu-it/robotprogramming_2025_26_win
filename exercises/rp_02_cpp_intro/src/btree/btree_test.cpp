#include <iostream>
#include "btree.h"

using namespace std;

int main() {
  TreeNodeInt* n=new TreeNodeInt(5);
  n->add(10);
  n->add(1);
  n->add(2);
  n->add(4);
  cerr << "ok here" << endl;
  cout << *n << endl;

  cerr << "address of obj 4: " << n->find(4) << endl;
}
