#include "btree.h"


TreeNodeInt::TreeNodeInt(int value_,
                         TreeNodeInt* left_,
                         TreeNodeInt* right_):
  value(value_),
  left(left_),
  right(right_){}

TreeNodeInt::~TreeNodeInt(){
  if (left)
    delete left;
  if (right)
    delete right;
}

TreeNodeInt* TreeNodeInt::find(int value_) {
  if (value==value_)
    return this;
  if (value_<value) {
    // TODO: fill here
    // if there is a left child, continue the search in the left, otherwise return null
  }
  if (value_>value) {
    // if there is a right child, continue the search in the right, otherwise return null
    if (! right)
      return 0;
    return right->find(value_);
  }
  return 0;
}
  
bool TreeNodeInt::add(int value_) {
  if (value==value_)
    return false;
  if (value_<value) {
    if (! left) {
      left=new TreeNodeInt(value_);
      return true;
    }
    return left->add(value_);
  }
  if (value_>value) {
    // TODO: fill here
  }
  return false;
}

std::ostream& operator <<(std::ostream& os, const TreeNodeInt& node) {
  if (node.left)
    os << *(node.left);
  os << node.value << " ";
  if (node.right)
    os << *(node.right);
  return os;
}
