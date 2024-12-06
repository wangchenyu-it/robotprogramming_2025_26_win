#pragma once
#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>

  
inline void HandleError(cudaError_t err, const char* file, int line) {
  // CUDA error handeling from the "CUDA by example" book
  if (err != cudaSuccess) {
    printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
    exit(EXIT_FAILURE);
  }
}

#define CUDA_CHECK(err) (HandleError(err, __FILE__, __LINE__))

__host__ __device__ __forceinline__ size_t roundUp(int idx, int block_size) {
  return idx/block_size + ((idx%block_size)?1:0);
};

#ifdef __CUDACC__
#pragma diag_suppress 20013
#pragma diag_suppress 20015
#define __CUDA_EXPORT_INLINE__ __host__ __device__ __forceinline__
#else
#define __CUDA_EXPORT_INLINE__ inline
#endif
