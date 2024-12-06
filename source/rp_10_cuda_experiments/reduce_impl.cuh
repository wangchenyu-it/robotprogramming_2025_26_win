#include "reduce_.h"
#include <iostream>

using namespace std;


template <typename T>
__global__ void _reduce1(T* dest, T* src, int size) {
  const Op op;
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  int start_idx=tid*2;
  if (start_idx>=size)
    return;
  if (start_idx+1>=size)
    dest[tid]=src[start_idx];
  else
    dest[tid]=src[start_idx]+src[start_idx+1];
}

template <typename T>
T Reduce_<T>::reduce(T* input, int num_items) {
  using namespace std;
  init(num_items);
  int size=num_items;
  T* src=input;
  T* dest=ws;
  while (size>1) {
    int n_size=roundUp(size, ChunkSize);
    int n_threads=1024;
    int n_blocks  = (n_size + n_threads - 1) / n_threads;
    _reduce1<T><<<n_blocks, n_threads>>>(dest, src,size);
    std::swap(src, dest);
    size=n_size;
  }
  T out;
  CUDA_CHECK(cudaMemcpy(&out, src, sizeof(T), cudaMemcpyDeviceToHost));
  return out;
}
  
template <typename T>
void Reduce_<T>::init(int num_items) {
  int new_ws_size=roundUp(num_items, 2);
  if (ws_capacity<new_ws_size) {
    if (ws) {
      CUDA_CHECK(cudaFree(ws));
    }
    CUDA_CHECK(cudaMalloc((void**)&ws, sizeof(T)*new_ws_size));
    ws_capacity=new_ws_size;
  }
}
  
template <typename T>
Reduce_<T>::~Reduce_() {
  if (ws)
    CUDA_CHECK(cudaFree(ws));
  ws=0;
  ws_capacity=0;
}
