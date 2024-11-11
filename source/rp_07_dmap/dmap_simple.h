#pragma once
#include <queue>
#include <limits>
#include <utility>
#include <vector>
#include "grid_.h"

struct DMapCell{
  DMapCell* parent=0;
};

struct DistanceMap: public Grid_<DMapCell> {
  using PQType=std::queue<DMapCell*>;
  inline int d2(const DMapCell* c1, const DMapCell* c2) const;
  inline int d2(const DMapCell* c) const;
  PQType initCompute(const std::vector<std::pair<int, int> >& obstacles);
  void expand(PQType& pq, int dmax);  
  void compute(const std::vector<std::pair<int, int> >& obstacles,
               int dmax=std::numeric_limits<int>::max());
  Grid_<float> toDistances() const;
};
