#include <algorithm>
#include <iostream>
#include <cmath>
#include "dmap_simple.h"
#include "dmap_simple_impl.cpp"

using namespace std;
DistanceMap::PQType DistanceMap::initCompute(const std::vector<std::pair<int, int> >& obstacles){
  PQType pq;
  if (! obstacles.empty()){
    for (const auto& o: obstacles) {
      int r=o.first;
      int c=o.second;
      auto& entry=at(r,c);
      entry.parent=&entry;
      pq.push(&entry);
    }
    return pq;
  } else {
    for (auto& c:_storage) {
      if (c.parent==&c)
        pq.push(&c);
    }
    return pq;
  }
}

void DistanceMap::expand(PQType& pq, int dmax){
  int dmax2 = dmax*dmax;
  int op_count=0;
  int popped=0;
  size_t max_frontier=0;
  while (! pq.empty()) {
    auto current=pq.front();
    ++popped;
    pq.pop();
    const std::pair coords=ptr2rc(current);
    //cerr << "src: " << coords.first << " " << coords.second << " " << d2(current) << endl;
    int r0=coords.first;
    int c0=coords.second;
    for (int dr=-1; dr<=1; ++dr) {
      for (int dc=-1; dc<=1; ++dc) {
        if (dr==0&&dc==0)
          continue;
          
        int r=r0+dr;
        if (r<0||r>=_rows)
          continue;

        int c=c0+dc;
        if (c<0||c>=_cols)
          continue;

        auto& expanded=at(r,c);
        const auto d_old=d2(&expanded);
        const auto d_new=d2(&expanded, current->parent);
        //cerr << "  dest: " << r << " " << c << " " << d_old << " " << d_new << endl;
        if (d_new<dmax2 && d_new<d_old) {
          expanded.parent=current->parent;
          pq.push(&expanded);
          ++op_count;
          max_frontier=std::max(pq.size(), max_frontier);
        }
        //cerr << "  dest-push: " << r << " " << c << " " << d2(&expanded) << endl;
      }
    }
    //char key =getchar();
  }
  cout << "ops: " << op_count << " pops: " << popped << " fmax: " << max_frontier;
}
  
void DistanceMap::compute(const std::vector<std::pair<int, int> >& obstacles,
                          int dmax) {
  auto pq=initCompute(obstacles);
  expand(pq, dmax);
}

Grid_<float> DistanceMap::toDistances() const {
  Grid_<float> dest(_rows, _cols);
  for (size_t i=0; i<_storage.size(); ++i) {
    dest._storage[i]=sqrt((float)d2(&_storage[i]));
  }
  return dest;
}

