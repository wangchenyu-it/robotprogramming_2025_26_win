#include "lidar.h"


LaserScan::LaserScan(int n_beams):
  num_beams(n_beams){
  ranges = new float[num_beams];
}

LaserScan::~LaserScan(){
  delete[] ranges;
}


Lidar::Lidar(LaserScan& my_scan, WorldItem* p):
    WorldItem(p),
    scan(my_scan)
  {}

const GridMap* Lidar::getGridMap() const {
  const WorldItem* aux=this;
  while (aux) {
    const GridMap* gmap=dynamic_cast<const GridMap*>(aux);
    if (gmap)
      return gmap;
    aux=aux->parent;
  }
  return 0;
}


void Lidar::timerTick(float dt) {
  const GridMap* gmap=getGridMap();
  if (! gmap)
    throw std::runtime_error("no gmap");

  // grid in world
  Isometry2f giw=gmap->poseInWorld();
  // lidar in world
  Isometry2f liw=poseInWorld();

  // need to get the relative angle of the lidar in the grid
  Isometry2f lig=giw.inverse()*liw;
  float alpha_offset=lig.R.angle();
    
  Vec2f lidar_origin=gmap->w2g(liw.t);
  float d_alpha=(scan.angle_end-scan.angle_start)/scan.num_beams;
  float alpha=scan.angle_start+alpha_offset;
  //cerr << "org: "<< lidar_origin << endl;
  for (int i=0; i<scan.num_beams; ++i) {
    int x_end=lidar_origin.x();
    int y_end=lidar_origin.y();
    int range_int=gmap->scanSegment(x_end, y_end, alpha, 127, scan.range_max/gmap->resolution);
    alpha+=d_alpha;
    float range=range_int*gmap->resolution;
    if (range<scan.range_min)
      range=scan.range_min;
    else if (range>scan.range_max)
      range=scan.range_max;
      
    scan.ranges[i]=range;
    //cerr << "[a: " << alpha << " r: " << range << "]";
  }
  //cerr << endl;
}

void Lidar::draw(Canvas& canvas) const {
    // lidar in world
    Isometry2f liw=poseInWorld();
    float alpha=scan.angle_start;
    float d_alpha=(scan.angle_end-scan.angle_start)/scan.num_beams;
    for (int i=0; i<scan.num_beams; ++i) {
      alpha+=d_alpha;
      float range=scan.ranges[i];
      Vec2f ep_in_lidar(cos(alpha)*range, sin(alpha)*range);
      canvas.drawLine(liw.t, liw*ep_in_lidar,200);
    }
    WorldItem::draw(canvas);
  }
