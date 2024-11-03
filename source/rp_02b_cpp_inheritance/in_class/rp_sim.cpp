#include "world.h"
#include "grid_map.h"
#include "differential_drive_robot.h"
#include "lidar.h"

int main (int argc, char** argv) {
  /*
  Vec2f v1;
  v1.fill();
  cout << v1;
  Vec2f v2=v1;
  cout << v2;
  v2.fill(0.1);
  cout << v2;
  cout << (v1-=v2);

  Rotation2f rot1;
  cout << "created" << endl;
  cout << rot1;
  
  rot1.setIdentity();
  cout << "setIdentity" << endl;
  cout << rot1;

  cout << "getAngle" << endl;
  cout << rot1.angle();

  cout << "setAngle" << endl;
  rot1.setAngle(M_PI/2);
  cout << rot1;
  cout << rot1.angle();

  Rotation2f rot_acc;
  rot_acc.setIdentity();
  Rotation2f rot_inc;
  rot_inc.setAngle(M_PI/180.f);

  
  for (int i=0; i<360; ++i) {
    rot_acc=rot_acc*rot_inc;
    float angle_in_radians=rot_acc.angle();
    cout << "i: " << i << " alpha: " << angle_in_radians*180.f/M_PI << endl;
  }

  Rotation2f r_boh(0.5);
  cout << r_boh << endl << r_boh*r_boh.inverse() << endl;

  cout << "*************************" <<endl;
  Vec2f v_mult;
  v_mult.values[0]=20;
  v_mult.values[1]=5;

  cout << v_mult;
  
  v_mult = r_boh*v_mult;
  cout << v_mult;

  cout << r_boh.inverse()*v_mult;
  
  
  */

  // todo: write the rest to test the above functions
  // hint: comment the todoes and address them one by one
  //       after each is done add a lil test

  // at the end uncomment the following
  World w;
  cerr << "world created" << endl;
  GridMap gmap(argv[1], 0.1, &w, Isometry2f(0,0,0));
  DifferentialDriveRobot ddr(&gmap);
  ddr.pose_in_parent=Isometry2f(0,0,0);
  ddr.radius=1.5;

  DifferentialDriveRobot ddr2(&ddr);
  ddr2.pose_in_parent=Isometry2f(3,0,0);
  ddr2.radius=1;

  DifferentialDriveRobot ddr3(&gmap);
  ddr3.pose_in_parent=Isometry2f(5,5,0);
  ddr3.radius=1.5;

  LaserScan scan(90);
  Lidar lid(scan, &ddr2);
  
  cerr << "gmap_loaded" << endl;
  Canvas canvas;
  canvas.init(gmap.rows, gmap.cols, 0.1);
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
