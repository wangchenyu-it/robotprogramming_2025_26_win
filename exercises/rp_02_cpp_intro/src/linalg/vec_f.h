#pragma once
#include <iostream>

struct VecF {
  int dim; // guess what
  float* v; // elements to the data

  // read/write access to element at pos
  float& at(int pos);
  
  // read access to element at pos
  // const after () means that the method does not modify the invoking object
  const float& at(int pos) const;

  // default ctor
  VecF();

  // ctor with dim
  VecF(int dim_);

  // copy ctor
  VecF(const VecF& other);

  // dtor
  ~VecF();

  // op =, deep copy
  VecF& operator =(const VecF& other);

  // returns the sum this + other
  VecF operator +(const VecF& other) const;

  // returns the difference this - other
  VecF operator -(const VecF& other) const;

  // returns this * f
  VecF  operator *(float f) const;

  // returns the dot product (vecs should have the same size);
  float operator *( const VecF& other) const;
};

std::ostream& operator <<(std::ostream& os, const VecF& v);
