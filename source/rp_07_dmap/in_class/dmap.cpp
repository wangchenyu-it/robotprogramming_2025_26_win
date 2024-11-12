#include "grid.h"
#include <queue>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

struct DMapCell {
  DMapCell* parent=0;
};

using DMapGrid=Grid_<DMapCell>;
using DMapCellQueue=std::queue<DMapCell*>;

using Vector2iVector = std::vector<Vector2i>;

struct DMap: public DMapGrid {
  DMap(int r, int c) : DMapGrid(r, c){}
  
  inline int parentDistance(const DMapCell* c) const {
    if (! c->parent)
      return std::numeric_limits<int>::max();
    Vector2i parent_pos=ptr2rc(c->parent);
    Vector2i c_pos=ptr2rc(c);
    return (parent_pos-c_pos).squaredNorm();
  }

  void clear(){
    std::fill(values.begin(), values.end(), DMapCell());
  }

  void update(const Vector2iVector& obstacles) {
    DMapCellQueue q;
    for (const auto& o: obstacles) {
      if (! inside(o.x(), o.y()))
        continue;
      auto& cell=at(o.x(), o.y());
      cell.parent=&cell;
      q.push(&cell);
      cerr << "push " << o.transpose() << endl;
      
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
};

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

int main(int argc, char** argv) {
  int rows=500, cols=700;
  int n_points=100;
  DMap dmap(rows, cols);
  dmap.clear();

  Vector2iVector obstacles(n_points);
  for (auto& o: obstacles) {
    o=Vector2i(drand48()*rows, drand48()*cols);
  }
  dmap.update(obstacles);

  ofstream os("out.dat");
  os << dmap;

  cout << "#visualize the output with gnuplot" << endl;
  cout << "# install if needed" << endl;
  cout << "# sudo apt install gnuplot" << endl;
  cout << "#start gnuplot"
  cout << "$>gnuplot" << endl;
  cout << "#plot command (from gnuplot)"
  cout << "$gnuplot> plot \"out.dat\" matrix with image" << endl;
  cout << "#ctrl-d to close"
}
