#include <iostream>

struct VecF {
   
  int dim; // guess what
  float* v; // elements to the data

  float get(int pos) {
    return v[pos];
  }

  void set(int i,float f) {
    v[i]=f;
  }

  VecF() {
    dim=0;
    v=nullptr;
  }

  VecF(int dim_){
    dim=dim_; 
    v=new float[dim];
  }
  
  VecF(const VecF& other) {
    dim=0; v=0;
    if (! other.dim)
      return;

    dim=other.dim;
    v=new float[dim];
    
    for (int i=0; i<dim; ++i)
      v[i]=other.v[i];
  }

  
  ~VecF() {
    if (dim)
      delete [] v;
  }

  VecF& operator =(const VecF& other) {
    if (dim)
      delete[] v;
    
    dim=0; v=0;
    if (! other.dim)
      return *this;

    dim=other.dim;
    v=new float[dim];

    for (int i=0; i<dim; ++i)
      v[i]=other.v[i];
    return *this;
  }
  
};

int main() {
  VecF v5(5);
  v5.set(0,0.1);
  v5.set(1,0.2);
  VecF v7(v5); //(copy ctor)
  VecF v8=v5;  //(copy ctor)
  v8=v7;       // op=   
}
