#pragma once
#include <cstdint> // for uint8_t
#include <Eigen/Core>
#include <vector>

using Vector2i=Eigen::Matrix<int,2,1>;

template <typename CellType_>
struct Grid_ {
  using CellType=CellType_;
  using CellTypeVector=std::vector<CellType_>;
  int rows=0;
  int cols=0;
  CellTypeVector values;
  

  void clear() {
    values.clear();
    rows=0;
    cols=0;
  }
    
  void resize(int r, int c) {
    if (r==rows && c==cols)
      return;
    rows=r;
    cols=c;
    values.resize(rows*cols);
  }

  Grid_(int rows, int cols) {
    resize(rows, cols);
  }

  ~Grid_() {
    clear();
  }


  inline CellType_& at(int r, int c){
    return values[r*cols+c];
  }

  inline const CellType_& at(int r, int c) const{
    return values[r*cols+c];
  }

  inline bool inside(int r, int c) const {
    return r>=0 && r<rows && c>=0 && c<cols;
  }

  inline Eigen::Vector2i ptr2rc(const CellType_* ptr) const {
    int offset=ptr-&values[0];
    return Vector2i(offset/cols, offset%cols);
  }
};
