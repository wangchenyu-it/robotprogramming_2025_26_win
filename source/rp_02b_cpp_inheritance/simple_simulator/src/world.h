#pragma once
#include "simple_geometry.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;

class WorldItem;

class World {
public:
  static constexpr int MAX_ITEMS=100;
  float resolution, inv_res;
  uint8_t* grid=0;
  int rows=0, cols=0, size=0;

  World(float resolution_=0.05);
  
  inline uint8_t at(int r, int c) {
    return grid[r*cols+c];
  }

  inline uint8_t at(const IndexPair& indices) {
    return at(indices.r, indices.c);
  }
  
  inline bool isInside(int r, int c) const {
    return r>=0 && r<rows && c>=0 && c<cols;
  }

  inline uint8_t isInside(const IndexPair& indices) {
    return isInside(indices.r, indices.c);
  }

  inline IndexPair worldToIndices(const Point p) {return IndexPair(p.x*inv_res, p.y*inv_res);}
  
  inline Point indicesToWorld(const IndexPair idx) {return Point(idx.r*resolution, idx.c*resolution);}

  void loadFromImage(const char* filename);
  
  // inefficency award
  bool traverseGrid(int& r, int& c, float& range, float alpha);
  

  void timeTick(float dt);
  
  void addItem(WorldItem* item) {items[num_items]=item; ++num_items;}
  WorldItem* items[MAX_ITEMS];
  int num_items=0;

  // for visualization
  void show();
  cv::Mat _display_image;
};

class WorldItem{
public:
  WorldItem(World* world_=0):
    parent(0),
    world(world_)
    {}
  
  WorldItem(WorldItem* parent_=0):
    parent(parent_),
    world(parent_->world){}
  WorldItem* parent;
  World* world;

  inline Pose poseInWorld()  const{
    Pose p;
    const WorldItem* aux=this;
    while (aux) {
      p=aux->pose*p;
      aux=aux->parent;
    }
    return p;
  }
  Pose pose;
  virtual void draw(){};
  virtual void timeTick(float dt)=0;
};

