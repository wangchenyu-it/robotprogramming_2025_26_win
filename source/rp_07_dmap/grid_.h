#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <iostream>

template <typename CellType_>
struct Grid_ {
  using CellType=CellType_;
  using StorageType=std::vector<CellType_>;
  int _rows=0, _cols=0;
  StorageType _storage;

  void resize(int nrows, int ncols){
    _rows=nrows;
    _cols=ncols;
    _storage.resize(_rows*_cols);
  }

  Grid_(int rows=0, int cols=0) {
    resize(rows, cols);
  }

  inline const StorageType& storage() const {
    return _storage;
  }

  inline StorageType& storage() {
    return _storage;
  }

  inline int rows() const {return _rows;}

  inline int cols() const {return _cols;}

  inline std::pair<int,int> idx2rc(size_t idx) const {
    return std::make_pair((int)(idx/_cols),(int)(idx%_cols));
  }


  inline size_t rc2idx(int row, int col) const {
    return row*_cols+col;
  }

  inline std::pair<int,int> ptr2rc(const CellType* ptr) const {
    return idx2rc(ptr - &_storage[0]);
  }


  inline CellType& at(int r, int c) {
    size_t idx=rc2idx(r,c);
    if (idx>_storage.size())
      throw std::runtime_error("out of bounds");
    return _storage[idx];
  }

  inline const CellType& at(int r, int c) const {
    size_t idx=rc2idx(r,c);
    if (idx>_storage.size())
      throw std::runtime_error("out of bounds");
    return _storage[idx];
  }

  template <typename DestType>
  Grid_<DestType> cast() const {
    Grid_<DestType> dest(_rows, _cols);
    for (size_t i=0; i<_storage.size(); ++i)
      dest._storage[i]=static_cast<DestType>(_storage[i]);
    return dest;
  }
};

template <typename CellType>
std::ostream& operator<< (std::ostream& os, const Grid_<CellType>& grid) {
  os << "# rows: " << grid.rows() << " cols: " << grid.cols() << std::endl;
  for (int r=0; r<grid.rows(); ++r) {
    for (int c=0; c<grid.cols(); ++c)
      os << grid.at(r,c) << " ";
    os << std::endl;
  }
  return os;
}
