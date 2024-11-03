#include "world.h"
  
struct DifferentialDriveRobot: public WorldItem {
  float rot_vel=0;
  float trans_vel=0;
  DifferentialDriveRobot(WorldItem* p=0): WorldItem(p) {}
  virtual void timerTick(float dt) override {
    Isometry2f motion(dt*trans_vel, 0, dt*rot_vel);
    Isometry2f old_pose_in_parent=pose_in_parent;
    pose_in_parent=pose_in_parent*motion;

    World* w=getWorld();
    if (w->checkCollision(this)) {
      pose_in_parent=old_pose_in_parent;
    }
    WorldItem::timerTick();
  }
};


struct LaserScan {
  float range_max=10, range_min=0.1;
  float angle_start=-M_PI/2, angle_end=M_PI/2;
  int num_beams=181;
  float* ranges;
  LaserScan(int n_beams) {
    num_beams=n_beams;
    ranges=new float[num_beams];
  }
  ~LaserScan(){
    delete[] ranges;
  }

};

struct Lidar: public WorldItem {
  LaserScan* scan=0;
  DifferentialDriveRobot(WorldItem* p=0): WorldItem(p) {}
  virtual void timerTick(float dt) override {
    Isometry2f motion(dt*trans_vel, 0, dt*rot_vel);
    Isometry2f old_pose_in_parent=pose_in_parent;
    pose_in_parent=pose_in_parent*motion;

    World* w=getWorld();
    if (w->checkCollision(this)) {
      pose_in_parent=old_pose_in_parent;
    }
    WorldItem::timerTick();
  }
};
  
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
  Isometry2f iso(0.1,0, 0.01);
  Isometry2f pose;
  pose.setIdentity();
  
}
