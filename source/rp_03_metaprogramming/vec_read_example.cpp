#include <iostream>
#include <fstream>
#include <list>
using namespace std;

template <typename ScalarP, int DimP> 
struct Vec_ {
  using Scalar=ScalarP;
  static constexpr int Dim=DimP;
  Scalar values[Dim];
  inline Scalar& operator [](int pos) { return values[pos];}
  inline const Scalar& operator [](int pos) const { return values[pos];}
};

template <typename ScalarP, int DimP>
ostream& operator << (ostream& os, const Vec_<ScalarP, DimP>& v) {
  for (int i=0; i<DimP; ++i)
    os << v[i] << " ";
  return os;
}

template <typename ScalarP, int DimP>
istream& operator >> (istream& is, Vec_<ScalarP, DimP>& v) {
  for (int i=0; i<DimP; ++i)
    is >> v[i];
  return is;
}


using Vec3f = Vec_<float, 3>;
using Vec3fList = list<Vec3f>;

int main(int argc, char** argv) {
  if (argc<2) {
    cerr << "no filename provided" << endl;
    return -1;
  }
  cerr << "loading file [" << argv[1] << "]" << endl;

  ifstream is(argv[1]);
  if (!is.good()) {
    cerr << "bad file" << endl;
    return -1;
  }

  Vec3fList l;
  while (is.good()) {
    Vec3f el;
    is >> el;
    if(is.good()) {
      l.push_back(el);
    }
  }

  for(const auto& v: l) {
    cout << v << endl;
  }
  
}
