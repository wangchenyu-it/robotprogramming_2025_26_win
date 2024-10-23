#include "world.h"

struct Robot: public WorldItem {
  float tv=0, rv=0;
  float radius;
  int radius_in_pixels;
  Robot(World* world_, float radius_=0.3):
    WorldItem(world_),
    radius(radius_),
    radius_in_pixels(radius_*world_->inv_res)
  {}
  
  void timeTick(float dt) override;
  
  bool collides(const Point& p);
  
  void draw() override;

};

