#pragma once
#include <cstdint> // for uint8_t
#include <Eigen/Core>
#include <vector>

using Vector2i=Eigen::Matrix<int,2,1>;

template <typename CellType_>
struct Grid_ {
  using CellType=CellType_;
  using CellGradientType=Eigen::Matrix<CellType, 2, 1>;
  using GridGradientType=Grid_<CellGradientType>;
  using CellTypeVector=std::vector<CellType_>;
  int rows=0;
  int cols=0;
  CellTypeVector values;

  /* This is new :)*/
  GridGradientType gradient() const {
    GridGradientType grad(rows, cols);
    std::fill(grad.values.begin(), grad.values.end(), CellGradientType(0,0));
    for (int r=1; r<rows-1; ++r){ 
      for (int c=1; c<cols-1; ++c) {
        grad.at(r,c)=CellGradientType(at(r+1,c)-at(r-1,c),
                                      at(r,c+1)-at(r,c-1));
      }
    }
    return grad;
  }

  /* This also is new :)*/
  inline std::pair<CellType, bool> atBilinear(float r, float c) const {
    int ix0=floor(r);
    int iy0=floor(c);
    if (ix0<0
        || ix0>rows-2
        || iy0<0
        || iy0>cols-2)
      return std::make_pair(CellType(), false);
    const CellType& f00=at(ix0, iy0);
    const CellType& f01=at(ix0, iy0+1);
    const CellType& f10=at(ix0+1, iy0);
    const CellType& f11=at(ix0+1, iy0+1);
    const float dx=r-ix0;
    const float dy=c-iy0;
    return std::make_pair(f00*(1.-dx)*(1.-dy)+f01*(1.-dx)*dy+f10*dx*(1.-dy)+f11*dx*dy, true);
  }

  /* This is cool new :)*/
  template <typename DestCellType>
  Grid_<DestCellType> cast() const {
    Grid_<DestCellType> dest(rows, cols);
    for (size_t i=0; i<values.size(); ++i) {
      dest.values[i]=static_cast<DestCellType>(values[i]);
    }
    return dest;
  }
  
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

  Grid_(int rows=0, int cols=0) {
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
