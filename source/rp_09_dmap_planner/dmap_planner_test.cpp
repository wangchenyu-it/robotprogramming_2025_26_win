#include <iostream>
#include "dmap_planner.h"
#include "display_utils.h"

using namespace std;

std::list<Vector2f> path;
cv::Mat background_image;
cv::Mat shown_image;
bool show_policy=false;
bool use_gradient=false;

void redisplay(const GridMapping& mapping) {
  background_image.copyTo(shown_image);
  drawPoints(shown_image, mapping, Eigen::Isometry2f::Identity(), path, 255);
  cv::imshow("planner", shown_image);
}

static void onMouse(int event, int x, int y, int val, void* arg) {
  DMapPlanner* planner=static_cast<DMapPlanner*>(arg);
  Eigen::Vector2f world_pose=planner->mapping.g2w(Eigen::Vector2f(y,x));
  
  if( event == cv::EVENT_RBUTTONDOWN ) {
    cerr << "grid_pose: " << x << " " << y << " world_pose: " << world_pose.transpose() << endl;
    cerr << "compute policy " << endl;
    planner->computePolicy(world_pose);
    redisplay(planner->mapping);
  } else {
    planner->computePath(path,
                         world_pose,
                         planner->mapping.resolution*2,
                         10000,
                         use_gradient);
    redisplay(planner->mapping);
  }
  
  if (planner->policy_ok && show_policy) {
    Eigen::Vector2i parent_pos(-10000, -10000);
    auto &d_cell=planner->d_grid.at(y,x);
    if (d_cell.parent)
      parent_pos=planner->d_grid.ptr2rc(d_cell.parent);
    cerr << "\r policy: " << y << " " << x << " val: " << planner->policy.at(y,x) << " parent: " << parent_pos.transpose() << "             ";
  } else {
    cerr << "\r cost: " << planner->obstacle_costs.at(y,x) << "        ";
  }
      
} 

int main(int argc, char** argv) {
  int rows=500, cols=700;
  int n_points=20;
  float resolution=0.05;
  
  //1 compute dmap
  DMap dmap(rows, cols);
  dmap.clear();
  Vector2iVector obstacles(n_points);
  for (auto& o: obstacles) {
    o=Vector2i(drand48()*rows, drand48()*cols);
  }
  dmap.update(obstacles);


  DMapPlanner planner;
  planner.init(resolution, 0.3, 1, dmap);
  planner.computePolicy(Vector2f::Zero());
  cerr << "planner ready" << endl;
  cv::namedWindow("planner", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
  cv::setMouseCallback("planner", onMouse, &planner);
  while (1) {
    int key=cv::waitKey();
    if (key==' ') {
      show_policy=!show_policy;
      if (show_policy) {
        background_image=grid2cv(planner.policy, true);
      } else {
        background_image=grid2cv(planner.obstacle_costs, true);
      }
    } else {
      if (key=='g')
        use_gradient=!use_gradient;
    }
    if (planner.policy_ok)
      redisplay(planner.mapping);
  }
}
