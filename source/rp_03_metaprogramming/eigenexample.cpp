#include <Eigen/Core>
#include <Eigen/Geometry>
#include <memory>

using Vector3f = Eigen::Matrix<float, 3,3>;
using Isometry3f = Eigen::Isometry3f;


struct Link {
  std::shared_ptr<Link> next_link;
  Link* previous_link;
  Isometry3f p2n;
  Vector3f axis;
  float q;

  Eigen::Isometry3f poseInParent() {
    Eigen::Isometry3f relative_joint_motion;
    relative_joint_motion.linear()=Eigen::AngleAxisf(q, axis).toRotationMatrix();
    relative_joint_motion.translation().setZero();
    
    if (previous_link)
      return previous_link->poseInParent()*relative_joint_motion*p2n;
    return relative_joint_motion*p2n;
  }
};

  int main(int argc, char** argv) {

  
}
