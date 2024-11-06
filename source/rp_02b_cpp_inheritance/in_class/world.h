#pragma once
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "linalg.h"
#include "canvas.h"

struct WorldItem;
struct World;

struct WorldItemVector {
  using ItemType=WorldItem*;
  int _size=0;
  ItemType* _values=0;

  inline int size() const {return _size;}
  void resize(int new_size);
  void pushBack(ItemType item);

  inline const ItemType& operator [](int pos) const {
    if (pos<0||pos>=_size)
      throw std::runtime_error("out of bounds");
    return _values[pos];
  }

  
  inline const ItemType& at(int pos) const {
    if (pos<0||pos>=_size)
      throw std::runtime_error("out of bounds");
    return _values[pos];
  }

  inline ItemType& at(int pos) {
    if (pos<0||pos>=_size)
      throw std::runtime_error("out of bounds");
    return _values[pos];
  }
};


struct WorldItem{
  WorldItem* parent=0; // if 0 is the world:)
  WorldItemVector children;
  Isometry2f pose_in_parent;
  float radius;

  virtual World* getWorld();

  bool isDescendant(const WorldItem* ancestor) const;

  virtual bool canCollide(const WorldItem* other) const;
  
  WorldItem(WorldItem* parent_=0);
  
  Isometry2f poseInWorld() const;
    
  virtual bool collides(const WorldItem* other) const;
  
  virtual void timerTick(float dt);

  virtual void draw(Canvas& canvas) const;
};

struct World: public WorldItem{
  // all elements in the world
  WorldItemVector items;

  World* getWorld() override;

  bool collides(const WorldItem* other) const override;

  const WorldItem* checkCollision(const WorldItem* current);
};
