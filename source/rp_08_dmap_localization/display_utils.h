#include <opencv2/opencv.hpp>
#include "grid.h"


template <typename ContainerType>
void drawPoints(cv::Mat& dest,
                const GridMapping& mapping,
                const Eigen::Isometry2f& X,
                const ContainerType& points,
                uint8_t intensity){
  for (const auto& p: points) {
    const auto pg=mapping.w2g(X*p);
    cv::circle(dest, cv::Point(pg.y(), pg.x()), 2, cv::Scalar(intensity));
  }
}

template <typename T>
cv::Mat grid2cv(const Grid_<T>& src, bool normalize) {
  // 1. we convert the grid to float
  Grid_<float> float_grid=src.template cast<float>();

  if (normalize) {
    // 2. if normalize is selected, we scan for min and max
    float lower=std::numeric_limits<float>::max();
    float upper=std::numeric_limits<float>::min();
    for (const auto& v: float_grid.values) {
      lower=std::min(lower,v);
      upper=std::max(upper,v);
    }
    float range=upper-lower;
    if(range==0) {
      std::fill(float_grid.values.begin(),
                float_grid.values.end(),
                0.);
    } else {
      float irange=255./range;
      for (auto& v: float_grid.values)
        v=(v-lower)*irange;
    }
  }
  
  cv::Mat dest(float_grid.rows, float_grid.cols, CV_8UC1);
  for (int r=0; r<float_grid.rows; ++r){
    for (int c=0; c<float_grid.cols; ++c) {
      dest.at<uint8_t>(r,c)=float_grid.at(r,c);
    }
  }
  return dest;
}
