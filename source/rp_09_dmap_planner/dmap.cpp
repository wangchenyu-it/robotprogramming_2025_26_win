#include "dmap.h"
#include <queue>
#include <iostream>

void DMap::clear(){
  std::fill(values.begin(), values.end(), DMapCell());
}

void DMap::update(const Vector2iVector& obstacles) {
  using DMapCellQueue=std::queue<DMapCell*>;
  DMapCellQueue q;
  for (const auto& o: obstacles) {
    if (! inside(o.x(), o.y()))
      continue;
    auto& cell=at(o.x(), o.y());
    cell.parent=&cell;
    q.push(&cell);
  }
    
  while (! q.empty()) {
    auto expanded=q.front();
    q.pop();
    Vector2i expanded_pos=ptr2rc(expanded);
    Vector2i parent_pos=ptr2rc(expanded->parent);
    for (int dr=-1; dr<=1; ++dr) {
      for (int dc=-1; dc<=1; ++dc) {
        if (dr==0 && dc==0)
          continue;
        Vector2i increment(dr, dc);
        Vector2i next_pos=increment+expanded_pos;
        if (! inside(next_pos.x(), next_pos.y()))
          continue;
        auto& next=at(next_pos.x(), next_pos.y());
        int d_next=(parent_pos-next_pos).squaredNorm();
        if ( ! next.parent || d_next  < parentDistance(&next)) {
          next.parent=expanded->parent;
          q.push(&next);
        }
      }
    }
  }
}

Grid_<float> DMap::distances(float max_distance) const {
  Grid_<float> dest(rows, cols);
  for (int r=0; r<rows; ++r){
    for (int c=0; c<cols; ++c) {
      const auto& cell=at(r,c);
      dest.at(r,c) = std::min(max_distance, sqrtf(parentDistance(&cell)));
    }
  }
  return dest;
}

std::ostream& operator<<(std::ostream& os, const DMap& dmap) {
  for (int r=0; r<dmap.rows; ++r){
    for (int c=0; c<dmap.cols; ++c) {
      const auto& cell=dmap.at(r,c);
      os << sqrt(dmap.parentDistance(&cell)) << " ";
    }
    os << endl;
  }
  return os;
}
