#include "grid.h"
#include "linalg.h"

void Grid::clear() {
  if (! values)
    return;
  delete [] values;
  values=0;
  rows=0;
  cols=0;
}
    
void Grid::resize(int r, int c) {
  if (r==rows && c==cols)
    return;
  clear();
  rows=r;
  cols=c;
  values = new CellType[rows*cols];
}

Grid::Grid(int rows, int cols) {
  resize(rows, cols);
}

Grid::~Grid() {
  clear();
}

int Grid::scanSegment(int& x,
                      int& y,
                      float angle,
                      const CellType& val_min,
                      const int max_range) const {

  Vec2f dir(cos(angle),
            sin(angle));

  Vec2f current(x,y);

  int range=0;
  while (range<=max_range) {
    if (! inside(current.x(), current.y()))
      return -1;
    const CellType& target=at(current.x(), current.y());
    if (target<val_min){
      x=current.x();
      y=current.y();
      return range;
    }
    current+=dir;
    ++range;
  }
  return -1;
      
}
