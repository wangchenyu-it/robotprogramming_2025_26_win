#include "robot.h"
  
void Robot::timeTick(float dt)  {
  cerr << "rv: " << rv << " tv: " << tv << endl;
  Pose next_pose=pose*Pose(tv*dt, 0, rv*dt);
  if (! collides(next_pose.translation())) {
    pose=next_pose;
    
  } else {
    cerr << "collision" << endl;
    tv=0; rv=0;
  }
}
  
bool Robot::collides(const Point& p) {
  IndexPair p0=world->worldToIndices(p);
  int r2=radius_in_pixels*radius_in_pixels;
  for (int r=-radius_in_pixels; r<radius_in_pixels+1; ++r) {
    for (int c = -radius_in_pixels; c<radius_in_pixels+1; ++c) {
      if (r*r+c*c>r2)
        continue;
      // outside of world
      IndexPair ip(p0.r+r, p0.c+c);
      if (! world->isInside(ip))
        return true;
      // obstacle
      if (world->at(ip)<127)
        return true;
    }
  }
  return false;
}

void Robot::draw()  {
  IndexPair p0=world->worldToIndices(pose.translation());
  int r2=radius_in_pixels*radius_in_pixels;
  for (int r=-radius_in_pixels; r<radius_in_pixels+1; ++r) {
    for (int c = -radius_in_pixels; c<radius_in_pixels+1; ++c) {
      if (r*r+c*c>r2)
        continue;
      // outside of world
      IndexPair ip(p0.r+r, p0.c+c);
      if (! world->isInside(ip))
        break;
      world->_display_image.at<uint8_t>(ip.r, ip.c)=0;
    }
  }
}
