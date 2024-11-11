#include "dmap_simple.h"
#include <cstdlib>
#include <fstream>

using namespace std;
  
int main(int argc, char**argv) {
  int rows=480;
  int cols=640;
  int num_obstacles=100;
  int iterations=1000;
  DistanceMap dmap;
  dmap.resize(rows, cols);
  std::vector<std::pair<int, int> > obstacles(num_obstacles);
  for (auto& p: obstacles) {
    p.first=(int) (drand48()*(rows-1));
    p.second=(int) (drand48()*(cols-1));
    //cerr << "[" << p.first << " " << p.second << "]" << endl;
  }
  for (int i=0; i<iterations; ++i) {
    cout <<"\r" << i <<"/" <<iterations << " ";
    std::fill(dmap.storage().begin(), dmap.storage().end(), DMapCell());
    dmap.compute(obstacles, 30);
  }

  auto out=dmap.toDistances();
  std::ofstream os("out.dat");
  os << out;
}
