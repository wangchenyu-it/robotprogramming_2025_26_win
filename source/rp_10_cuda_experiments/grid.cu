#include "grid.h"
#include "grid.cuh"

template struct Grid_<float>;
template struct GridCUDA_<float>;
template struct Grid_<Eigen::Vector2f>;
template struct GridCUDA_<Eigen::Vector2f>;
