#include "world.h"
#include <iostream>
using namespace std;

World::World(float resolution_):
  resolution(resolution_),
  inv_res(1./resolution_){
  for (int i=0; i<MAX_ITEMS; ++i)
    items[i]=0;
  num_items=0;
}

void World::loadFromImage(const char* filename) {
  cerr << "loading [" << filename << "]" << endl;
  cv::Mat m=cv::imread(filename);
  if (m.rows==0) {
    throw std::runtime_error("unable to load image");
  }
  cv::cvtColor(m, _display_image, cv::COLOR_BGR2GRAY);
  size=_display_image.rows*_display_image.cols;
  grid=new uint8_t[_display_image.rows*_display_image.cols];
  rows=_display_image.rows;
  cols=_display_image.cols;
  memcpy(grid, _display_image.data, size);
}
                       
  // inefficency award
bool World::traverseGrid(int& r, int& c, float& range, float alpha) {
    Point p(r,c);
    Point dp(cos(alpha), sin(alpha));
    while (isInside(p.x,p.y) && range>0) {
      if (at(p.x, p.y)<127) {
        r=p.x;
        c=p.y;
        return true;
      }
      p+=dp;
      range-=1;
    }
    r=p.x;
    c=p.y;
    return false;
  }

  
void World::show() {
  memcpy(_display_image.data, grid, size);
  for (int i=0; i<num_items; ++i)
    if (items[i])
      items[i]->draw();
  
  cv::imshow("grid", _display_image);
}


void World::timeTick(float dt){
  for (int i=0; i<num_items; ++i)
    if (items[i])
      items[i]->timeTick(dt);
}

