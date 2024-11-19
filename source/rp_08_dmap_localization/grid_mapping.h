#pragma once
#include <Eigen/Core>

using Eigen::Vector2f;

struct GridMapping {
  int rows, cols;
  float resolution;
  float inv_resolution;
  Eigen::Vector2f center;
  void resize(int r, int c, float res) {
    rows=r;
    cols=c;
    center.x()=r*.5+0.5;
    center.y()=c*.5+0.5;
    resolution=res;
    inv_resolution=1./resolution;
  }

  
  GridMapping(int r=0, int c=0, float res=0.1){
    resize(r,c,res);
  }

  inline Eigen::Vector2f w2g(const Eigen::Vector2f& wp) const {
    return wp*inv_resolution+center;
  }

  inline Eigen::Vector2f g2w(const Eigen::Vector2f& gp) const {
    return (gp-center)*resolution;
  }
  
};
