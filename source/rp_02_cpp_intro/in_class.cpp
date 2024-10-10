#include <iostream>

using namespace std;

struct VecF {
  float* data;
  int dim;

  VecF() {
    cout << "I am the default constructor of" << this << endl;
    data=0;
    dim=0;
  }

  VecF(int dim){
    cout << "I am the fancy constructor of" << this << endl;
    data=0;
    dim=0;
    resize(dim);
  }

  VecF(const VecF& src):
    VecF(src.dim)
  {
    cout << "I am the copy ctor " << this << " src:" << &src << endl;
    for (int i=0; i<dim; ++i) {
      data[i]=src.data[i];
    }
  }

  ~VecF() {
    cout << "I am the DESTRUCTOR of" << this <<  endl;
    if (data)
      delete [] data;
  }


  VecF operator+(const VecF& src ) {
    cout << "I am + op" << this << " src: "<< &src << endl;
    VecF other(*this);
    for (int i=0; i<dim; ++i)
      other.data[i]+=src.data[i];
    return other;
  }

  VecF& operator=(const VecF& src ) {
    cout << "I am assignment op" << this << " src: "<< &src << endl;
    resize(src.dim);
    for (int i=0; i<dim; ++i) {
      data[i]=src.data[i];
    }
    return *this;
  }

  void resize(int new_dim) {
    if (new_dim==dim)
      return;

    if (new_dim==0) {
      delete [] data;
      data=0;
      dim=0;
      return;
    }

    float * new_data=new float[new_dim];

    int min_dim = new_dim;
    if (min_dim>dim)
      min_dim=dim;

    for (int i=0; i<min_dim; ++i)
      new_data[i]=data[i];

    delete[] data;
    data=new_data;
    dim=new_dim;
  }
              
  void print() {
    cout << "[";
    
    for (int i=0; i<dim; ++i) {
      cout << data[i] << " ";
    }

    cout << "]" << endl;
  }
  
};


int main() {
  VecF v(5);
  for (int i=0; i<5; ++i)
    v.data[i]=i;
  v.print();

  VecF v2=v;
  v2.print();

  VecF v3(3);
  v3=v2;

  (v3+v2).print();
  
  // VecF v0(); // 
  // VecF v(5);
  // v[4]=10;
  // VecF v2=v;
  // v=v2+v;

  
}
