#pragma once
#include "cuda_common.h"
#include <iostream>


template <typename T, int ChunkSize=16>
struct Reduce_{
  void init(int num_items);  
  T reduce(T* input, int num_items);  
  ~Reduce_();
  T* ws=0;
  int ws_capacity=0;
};
