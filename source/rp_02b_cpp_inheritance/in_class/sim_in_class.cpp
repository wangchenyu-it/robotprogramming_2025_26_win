#include <iostream>
using namespace std;

using Scalar=float;

struct Vec2f {
  static constexpr int Dim=2;
  Scalar values[Dim];
  inline Vec2f& operator+=(const Vec2f& src) {
    for (int i=0; i<Dim; ++i)
      values[i]+=src.values[i];
    return *this;
  };
  inline Vec2f& operator-=(const Vec2f& src) {
    for (int i=0; i<Dim; ++i)
      values[i]-=src.values[i];
    return *this;
  };
  
  inline void fill(const Scalar v=Scalar(0)) {
    for (int i=0; i<Dim; ++i)
      values[i]=v;
  };
  
  inline Scalar dot(const Vec2f& src) const {
    Scalar acc=Scalar(0);
    for (int i=0; i<Dim; ++i)
      acc += values[i]*src.values[i];
    return acc;
  };

  inline Vec2f operator+(const Vec2f& other) const {
    Vec2f v=*this;
    v+=other;
    return v;
  }

  inline Vec2f operator-(const Vec2f& other) const {
    Vec2f v=*this;
    v-=other;
    return v;
  }
};

  
ostream& operator << (ostream& os, const Vec2f& src){
  os << "V2 [" <<&src << "] [ ";
  for (int i =0; i<Vec2f::Dim; ++i) {
    os << src.values[i] << " ";
  }
  os << " ]";
  return os;
}

struct Rotation2f {
  static constexpr int Dim=2;
  Scalar R[Dim][Dim];
  void setIdentity();

  //returns the inverse (transposed)
  inline Rotation2f inverse() const {
    //TODO
  }
  
  // multiplies two matrices
  inline Rotation2f operator*(const Rotation2f & other) const {
    //TODO
  }

  // computes the angle
  Scalar toAngle() const {
    return atan2(R[1][0],R[0][0]);
  }
  
  void setAngle(Scalar angle) {
    Scalar s=sin(angle), c=cos(angle);
    R={ {c, -s},
        {s,  c}
    }; // initializer list
  }

  // returns R*src;
  Vec2f operator*(const Vec2f& src) const{
    //TODO
  }

  const Rotation2f& transposeInPlace() {
    for (int r=0; r<Dim; ++r) {
      for (int c=r+1; c<Dim; ++c){
        std::swap(R[r][c],R[c][r]);
      }
    }
    return *this;
  }
};


struct Isometry2f {
  Vec2f t;
  Rotation2f R;
  Isometry2f(){}
  Isometry2f(Scalar x, Scalar y, Scalar theta){
    // todo;
  }

  Isometry2f(const Vec2f& translation,
             const Rotation2f& rotation){
    // todo;
  }

  inline void setIdentity(){
    // todo
  }

  inline Isometry2f operator*(const Isometry2f& src) const {
    //todo
  }

  inline Isometry2f inverse() const {
    // todo
  }

  inline Vec2f operator*(const Vec2f& src) const {
    // todo
  }
  
}

int main (int argc, char** argv) {
  Vec2f v1;
  v1.fill();
  cout << v1;
  Vec2f v2=v1;
  cout << v2;
  v2.fill(0.1);
  cout << v2;
  cout << (v1-=v2);

  // todo: write the rest to test the above functions
  // hint: comment the todoes and address them one by one
  //       after each is done add a lil test

  // at the end uncomment the following
  /*
  Isometry2f iso(0.1,0, 0.01);
  Isometry2f pose;
  pose.setIdentity();
  Isometry2f point;
  point.values[0]=0;
  point.values[1]=0.1;
  
  for (int i=0; i<1000000; ++i) {
    pose=pose*iso;
  }
  */
}
