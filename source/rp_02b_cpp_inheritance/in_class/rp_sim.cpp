#include "world.h"
#include "grid_map.h"
#include "differential_drive_robot.h"
#include "lidar.h"

int main (int argc, char** argv) {
  World w;
  cerr << "world created" << endl;
  GridMap gmap(argv[1], 0.1, &w, Isometry2f(0,10,0.3));
  DifferentialDriveRobot ddr(&gmap);
  ddr.pose_in_parent=Isometry2f(0,0,0);
  ddr.radius=1.5;

  // DifferentialDriveRobot ddr2(&ddr);
  // ddr2.pose_in_parent=Isometry2f(3,0,0);
  // ddr2.radius=1;

  DifferentialDriveRobot ddr3(&gmap);
  ddr3.pose_in_parent=Isometry2f(5,5,0);
  ddr3.radius=1.5;

  LaserScan scan(90);
  Lidar lid(scan, &ddr);
  
  // cerr << "gmap_loaded" << endl;

  Canvas canvas;
  canvas.init(gmap.rows/2, gmap.cols/2, 0.2);
  int key=0;
  float rv, tv;
  cerr << "ddr: " << &ddr << endl;
  cerr << "gmap: " << &gmap << endl;
  do {
    tv=0; rv=0;
    w.draw(canvas);
    canvas.show();
    key=cv::waitKey(0);
    switch(key) {
    case 81: rv=0.5; break;
    case 82: tv=1; break;
    case 84: tv=-1; break;
    case 83: rv=-0.5; break;
    default:;
    }
    ddr.rot_vel=rv;
    ddr.trans_vel=tv;
    w.timerTick(0.1);
  } while (key!=27);
}
