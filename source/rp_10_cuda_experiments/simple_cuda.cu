#include <vector>
#include "cuda_common.h"
#include <iostream>

using namespace std;

void addArrayCPU(float* dest, const float* src1,  const float* src2, int size) {
  for (int i=0; i<size; ++i) {
    dest[i]=src1[i]+src2[i];
  }
}

__global__ void addArrayCUDA_kernel(float* dest, const float* src, int size) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid >=size)
      return;
    dest[tid]+=src[tid];
}

void addArrayCUDA(float* dest, const float* src1,  const float* src2, int size) {

  // copu on gpu
  float* ws_gpu;
  CUDA_CHECK(cudaMalloc((void**)&ws_gpu, sizeof(float)*2*size));
  CUDA_CHECK(cudaMemcpy(ws_gpu, src1, sizeof(float)*size, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(ws_gpu+size, src2, sizeof(float)*size, cudaMemcpyHostToDevice));
  
  int n_threads=1024;
  int n_blocks  = roundUp(size,n_threads);
  addArrayCUDA_kernel<<<n_blocks, n_threads>>>(ws_gpu, ws_gpu+size, size);
  CUDA_CHECK(cudaMemcpy(dest, ws_gpu, sizeof(float)*size, cudaMemcpyDeviceToHost));
  CUDA_CHECK(cudaFree(ws_gpu));
}



float dotProductArrayCPU(const float* src1,  const float* src2, int size) {
  float result=0;
  for (int i=0; i<size; ++i) {
    result+=src1[i]*src2[i];
  }
  return result;
}


__global__ void mulArrayCUDA_kernel(float* dest, const float* src, int size) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid >=size)
      return;
    dest[tid]*=src[tid];
}

__global__ void reduceArrayCUDA_kernel(float* dest, const float* src, int dest_size, int src_size) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid >=dest_size)
      return;
    int dest_idx=tid;
    int src_idx=tid*2;
    if (src_idx>=src_size)
      return;
    dest[dest_idx]=src[src_idx];
    ++src_idx;
    if (src_idx>=src_size)
      return;
    dest[dest_idx]+=src[src_idx];
}

float dotProductArrayCUDA(const float* src1,  const float* src2, int size) {
  if (! size)
    return 0;
  
  float* ws=0;
  CUDA_CHECK(cudaMalloc((void**)&ws, sizeof(float)*2*size));
  float* prod_ws=ws;
  float* red_ws=ws+size;
  CUDA_CHECK(cudaMemcpy(prod_ws, src1, sizeof(float)*size, cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(red_ws,  src2, sizeof(float)*size, cudaMemcpyHostToDevice));

  // a*b
  int n_threads=1024;
  int n_blocks  = roundUp(size,n_threads);
  mulArrayCUDA_kernel<<<n_blocks, n_threads>>>(prod_ws, red_ws, size);

  // reduction
  int src_size=size;
  float* src=prod_ws;
  float* dest=red_ws;
  while (src_size>1) {
    int dest_size=roundUp(src_size, 2);
    int n_threads=1024;
    int n_blocks  = roundUp(size,n_threads);
    reduceArrayCUDA_kernel<<<n_blocks, n_threads>>>(dest, src, dest_size, src_size);
    std::swap(src, dest);
    src_size=dest_size;
  }
  float result;
  CUDA_CHECK(cudaMemcpy(&result, src, sizeof(float), cudaMemcpyDeviceToHost));
  return result;
}

std::ostream& operator<<(std::ostream& os, const std::vector<float>& src) {
  for (const auto& v: src)
    os << v << " ";
  return os;
}


int main(int argc, char** argv) {
  int size=20;
  std::vector<float> src1(size), src2(size);
  for (auto& v: src1)
    v=drand48();

  cerr << "src1: " << src1 << endl;
  for (auto& v: src2)
    v=drand48();
  cerr << "src2: " << src2 << endl;

  cerr << "sum (cpu)";
  std::vector<float> dest(size);
  addArrayCPU(&dest[0], &src1[0], &src2[0], size);
  cerr << dest << endl;

  cerr << "sum (cuda)";
  addArrayCUDA(&dest[0], &src1[0], &src2[0], size);
  cerr << dest << endl;

  cerr << "dot (cpu)" << dotProductArrayCPU(&src1[0], &src2[0], size);
  cerr << "dot (cuda)" << dotProductArrayCUDA(&src1[0], &src2[0], size);
  
}
