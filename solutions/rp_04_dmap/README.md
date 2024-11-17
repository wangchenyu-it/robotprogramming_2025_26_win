# Distance map exercise solution

### Compilation
```sh
pixi run colcon build
```

### Run the simulator
```sh
pixi run ros2 run rp_simulator simulator --ros-args -p config_file:=assets/diag_single_robot.yaml
```

### Run the distance map test node
```sh
pixi run ros2 run rp_commons test_distance_map_node
```

### Run Rviz2
```sh
pixi run rviz2 rviz2 -d rviz_configs/diag_single_robot_dmap.rviz 
```
