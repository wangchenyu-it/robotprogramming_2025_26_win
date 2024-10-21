#include <chrono>
#include <memory>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

// Allows us to use literals for time units
using namespace std::chrono_literals;
// Just to be sure that we have pi available on every platform [required up
// c++20]
constexpr float pi = 3.14159265358979323846;

class TurtleSquareNode : public rclcpp::Node {
 public:
  TurtleSquareNode() : Node("turtle_square_node") {
    // Create the publisher for velocity commands [binded to
    // geometry_msgs::msg::Twist message] For simplicity, publish on
    // "/turtle1/cmd_vel"
    // TODO here!

    // Create the subscriber for the turtle pose [binded to turtlesim::msg::Pose
    // message] For simplicity, subscribe to "/turtle1/pose"
    // TODO here!

    // Create the timer for the control loop
    // The timer is set to call the timer_callback function every 1millisecond
    // [1ms]
    timer = this->create_wall_timer(
        1ms, std::bind(&TurtleSquareNode::timer_callback, this));
  }

 private:
  /**
   * Timer callback.
   * Increase the current time by dt and execute the control loop.
   * The control loop is a simple state machine that makes the turtle move in a
   * square trajectory. The machine has two states:
   * - 0: Move forward for side_length meters
   * - 1: Turn right for angle_to_turn radians
   * The completion of each state is verified as follows:
   * - [0-FORWARD] If the current time excedes [side_length / trans_vel] then
   * the state is changed to 1 [TURN]
   * - [1-TURN] If the current time excedes [angle_to_turn / rot_vel] then the
   * state is changed to 0 [FORWARD]
   * In both cases, reset current_time to 0.
   *
   * The translational velocity [v] and angular velocity [w] are set based on
   * the current state:
   * - [0-FORWARD] : v = trans_vel | w = 0
   * - [1-TURN] : v = 0 | w = rot_vel
   *
   * Once the velocities are set, create a geometry_msgs::msg::Twist message and
   * store the computed velocities accordingly. Remember that, to control the
   * turtle, [v] should be written in the linear.x field and [w] in the
   * angular.z field. Finally, publish the message on the velocity_pub
   * publisher.
   */
  void timer_callback() {
    // Commenting these to prevent unusued variable warning | errors since
    // -Wpedantic is set in the CMake :( float v = 0.0; float w = 0.0;

    current_time += dt;

    // TODO here!

    // velocity_pub->publish(velocity_message);
  }

  /**
   * Pose callback.
   * Once the turtle pose is received, print the turtle pose on the console
   * using RCLCPP_INFO macro. If you have problems, you might also use std::cout
   */
  void pose_callback(const turtlesim::msg::Pose::SharedPtr msg) {
    // TODO here!
  }
  // Timer
  rclcpp::TimerBase::SharedPtr timer;
  // Velocity publisher
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_pub;
  // Position subscriber
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr position_sub;

  // Simulation parameters
  const float dt = 1e-3;  // timer_callback is called every 0.001 seconds [if
                          // changed, remember to change the timer period]
  float current_time = 0.0f;
  bool state = 0;  // 0 forward - 1 turn
  // Trajectory parameters
  float side_length = 2;
  float angle_to_turn = pi * 0.5;  // To turn of 90 degrees
  // Pre-set velocities.
  float trans_vel = 1.0;
  float rot_vel = 1.0;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtleSquareNode>());
  rclcpp::shutdown();
  return 0;
}
