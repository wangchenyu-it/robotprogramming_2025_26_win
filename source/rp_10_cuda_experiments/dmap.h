#pragma once
#include "grid.h"
#include <iostream>
#include <Eigen/Core>

using namespace std;

struct DMapCell {
  DMapCell* parent=0;
};

using DMapGrid=Grid_<DMapCell>;
using Eigen::Vector2f;
using Eigen::Matrix3f;

using Vector2iVector = std::vector<Vector2i>;
using Vector2fVector = std::vector<Vector2f>;

struct DMap: public DMapGrid {
  DMap(int r, int c) : DMapGrid(r, c){}
  
  inline int parentDistance(const DMapCell* c) const {
    if (! c->parent)
      return std::numeric_limits<int>::max();
    Vector2i parent_pos=ptr2rc(c->parent);
    Vector2i c_pos=ptr2rc(c);
    return (parent_pos-c_pos).squaredNorm();
  }

  void clear();
  
  void update(const Vector2iVector& obstacles);
  
  // this is new
  Grid_<float> distances(float max_distance=std::numeric_limits<float>::max()) const;
  
};

std::ostream& operator<<(std::ostream& os, const DMap& dmap);
