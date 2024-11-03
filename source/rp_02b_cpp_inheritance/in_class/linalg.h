#pragma once
#include <iostream>
#include <cmath>
using namespace std;

using Scalar=float;

struct Vec2f {
  static constexpr int Dim=2;
  Scalar values[Dim];
  Vec2f(const Scalar& x, const Scalar& y){
    values[0]=x;
    values[1]=y;
  }

  Vec2f() {}

  inline Scalar& x() { return values[0];} 
  inline Scalar& y() { return values[1];}

  inline const Scalar& x() const { return values[0];} 
  inline const Scalar& y() const { return values[1];}

  inline Vec2f& operator+=(const Vec2f& src) {
    for (int i=0; i<Dim; ++i)
      values[i]+=src.values[i];
    return *this;
  };

  inline Vec2f operator-() const {
    Vec2f ret;
    for (int i=0; i<Dim; ++i)
      ret.values[i] = -values[i];
    return ret;
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
  
  Scalar norm() const {
    return sqrt(dot(*this));
  }
  
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

  inline Vec2f& operator*=(const Scalar& s) {
    for (int r=0; r<Dim; ++r)
      values[r]*=s;
    return *this;
  }

  inline Vec2f operator*(const Scalar& s) const {
    Vec2f v=*this;
    return v*=s;
  }
};

  
inline ostream& operator << (ostream& os, const Vec2f& src){
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

  Rotation2f() {}
  
  Rotation2f(const Scalar angle) {
    setAngle(angle);
  }
  
  void setIdentity() {
    for (int r=0; r<Dim; ++r)
      for (int c=0; c<Dim; ++c)
        R[r][c] = (Scalar)(r==c);
  }

  inline const Rotation2f& transposeInPlace() {
    for (int r=0; r<Dim; ++r) {
      for (int c=r+1; c<Dim; ++c){
        Scalar aux=R[r][c];
        R[r][c]=R[c][r];
        R[c][r]=aux;
        //std::swap(R[r][c],R[c][r]);
      }
    }
    return *this;
  }

  
  //returns the inverse (transposed)
  inline Rotation2f inverse() const {
    Rotation2f ret=*this;
    ret.transposeInPlace();
    return ret;
  }
  
  // multiplies two matrices
  inline Rotation2f operator*(const Rotation2f & other) const {
    Rotation2f ret;
    for (int r=0; r<Dim; ++r)
      for (int c=0; c<Dim; ++c)
        ret.R[r][c]=0;
    
    for (int r=0; r<Dim; ++r)
      for (int c=0; c<Dim; ++c)
        for (int k=0; k<Dim; ++k)
          ret.R[r][c]+=R[r][k]*other.R[k][c];
    return ret;
  }

  // computes the angle
  Scalar angle() const {
    return atan2(R[1][0],R[0][0]);
  }
  
  void setAngle(Scalar angle) {
    Scalar s=sin(angle), c=cos(angle);
    R[0][0]=c; R[0][1]=-s;
    R[1][0]=s; R[1][1]= c;
    // initializer list
  }

  // returns R*src;
  Vec2f operator*(const Vec2f& src) const{
    Vec2f ret;
    ret.fill(0.f);
    for (int r=0; r<Dim; ++r)
      for (int c=0; c<Dim; ++c)
        ret.values[r]+=R[r][c]*src.values[c];
    return ret;
  }

  Rotation2f scale(float s) const {
    Rotation2f ret(*this);
    for (int r=0; r<Dim; ++r)
      for (int c=0; c<Dim; ++c)
        ret.R[r][c]*=s;
    return ret;
  }
};

inline ostream& operator << (ostream& os, const Rotation2f& src){
  os << "R2 [" <<&src << "]" << endl;
    
  for (int r =0; r<Rotation2f::Dim; ++r) {
    for (int c =0; c<Rotation2f::Dim; ++c) {
      os << src.R[r][c] << " ";
    }
    os << endl;
  }
  return os;
}


struct Isometry2f {
  Vec2f t;
  Rotation2f R;
  Isometry2f(){}

  Isometry2f(Scalar x, Scalar y, Scalar theta):
    t(x,y),
    R(theta){}

    
  Isometry2f(const Vec2f& translation,
             const Rotation2f& rotation):
    t(translation),
    R(rotation){}

  inline void setIdentity(){
    t.fill(0);
    R.setIdentity();
  }

  inline Isometry2f operator*(const Isometry2f& src) const {
    Isometry2f ret;
    ret.R=R*src.R;
    ret.t=R*src.t+t;
    return ret;
  }

  inline Isometry2f inverse() const {
    Isometry2f ret;
    ret.R=R.inverse();
    ret.t=-(ret.R*t);
    return ret;
  }

  inline Vec2f operator*(const Vec2f& src) const {
    return R*src+t;
  }
  
};
