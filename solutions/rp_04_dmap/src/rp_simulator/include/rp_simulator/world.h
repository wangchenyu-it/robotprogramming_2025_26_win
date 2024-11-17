#pragma once

#include <tf2_ros/transform_broadcaster.h>

#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <set>

#include "rp_commons/grid_map.h"
#include "world_item.h"

/**
 * @brief The World class represents a simulated world.
 *
 * This class inherits from WorldItem and manages the simulation of a world,
 * including ticking the world state and publishing the occupancy grid map.
 */
class World : public WorldItem {
 public:
  /**
   * @brief Constructs a new World object.
   *
   * @param gmap The grid map representing the world.
   * @param node The ROS node to use for timers and publishers.
   * @param tick_interval The interval at which the world state is updated.
   */
  World(const GridMap &gmap, rclcpp::Node::SharedPtr node, float tick_interval)
      : WorldItem(gmap, "", node), _tick_interval(tick_interval) {
    // Create a timer inside World for ticking the world
    _timer_tick =
        _node->create_wall_timer(std::chrono::duration<float>(_tick_interval),
                                 std::bind(&World::clock, this));

    // Create a timer inside World for publishing the map
    _timer_map =
        _node->create_wall_timer(std::chrono::duration<float>(_map_interval),
                                 std::bind(&World::publishMap, this));

    // Create a publisher for the occupancy grid map
    _occupancy_grid_pub =
        _node->create_publisher<nav_msgs::msg::OccupancyGrid>("map", 10);
  }

  /**
   * @brief Ticks the world and all its children.
   *
   * This function is called periodically by the timer to update the world
   * state.
   */
  void clock() {
    WorldItem::tick(_tick_interval,
                    _node->now());  // Call the base class tick method
  }

  /**
   * @brief Publishes the occupancy grid map.
   *
   * This function is called periodically by the timer to publish the current
   * state of the occupancy grid map.
   */
  void publishMap() {
    // Create an OccupancyGrid message and fill it with the grid map data
    nav_msgs::msg::OccupancyGrid occupancy_grid;
    occupancy_grid.header.stamp = _node->now();
    occupancy_grid.header.frame_id = "map";

    occupancy_grid.info.map_load_time = _node->now();
    occupancy_grid.info.resolution = _grid_map->_resolution;
    occupancy_grid.info.width = _grid_map->_cols;
    occupancy_grid.info.height = _grid_map->_rows;

    // Set the origin position of the grid map
    occupancy_grid.info.origin.position.x = _grid_map->_origin.x();
    occupancy_grid.info.origin.position.y = _grid_map->_origin.y();

    // Resize the data vector to fit the grid map dimensions
    occupancy_grid.data.resize(_grid_map->_rows * _grid_map->_cols);

    // Fill the occupancy grid data
    for (size_t r = 0; r < _grid_map->_rows; ++r) {
      for (size_t c = 0; c < _grid_map->_cols; ++c) {
        uint8_t grayscale_value =
            _grid_map->at(r, c);  // Get the grayscale value at the current cell

        // Calculate the index in the occupancy grid data vector
        size_t index =
            occupancy_grid.info.width * (occupancy_grid.info.height - r - 1) +
            c;

        // Convert the grayscale value to occupancy grid value
        switch (grayscale_value) {
          case GridMap::FREE:
            occupancy_grid.data[index] = 0;  // Free cell
            break;
          case GridMap::OCCUPIED:
            occupancy_grid.data[index] = 100;  // Occupied cell
            break;
          case GridMap::UNKNOWN:
            occupancy_grid.data[index] = -1;  // Unknown cell
            break;
          default:
            occupancy_grid.data[index] = -1;  // Default to unknown cell
            break;
        }
      }
    }

    _occupancy_grid_pub->publish(occupancy_grid);  // Publish the occupancy grid
  }

  /**
   * @brief Draws the world onto the given canvas.
   *
   * This function draws the grid map on the provided canvas. It also
   * recursively draws all child elements of the world.
   *
   * @param canvas The canvas on which to draw the world.
   */
  void draw(Canvas &canvas) const {
    _grid_map->draw(canvas);

    for (auto _child : _children) _child->draw(canvas);
  }

 protected:
  rclcpp::TimerBase::SharedPtr _timer_tick;  // Timer for ticking the world
  float _tick_interval;                      // Time interval for each tick

  rclcpp::TimerBase::SharedPtr _timer_map;  // Timer for publishing the map
  float _map_interval = 1.0;                // Time interval for publishing map

  rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr
      _occupancy_grid_pub;
};