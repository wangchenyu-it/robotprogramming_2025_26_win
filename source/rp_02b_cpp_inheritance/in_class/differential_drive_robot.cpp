#include "differential_drive_robot.h"

void DifferentialDriveRobot::timerTick(float dt)  {
  Isometry2f motion(dt*trans_vel, 0, dt*rot_vel);
  Isometry2f old_pose_in_parent=pose_in_parent;
  pose_in_parent=pose_in_parent*motion;

  World* w=getWorld();
  if (w->checkCollision(this)) {
    //cerr << "collision" << endl;
    pose_in_parent=old_pose_in_parent;
  }
  WorldItem::timerTick(dt);
}

void DifferentialDriveRobot::draw(Canvas& canvas) const {
  // we draw the robot as a circle with a radial line
  // showing its orientation;
  Isometry2f piw=poseInWorld();
  Vec2f endpoint(piw.t.x()+piw.R.R[0][0]*radius,
                 piw.t.y()+piw.R.R[1][0]*radius);
    
  canvas.drawCircle(piw.t, radius, 20);
  canvas.drawLine(piw.t,
                  endpoint,
                  20);
  // cerr << "robot_origin: " << piw.t << endl;
  // cerr << "robot_endpoint: " << endpoint << endl;
    
  WorldItem::draw(canvas);
}
