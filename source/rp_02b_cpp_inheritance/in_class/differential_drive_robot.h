#pragma once
#include "world.h"

struct DifferentialDriveRobot: public WorldItem {
  float rot_vel=0;
  float trans_vel=0;
  DifferentialDriveRobot(WorldItem* p=0): WorldItem(p) {}
  virtual void timerTick(float dt) override;
  void draw(Canvas& canvas) const override;
};
