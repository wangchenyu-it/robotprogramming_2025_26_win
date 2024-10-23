#pragma once
#include "world.h"

struct Lidar: public WorldItem {
  Lidar(WorldItem* parent_,
        float fov_=M_PI,
        int num_beams_=180,
        int range_max_=10,
        const Pose& pose_=Pose());
  
  Lidar(World* world_,
        float fov_=M_PI,
        int num_beams_=180,
        int range_max_=10,
        const Pose& pose_=Pose());
  
  void timeTick(float dt);
  
  void draw() override;  
  float fov;
  int num_beams;
  float angle_min;
  float angular_res;
  float range_max;
  float* ranges;
};
