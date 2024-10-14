#pragma once
#include "vec_f.h"

struct MatF {
  int rows, cols, dimension; // guess what
  float* v; // elements to the data

  // default ctor
  MatF();

  // ctor with dim
  MatF(int rows, int cols);

  // copy ctor
  MatF(const MatF& other);

  // dtor
  ~MatF();

  // fills the matrix with f;
  void fill(float f);

  // fills with random values;
  void randFill();
  
  // read/write access to element at pos
  float& at(int pos);
  
  // read access to element at pos
  // const after () means that the method does not modify the invoking object
  const float& at(int pos) const;

  // read/write access to element at pos
  float& at(int r, int c);
  
  // read access to element at pos
  // const after () means that the method does not modify the invoking object
  const float& at(int r, int c) const;

  // op =, deep copy
  MatF& operator =(const MatF& other);

  // returns the sum this + other
  MatF operator +(const MatF& other) const;

  // returns the difference this - other
  MatF operator -(const MatF& other) const;

  // returns this * f
  MatF  operator *(float f) const;

  // returns this * other
  VecF  operator *(const VecF& other) const;

  // returns this * other
  MatF  operator *(const MatF& other) const;

  MatF transpose() const;
};

std::ostream& operator <<(std::ostream& os, const MatF& v);
