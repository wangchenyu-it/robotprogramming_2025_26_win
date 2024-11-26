#include "dmap_planner.h"
#include <queue>

void DMapPlanner::init(float resolution,
                       float robot_radius,
                       float max_range,
                       const DMap& dmap) {
  max_distance_range=max_range;
  float max_range_in_pixel=max_range/resolution;
  distances=dmap.distances(max_range_in_pixel);
  obstacle_costs.resize(distances.rows, distances.cols);
  float d_slope=max_traversal_cost/(max_range-robot_radius);
  for (int r=0; r<obstacle_costs.rows; ++r) {
    for (int c=0; c<obstacle_costs.cols; ++c) {
      auto distance_in_pixel=distances.at(r,c);
      auto distance_in_meters = distance_in_pixel*resolution;
      float delta_cost=distance_in_meters-robot_radius;
      auto cost=max_traversal_cost;
      if (delta_cost>0) {
        cost=std::max(0.f,max_traversal_cost-delta_cost*d_slope)+resolution;
      }
      obstacle_costs.at(r,c)=cost;
    }
  }
  mapping.resize(obstacle_costs.rows, obstacle_costs.cols, resolution);
  policy_ok=false;
}
  
struct DijkstraPQCompare {
  const DMapPlanner& planner;
  DijkstraPQCompare(const DMapPlanner& plan):
    planner(plan){}
  inline bool operator()(const DMapPlanner::DijkstraCell* a, const DMapPlanner::DijkstraCell* b) const {
    return a->cost>b->cost;
  }
};

using DijkstraFrontier=std::priority_queue<DMapPlanner::DijkstraCell*,std::vector<DMapPlanner::DijkstraCell*>, DijkstraPQCompare>;
  
void DMapPlanner::computePolicy(const Eigen::Vector2f& goal) {
  d_grid.resize(mapping.rows, mapping.cols);
  std::fill(d_grid.values.begin(), d_grid.values.end(), DijkstraCell());
  Vector2i goal_i=mapping.w2g(goal).cast<int>();
  if (! obstacle_costs.inside(goal_i.x(), goal_i.y())) {
    policy_ok=false;
    return;
  }
  DijkstraFrontier frontier(*this);
  auto& goal_cell=d_grid.at(goal_i.x(), goal_i.y());
  goal_cell.parent=&goal_cell;
  goal_cell.cost=0;
  frontier.push(&goal_cell);
  int num_expansions=0;
  bool step_mode=true;
  while (! frontier.empty()) {
    auto current=frontier.top();
    ++num_expansions;
    frontier.pop();
    Vector2i current_pos=d_grid.ptr2rc(current);
    //cerr << "current_pos: " << current_pos.transpose() << "cost: " << current->cost << endl;
    for (int dr=-1; dr<=1; ++dr) {
      for (int dc=-1; dc<=1; ++dc) {
        if(dr==0 && dc==0)
          continue;
        Vector2i d_pos(dr,dc);
        Vector2i next_pos=current_pos+d_pos;
        if (! obstacle_costs.inside(next_pos.x(),next_pos.y())) {
          continue;
        }
        float traversal_cost=traversalCost(current_pos, next_pos);
        if (traversal_cost>=max_traversal_cost) {
          continue;
        }
        float expected_cost=current->cost+traversal_cost;
        auto& next=d_grid.at(next_pos.x(),next_pos.y());
        if (expected_cost<next.cost) {
          next.parent=current;
          next.cost=expected_cost;
          frontier.push(&next);
        } 
      }
    }
  }
  policy.resize(mapping.rows, mapping.cols);
  std::fill(policy.values.begin(), policy.values.end(), -1);
  for(size_t i=0; i<d_grid.values.size(); ++i) {
    const auto& cell=d_grid.values[i];
    if (! cell.parent)
      continue;
    policy.values[i]=d_grid.values[i].cost;
  }
  policy_gradients=policy.gradient();
  cerr << "num_expansions: " << num_expansions << endl;
  policy_ok=true;
}


float  DMapPlanner::computePath(std::list<Vector2f>& path,
                                const Eigen::Vector2f& start_pose,
                                float approach_cost,
                                size_t max_path_length,
                                bool use_gradient) const {
  path.clear();
  if (! policy_ok)
    return -1;
  if (approach_cost<0)
    approach_cost=mapping.resolution*2;

  if (use_gradient) {
    auto current=start_pose;
    while (path.size()<max_path_length) { 
      path.push_back(current);
      auto pgf=mapping.w2g(current);
      auto pg=pgf.cast<int>();
      if (! policy.inside(pg.x(), pg.y())) {
        cerr << "outside, abort " << pg.transpose() << " current: " << current.transpose() << endl;
        return -1;
      }
      float cost=policy.at(pg.x(), pg.y());
      auto gradient=policy_gradients.atBilinear(pg.x(), pg.y()).first;
      if (cost<0) {
        cerr << "size: " << path.size() << "pos: " << current.transpose() << "gradient: " << gradient.transpose() << " cost : "<< cost << endl;
        cerr << "abort(cost)" << endl;
        path.clear();
        return -1;
      }
      if (cost<approach_cost) {
        //cerr << "close(reach)" << endl;
        return 0;
      }
      if (gradient.squaredNorm()<1e-5) {
        cerr << "abort(no gradient)" << endl;
        return -1;
      }
      gradient.normalize();
      current-=gradient*mapping.resolution;
    }
    return -1;
  } else {
    auto pgf=mapping.w2g(start_pose);
    auto pg=pgf.cast<int>();
    if (! d_grid.inside(pg.x(), pg.y())) {
      cerr << "path outside" << pg.transpose() << " " << start_pose.transpose() << endl;
      return -1;
    }
    auto current=&d_grid.at(pg.x(), pg.y());
    if (! current->parent) {
      cerr <<  "no_parent" << endl;
      return -1;
    }
  
    while (current->parent!=current && current->cost>approach_cost) {
      path.push_back(mapping.g2w(d_grid.ptr2rc(current).cast<float>()));
      current=current->parent;
    }
    return 0;
  }
}


    
