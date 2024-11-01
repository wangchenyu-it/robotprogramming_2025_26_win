#include "grid_map.h"

GridMap::GridMap(int rows,
                 int cols,
                 float res,
                 WorldItem* p,
                 const Isometry2f& pose_in_parent_):
  Grid(rows, cols),
  WorldItem(parent),
  resolution(res),
  inv_resolution(1./res),
  grid_origin(rows/2, cols/2),
  _piw(poseInWorld()),
  _ipiw(poseInWorld().inverse())
{
  WorldItem::pose_in_parent=pose_in_parent;
}

bool GridMap::canCollide(const WorldItem* other) const {
  return ! other->isDescendant(this);
}

bool GridMap::collides(const WorldItem* item) const {
  if (! item->isDescendant(this))
    return false;
  Vec2f gp=w2g(item->poseInWorld().t);
  int r0=gp.x();
  int c0=gp.y();
  int radius=(int)(item->radius*inv_resolution);
  int radius2=radius*radius;
  for (int r=-radius; r<radius; ++r){
    int rx=r+r0;
    for (int c=-radius; c<radius; ++c) {
      int cx=c+c0;
      if (! inside(rx,cx))
        continue;
      if (at(rx,cx)<127)
        return true;
    }
  }
  return false;
}
