#pragma once
#include <iostream>

struct TreeNodeInt {
  int value;
  TreeNodeInt * left, *right;
  TreeNodeInt(int value=0,
              TreeNodeInt* left=0,
              TreeNodeInt* right=0);

  ~TreeNodeInt();

  TreeNodeInt* find(int value);
  
  bool add(int value);
};

std::ostream& operator <<(std::ostream& os, const TreeNodeInt& node);

  
