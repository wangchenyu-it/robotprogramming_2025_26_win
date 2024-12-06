#include "dmap_localizer.h"
#include <Eigen/Cholesky>

void DMapLocalizer::init(float resolution,
                         float max_range,
                         const DMap& dmap) {
    
  float max_range_in_pixel=max_range/resolution;
  distances=dmap.distances(max_range_in_pixel);
  distance_gradients=distances.gradient();
  for(auto& d: distances.values) {
    d*=resolution;
  }
  mapping.resize(distances.rows, distances.cols, resolution);
}

// inner computation
void DMapLocalizer::reset(const Eigen::Isometry2f& X_start){
  X=X_start;
  H.setZero();
  b.setZero();
  dx.setZero();
  chi2=0;
  num_inliers=0;
  num_outliers=0;
  total_damping=0;
}

  
bool DMapLocalizer::fetchDistanceAndGradients(float& distance,
                               Eigen::Vector2f& gradients,
                               const Vector2f& pg) {
  if (use_bilinear) {
    auto d_pair=distances.atBilinear(pg.x(), pg.y());
    if (! d_pair.second)
      return false;
    distance=d_pair.first;
    gradients=distance_gradients.atBilinear(pg.x(), pg.y()).first;
    return true;
  } else {
    Eigen::Vector2i pi=pg.cast<int>();
    if (! distances.inside(pi.x(), pi.y()))
      return false;
    distance=distances.at(pi.x(), pi.y());
    gradients=distance_gradients.at(pi.x(), pi.y());
    return true;
  }
}

void DMapLocalizer::addMeasurement(const Eigen::Vector2f& z) {
  // 1. map the measurement in the world
  Eigen::Vector2f p=X*z;

  // 2. convert the world point in grid coordinates
  Eigen::Vector2f pg=mapping.w2g(p);

  // 3. fetch the distance value and the gradients
  //    according to the choosen policy
  float e;
  Eigen::Vector2f grad;
  if (! fetchDistanceAndGradients(e, grad, pg)) {
    ++num_outliers;
    return;
  }

  // 4. fetch the distance (in meters)
  total_damping+=point_damping;
  float this_chi=e*e;
  float kscale=1;
  if (this_chi>kernel_threshold) {
    kscale=sqrt(kernel_threshold/this_chi);
    ++num_outliers;
  } else {
    ++num_inliers;
  }
  chi2+=this_chi;

  // 5. compute the linearization (gauss newton)
  //cerr << "p: " << p.transpose() << endl;
  Eigen::Matrix<float, 2, 3> J_icp;
  J_icp.block<2,2>(0,0).setIdentity();
  J_icp.block<2,1>(0,2)=Eigen::Vector2f(-p.y(), p.x());

    
  Eigen::Matrix<float,1,2> J_grad=grad.transpose();
  //cerr << J_icp << endl;
  //cerr << J_grad << " norm: " << J_grad.norm() << endl;
  Eigen::Matrix<float,1,3> J=J_grad*J_icp;
  H.noalias()+=J.transpose()*J*kscale;
  b.noalias()-=J.transpose()*e*kscale;
}

  
void DMapLocalizer::update() {
  H+=Matrix3f::Identity()*total_damping;
  //cerr << "H: " << endl;
  //cerr << H << endl;
  //cerr << "b: " << b.transpose() << endl;
  dx=H.ldlt().solve(b);
  Eigen::Isometry2f dX;
  dX.setIdentity();
  dX.linear()=Eigen::Rotation2Df(dx(2)).toRotationMatrix();
  dX.translation()=dx.head<2>();
  X=dX*X;
}
