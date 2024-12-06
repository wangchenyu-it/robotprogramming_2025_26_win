#include <iostream>
#include "dmap_localizer.h"
#include "display_utils.h"

using namespace std;

int main(int argc, char** argv) {
  int rows=500, cols=700;
  int n_points=30;
  float resolution=0.01;
  
  //1 compute dmap
  DMap dmap(rows, cols);
  dmap.clear();
  Vector2iVector obstacles(n_points);
  for (auto& o: obstacles) {
    o=Vector2i(3+drand48()*(rows-6), 3+(drand48()*cols-6));
    
  }
  dmap.update(obstacles);

  //2 for display purpose map the obstacles from grid to world
  GridMapping gm(rows, cols, resolution);
  Vector2fVector obstacles_in_world(obstacles.size());
  for (size_t i=0; i<obstacles.size(); ++i) {
    obstacles_in_world[i]=gm.g2w(obstacles[i].cast<float>());
  }

  
  //2 express points in grid in points in robot reference
  Eigen::Isometry2f riw;
  riw.linear()=Eigen::Rotation2Df(0.03).toRotationMatrix();
  riw.translation()=Vector2f(0.3,-0.2);
  Eigen::Isometry2f wir=riw.inverse();

  
  Vector2fVector zs(n_points);
  for (size_t i=0; i<obstacles_in_world.size(); ++i) {
    zs[i]=wir*obstacles_in_world[i];
  }

  DMapLocalizer loc;
  loc.point_damping=1;
  loc.kernel_threshold=1e9;
  loc.use_bilinear=true;
  loc.init(resolution, 10, dmap);
  loc.reset();

  cerr << "loc ready" << endl;
  cv::Mat background_image=grid2cv(loc.distances, true);
  drawPoints(background_image,
             gm,
             Eigen::Isometry2f::Identity(),
             obstacles_in_world,
             255);
  
  while (1) {
    cv::Mat shown_image;
    background_image.copyTo(shown_image);
    drawPoints(shown_image,
               loc.mapping,
               loc.X,
               zs,
               127);
    cv::imshow("image", shown_image);
    cv::waitKey();
    int iterations=1;
    for (int i=0; i<iterations; ++i) {
      loc.reset(loc.X);
      for (const auto& z: zs) {
        loc.addMeasurement(z);
      }
      loc.update();
      cerr << "Loc chi2: " << loc.chi2 << " dx : " << loc.dx.transpose() << " inl: " << loc.num_inliers << endl;
    }
    cerr << "X:" << loc.X.matrix() << endl;
    cerr << "X_gt:" << riw.matrix() << endl;
  }
}
