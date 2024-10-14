#include "vec_f.h"

#include <iostream>
#include <assert.h>


using namespace std;

// read/write access to element at pos
float& VecF::at(int pos){
  return v[pos];
}
  
// read access to element at pos
// const after () means that the method does not modify the invoking object
const float& VecF::at(int pos) const {
  return v[pos];
}

// default ctor
VecF::VecF():
  dim(0),
  v(nullptr) {
}

// ctor with dim
VecF::VecF(int dim_):
  dim(dim_),
  v(nullptr){
  if (dim)
    v=new float [dim];
}

// copy ctor
VecF::VecF(const VecF& other):
 VecF(other.dim)
{
  for (int i=0; i<dim; ++i)
    v[i]=other.v[i];
}

// dtor
VecF::~VecF() {
  if(dim)
    delete [] v;
}

// op =, deep copy
VecF& VecF::operator =(const VecF& other) {
  if(dim!=other.dim) {
    delete [] v;
    v=0;
    dim=other.dim;
    if (! dim)
      return *this;
    v=new float[dim];
  }
  for (int i=0; i<dim; ++i)
    v[i]=other.v[i];
  return *this;
}

// returns the sum this + other
VecF VecF::operator +(const VecF& other) const {
  assert(other.dim==dim && "dim mismatch");
  VecF returned(*this);
  for (int i=0; i<dim; ++i)
    returned.v[i]+=other.v[i];
  return returned;
}

VecF VecF::operator -(const VecF& other) const {
  assert(other.dim==dim && "dim mismatch");
  VecF returned(*this);
  // TODO: fillme
  return returned;
}


// returns this*f
VecF VecF::operator *(float f) const {
  VecF returned(*this);
  // TODO: fillme
  return returned;
}


// returns the dot product (vecs should have the same size);
float VecF::operator *(const VecF& other) const {
  float acc=0.f;
  // TODO: fillme
  return acc;
}

ostream& operator <<(ostream& os, const VecF& v) {
  os << "{ ptr: " << &v << " dim:" << v.dim << " vals: [ ";
  for (int i=0; i<v.dim; ++i)
    os << v.at(i) <<  " ";
  os << "] }";
  return os;
}
