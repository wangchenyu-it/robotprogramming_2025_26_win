# rp/_03/_ros\_basics
In this exercise, you will put hands in the classical publisher/_subscriber structure. To make this as easy as possible, we will collaborate with our friendly turtles provided by `turtlesim` via `ROS 2`. You may verify that you have turtlesim by typing:
```bash
ros2 run turtlesim turtlesim_node
```
If you incurr in a `package not found` error, then you probably need to download it manually.
For Linux based machines, write:
```bash
sudo apt install ros-<$DISTRO>-turtlesim
```
where `<$DISTRO>` is your ROS 2 distribution (e.g., _humble_ or _jazzy_ )

For the exercise, you are tasked to build a node for the package `rp_simple_controller` that allows a simulated turtle to follow a square trajectory.

Starting from the basics, when born, the turtle will latch to the following topics:

-   **SUBSCRIBED TO**
    -   `/turtle1/cmd_vel` : \[[geometry_msgs/msg/Twist](https://docs.ros2.org/foxy/api/geometry_msgs/msg/Twist.html)\] Input velocity commands
-   **PUBLISH TO**
    -   `/turtle1/pose`: \[[turtlesim/msg/Pose](https://docs.ros2.org/foxy/api/turtlesim/msg/Pose.html)\] Output pose of the turtle
    -   `/turtle1/color_sensor`: [turtlesim/msg/Color] ~~Output sensor measurement~~ (unused)

Our task is to:

1. Send velocity commands to drive the turtle in a square trajectory
2. Read the turtle pose and emit a message on the terminal

PS: _check for `TODO: here!` comments across the package to get insights on where to write ;)

Most of the coding details can be found on the source file `src/rp_simple_controller/src/turtle_square.cpp`.

Before working on the C++ aspect, take care about the `CMake` and `package.xml`.

To produce this node we will need to resolve some dependencies, namely:

- **rclcpp**
- **geometry_msgs**
- **turtlesim**

Be sure to include these dependencies in the `CMakeLists` and in the `package.xml`.
You will find tips in the CMakeLists file while for `package.xml` stick with these suggestions:

_Each dependency should be included after the `<buildtool_depend>ament_cmake</buildtool_depend>` line. Each dependency is defined by `<depend>$DEPENDENCY</depend>` line, where $DEPENDENCY is the package you're depending from._
For instance, since we depend from `rclcpp`, we should add:

- `<depend>rclcpp</depend>`

After you fix up these two files, please head to the C++ file to complete the task!

Once you are ready, place your working directory on top of the colcon workspace (AKA where this file is placed) and type the following to build the package:

```bash
colcon build
```

If no errors occur, you may source the workspace 

```bash
source install/setup.sh
# OR if you are using ZSH (you guys from OSX )
source install/setup.zsh
```

## Launch instructions

First, we should spawn our friendly turtle:

```bash
ros2 run turtlesim turtlesim_node
```

You should see a turtle spawning up in a GUI.

Once the turtle is ready, on a new terminal with sourced workspace, type:

```bash
ros2 run rp_simple_controller turtle_square
```

If the topics are latched well and, you did your code correctly, you should see the turtle following a square trajectory!

As always, if you encounter compatibility issues on code you didn't write, please let us know!

