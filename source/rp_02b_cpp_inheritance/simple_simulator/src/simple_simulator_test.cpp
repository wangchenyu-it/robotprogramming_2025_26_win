#include <cmath>
#include <cstdint>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "simple_geometry.h"
#include "world.h"
#include "robot.h"
#include "lidar.h"

using namespace std;
  
int main(int argc, char** argv){
  float res=0.05;
  float delay=0.1;
  World w(0.05);
  w.loadFromImage(argv[1]);
  Lidar l(&w);
  l.pose=Pose(res*w.rows/2, res*w.cols/2, 0);
  w.addItem(&l);
  Robot r(&w,0.2);
  w.addItem(&r);
  Lidar lr(&r);
  w.addItem(&lr);
  r.pose=Pose(10, 10, M_PI/2);
  int k;
  while (1) {
    w.timeTick(delay);
    w.show();
    k=cv::waitKeyEx(delay*1000)&255;
    switch (k) {
    case 81: r.rv+=0.05; break;// arow left
    case 82: r.tv+=0.1; break;// arow up
    case 83: r.rv-=0.05; break;// arow right
    case 84: r.tv-=0.1; break;// arow dw
    case 32: r.tv=0; r.rv=0; break;// spacebar
    case 27: return 0;
    default:;
    }
    cerr << "k: " << (int) k << endl;
  }
}

