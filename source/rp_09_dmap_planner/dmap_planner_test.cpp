#include <iostream>
#include "dmap_planner.h"
#include "display_utils.h"

using namespace std;

std::list<Vector2f> path;
cv::Mat background_image;
cv::Mat shown_image;

int background_mode=0; // 0: DMap, 1:Obstacles, 2: Policy

bool use_gradient=false;

void recomputeBackground(DMapPlanner& planner, int new_mode) {
  if (new_mode==background_mode)
    return;
  if (new_mode==-1)
    new_mode=background_mode;
  background_mode=new_mode;
  switch(background_mode){
  case 0:
    background_image=grid2cv(planner.distances, true);
    break;
  case 1:
    background_image=grid2cv(planner.obstacle_costs, true);
    break;
  case 2:
    background_image=grid2cv(planner.policy, true);
    break;
  default:;
  }
}
void redisplay(const GridMapping& mapping) {
  if (background_image.rows==0 || background_image.cols==0)
    return;
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

  cerr << "background_mode= " << background_mode << endl;
  switch(background_mode) {
  case 0:
    cerr << "\r distance: " << planner->distances.at(y,x)*planner->mapping.resolution << "        ";
    break;
  case 1:
    cerr << "\r cost: " << planner->obstacle_costs.at(y,x) << "        ";
    break;
  case 2:
    Eigen::Vector2i parent_pos(-10000, -10000);
    auto &d_cell=planner->d_grid.at(y,x);
    if (d_cell.parent)
      parent_pos=planner->d_grid.ptr2rc(d_cell.parent);
    cerr << "\r policy: " << y << " " << x << " val: " << planner->policy.at(y,x) << " parent: " << parent_pos.transpose() << "             ";
    break;
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
  float expansion_range=1;
  planner.init(resolution, 0.3, expansion_range, dmap);
  planner.computePolicy(Vector2f::Zero());
  cerr << "planner ready" << endl;
  cv::namedWindow("planner", cv::WINDOW_GUI_NORMAL | cv::WINDOW_AUTOSIZE);
  background_image=grid2cv(planner.policy, true);
  redisplay(planner.mapping);
  cv::setMouseCallback("planner", onMouse, &planner);
  while (1) {
    int key=cv::waitKey();
    switch(key) {
    case ' ':
      recomputeBackground(planner, (background_mode+1)%3);
      break;
    case 'g':
      use_gradient=!use_gradient;
      redisplay(planner.mapping);
      break;
    case '+':
      expansion_range+=1;
      planner.init(resolution, 0.3, expansion_range, dmap);
      planner.computePolicy(Vector2f::Zero());
      recomputeBackground(planner, -1);
      break;
    case '-':
      expansion_range-=1;
      expansion_range=std::max(1.f, expansion_range);
      planner.init(resolution, 0.3, expansion_range, dmap);
      planner.computePolicy(Vector2f::Zero());
      recomputeBackground(planner, -1);
      break;
    }
    if (planner.policy_ok)
      redisplay(planner.mapping);
  }
}
