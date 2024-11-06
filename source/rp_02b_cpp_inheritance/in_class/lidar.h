#pragma once
#include "world.h"
#include "grid_map.h"

struct LaserScan {
  float range_max=10, range_min=0.1;
  float angle_start=-M_PI/2, angle_end=M_PI/2;
  int num_beams;
  float* ranges;
  LaserScan(int n_beams=181);
  LaserScan(const LaserScan&) = delete;
  LaserScan& operator=(const LaserScan&) = delete;
  
  ~LaserScan();
};

struct Lidar: public WorldItem {
  LaserScan& scan;

  const GridMap* getGridMap() const;

  Lidar(LaserScan& my_scan, WorldItem* p=0);

  virtual void timerTick(float dt) override;

  void draw(Canvas& canvas) const override;
};
