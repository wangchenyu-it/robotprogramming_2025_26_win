#pragma once
#include "grid.h"
#include "cuda_common.h"

template <typename CellType_>
__global__ void GridCUDA_fillKernel(CellType_* dest, int size, const CellType_ value) {
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  if (tid >=size)
    return;
  dest[tid]=value;
}

template <typename DestCellType_, typename SrcCellType_>
__global__ void GridCUDA_castKernel(DestCellType_* dest, SrcCellType_*src , int size) {
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  if (tid >=size)
    return;
  dest[tid]=static_cast<DestCellType_>(src[tid]);
}

template <typename GridCudaType_>
__global__ void GridCUDA_gradientKernel(typename GridCudaType_::GridGradientType* dest,
                                        const GridCudaType_* src) {
  const int size=src->rows*src->cols;
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  if (tid>=size)
    return;
  int r=tid/src->cols;
  if (r<1 || r>=src->rows-1)
    return;
  int c=tid-(r*src->cols);
  if (c<1 || c>=src->cols-1)
    return;
  dest->at(r,c)=CellGradientType(src->at(r+1,c)-src->at(r-1,c),
                                 src->at(r,c+1)-src->at(r,c-1));
}

template <typename CellType_>
struct GridCUDA_ {
  using CellType=CellType_;
  using CellGradientType=Eigen::Matrix<CellType, 2, 1>;
  using GridGradientType=GridCUDA_<CellGradientType>;
  using CellTypeArray=CellType*;
  int rows=0;
  int cols=0;
  CellTypeArray values=0;
  GridCUDA_<CellGradientType> *_device_self=0;
  
  void resize(int r, int c) {
    if (rows*cols==r*c)
      return;
    if (values) {
      CUDA_CHECK(cudaFree(values));
    }
    values=0;
    rows=r;
    cols=c;
    CUDA_CHECK(cudaMalloc((void**)values, sizeof(CellType)*rows*cols));
  }

  void clear() {
    if (values) {
      CUDA_CHECK(cudaFree(values));
    }
    values=0;
    rows=0;
    cols=0;
  }

  GridCUDA_(int rows=0, int cols=0) {
    if (rows&&cols)
      resize(rows, cols);
    CUDA_CHECK(cudaMalloc((void**)&_device_self, sizeof(GridCUDA_<CellType>)));
    CUDA_CHECK(cudaMemcpy(_device_self, this, sizeof(GridCUDA_<CellType>), cudaMemcpyHostToDevice));
  }

  GridCUDA_(const GridCUDA_<CellType_>& src) {
    resize(src.rows, src.cols);
    if (!(rows*cols))
      return;
    CUDA_CHECK(cudaMemcpy(values, src.values, sizeof(CellType)*rows*cols, cudaMemcpyDeviceToDevice));
  }

  const GridCUDA_<CellType_>& operator=(const GridCUDA_<CellType_>& src) {
    resize(src.rows, src.cols);
    if (!(rows*cols))
      return *this;
    CUDA_CHECK(cudaMemcpy(values, src.values, sizeof(CellType)*rows*cols, cudaMemcpyDeviceToDevice));
    return *this;
  }

  const GridCUDA_& operator=(const Grid_<CellType_>& src) {
    resize(src.rows, src.cols);
    if (!(rows*cols))
      return *this;
    CUDA_CHECK(cudaMemcpy(values, &src.values[0], sizeof(CellType)*rows*cols, cudaMemcpyHostToDevice));
    return *this;
  }

  ~GridCUDA_() {
    clear();
    CUDA_CHECK(cudaFree(_device_self));
  }

  __CUDA_EXPORT_INLINE__
  CellType_& at(int r, int c){
    return values[r*cols+c];
  }

  __CUDA_EXPORT_INLINE__
  const CellType_& at(int r, int c) const{
    return values[r*cols+c];
  }

  __CUDA_EXPORT_INLINE__
  bool inside(int r, int c) const {
    return r>=0 && r<rows && c>=0 && c<cols;
  }

  __CUDA_EXPORT_INLINE__
  Eigen::Vector2i ptr2rc(const CellType_* ptr) const {
    int offset=ptr-values;
    return Vector2i(offset/cols, offset%cols);
  }
  
  /* This also is new :)*/
  __CUDA_EXPORT_INLINE__
  std::pair<CellType, bool> atBilinear(float r, float c) const {
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
  GridCUDA_<DestCellType> cast() const {
    GridCUDA_<DestCellType> dest(rows, cols);
    int size=rows*cols;
    int n_threads=1024;
    int n_blocks  = (size + n_threads - 1) / n_threads;
    GridCUDA_gradientKernel<<<n_blocks, n_threads>>>(dest->_device_self, _device_self, size);
    return dest;
  }

  void toCPU(Grid_<CellType_>& dest) const {
    dest.resize(rows, cols);
    dest.rows=rows;
    dest.cols=cols;
    CUDA_CHECK(cudaMemcpy(&dest.values[0], values, sizeof(GridCUDA_<CellType>)*rows*cols, cudaMemcpyDeviceToHost));
  }
};
  
