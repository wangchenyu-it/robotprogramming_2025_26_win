#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "dmap.h"
#include "grid_mapping.h"
#include <list>

struct DMapPlanner {
  GridMapping mapping; // to convert from world to grid coordinates
  Grid_<float> obstacle_costs; // cost of a cell based on distance
  Grid_<float> distances; // useless, kept only for visualization
  Grid_<float> policy; // surface pointing to the goal
  Grid_<Vector2f> policy_gradients; // surface pointing to the goal
  float max_traversal_cost=100;
  float robot_radius=0.3;
  float max_distance_range=5;

  
  struct DijkstraCell{
    DijkstraCell(DijkstraCell* p=0,
                 float c=std::numeric_limits<float>::max()):
      parent(p),
      cost(c)
    {}
    DijkstraCell* parent;
    float cost;
  };

  using DijkstraGrid=Grid_<DijkstraCell>;
  DijkstraGrid d_grid;
  
  inline float traversalCost(const Vector2i& from, const Vector2i& to) const {
    return sqrt((from-to).squaredNorm()) * obstacle_costs.at(to.x(), to.y());
  }

  void init(float resolution,
            float robot_radius,
            float max_range,
            const DMap& dmap);
  
  void computePolicy(const Eigen::Vector2f& goal);

  float  computePath(std::list<Vector2f>& path,
                     const Eigen::Vector2f& start_pose,
                     float approach_cost=-1,
                     size_t max_path_length=10000,
                     bool use_gradient=true) const;

  bool policy_ok=false;
};
