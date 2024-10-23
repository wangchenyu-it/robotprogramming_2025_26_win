#pragma once
#include <cstdint>
#include <cmath>

struct Point {
  Point(float x_, float y_) :x(x_), y(y_){}
  inline Point operator+(const Point& other) const {
    return Point(x+other.x, y+other.y);
  }
  inline Point& operator+=(const Point& other) {
    x+=other.x;
    y+=other.y;
    return *this;
  }
  inline Point operator-(const Point& other) const {
    return Point(x-other.x, y-other.y);
  }
  float x, y;
};

struct Pose {
  float x,y, theta;
  inline Pose (float x_=0, float y_=0, float theta_=0): x(x_), y(y_), theta(theta_) {}
  Pose inverse() const {
    float s=sin(-theta), c=cos(-theta);
    return Pose (x*c-y*s, x*s+y*c, -theta);
  }
  inline Pose operator*(const Pose& other) const {
    float s=sin(theta), c=cos(theta);
    return Pose (x+c*other.x-s*other.y,
                 y+s*other.x+c*other.y,
                 theta+other.theta);
  }
  inline Point operator*(const Point& other) const {
    float s=sin(theta), c=cos(theta);
    return Point (x+c*other.x-s*other.y,
                  y+s*other.x+c*other.y);
  }
  inline Point translation() {return Point(x,y);}
};

struct IndexPair {
  int r, c;
  IndexPair(int r_=0, int c_=0): r(r_), c(c_){}
  inline IndexPair operator + (const IndexPair& other) const {
    return IndexPair(r+other.r, c+other.c);
  }
  inline IndexPair operator - (const IndexPair& other) const {
    return IndexPair(r-other.r, c-other.c);
  }
  inline int operator * (const IndexPair& other) const {
    return r*r+c*c;
  }

};
