#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "dmap.h"
#include "grid_mapping.h"


/* simple registration on dmap
   usage: instantiate and intialize an object

   DMapLocalizer loc;
   float resolution=0.1; // 10 cm x cell
   float max_range=10; // 10 m // saturation of dmap
   loc.init(resolution, max_range, dmap);

   initialize the initial pose to something that makes sense
   
   Eigen::Isometry2f initial_pose=...;

   // these are the lidar endpoints in robot coordinates
   std::vector<Eigen::Vector2f> measurements;

   loc.reset(initial_pose);
   bool happy=false;
   while (! happy && ! sad) {
     loc.reset(loc.X);
     for (const auto& z: measurements)
       loc.addMeasurement(z);
     loc.update();
   }
   // at this point loc.X contains the localized position
   
 */
struct DMapLocalizer {
  GridMapping mapping; // to convert from world to grid coordinates
  Grid_<float> distances; // dmap (float)
  Grid_<float>::GridGradientType distance_gradients; // grid_<Vector2f>

  Eigen::Isometry2f X; // current pose
  float kernel_threshold=0.5;
  float point_damping=1;
  bool use_bilinear=false;
  
  void init(float resolution,
            float max_range,
            const DMap& dmap);
  
  // call once at the beginning of each iteration
  // x_start is the initial robot position
  void reset(const Eigen::Isometry2f& X_start=Eigen::Isometry2f::Identity());
  
  // within an iteration, add each measurement using this method
  // z is the lidar endpoint in world coordinates
  void addMeasurement(const Eigen::Vector2f& z);

  // returns true if inside,
  // distance: out parameter, the value at the point pg on the grid
  // gradients: out parameter, the vector with the gradients
  // pg: input parameter, the point on the grid (in float coords)
  
  bool fetchDistanceAndGradients(float& distance,
                                 Eigen::Vector2f& gradients,
                                 const Vector2f& pg);
  
  // call this after adding the measurements, to end one iteration
  void update();

  // temporaries
  float chi2;
  int   total_damping;
  int   num_inliers;
  int   num_outliers;
  Eigen::Matrix3f H;
  Eigen::Vector3f b;
  Eigen::Vector3f dx;
};
