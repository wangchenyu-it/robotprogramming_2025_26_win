#include "lidar.h"
#include <iostream>
using namespace std;

Lidar::Lidar(WorldItem* parent_,
             float fov_,
             int num_beams_,
             int range_max_,
             const Pose& pose_):
  WorldItem(parent_),
  fov(fov_),
  num_beams(num_beams_),
  range_max(range_max_)
{
  pose=pose_;
  angle_min=-fov/2;
  angular_res=fov/num_beams;
  ranges=new float[num_beams];
}

Lidar::Lidar(World* world_,
             float fov_,
             int num_beams_,
             int range_max_,
             const Pose& pose_):
  WorldItem(world_),
  fov(fov_),
  num_beams(num_beams_),
  range_max(range_max_)
{
  pose=pose_;
  angle_min=-fov/2;
  angular_res=fov/num_beams;
  ranges=new float[num_beams];
}

void Lidar::timeTick(float dt) {
  Pose wp=poseInWorld();
  float alpha=angle_min+wp.theta;
  IndexPair origin=world->worldToIndices(wp.translation());
  float max_grid_range=range_max*world->inv_res;
  for (int i=0; i<num_beams; ++i) {
    IndexPair endpoint=origin;
    float grid_range=max_grid_range;
    bool result = world->traverseGrid(endpoint.r, endpoint.c, grid_range, alpha);
    alpha+=angular_res;
    if (! result)
      ranges[i]=range_max;
    else {
      ranges[i]= (max_grid_range-grid_range)*world->resolution;
    }
  }
}

void Lidar::draw() {
  Pose wp=poseInWorld();
  float alpha=angle_min;
  IndexPair origin=world->worldToIndices(wp.translation());
  for (int i=0; i<num_beams; ++i) {
    float r=ranges[i];
    Point ep = wp * Point(r*cos(alpha), r*sin(alpha));
    IndexPair epi=world->worldToIndices(ep);
    cv::line(world->_display_image, cv::Point(origin.c, origin.r), cv::Point(epi.c, epi.r), cv::Scalar(127,127,127), 1);
    alpha+=angular_res;
  }
}
