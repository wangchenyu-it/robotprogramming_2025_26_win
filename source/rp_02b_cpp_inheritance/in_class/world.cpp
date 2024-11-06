#include "world.h"

void WorldItemVector::resize(int new_size) {
  if (_size==new_size)
    return;
    
  ItemType * old_values=_values;
  _values=0;
    
  if (new_size) {
    _values=new ItemType[new_size];
    int copy_index=std::min(new_size, _size);
    for (int i=0; i<copy_index; ++i)
      _values[i]=old_values[i];
  }
  _size=new_size;
  delete [] old_values;
}

void WorldItemVector::pushBack(ItemType new_item) {
  resize(_size+1);
  at(_size-1)=new_item;
}


World* WorldItem::getWorld() {
  if (! parent)
    return static_cast<World*>(this);
  return parent->getWorld();
}

  
bool WorldItem::isDescendant(const WorldItem* ancestor) const {
  WorldItem* aux=parent;
  while (aux) {
    if (aux==ancestor)
      return true;
    aux=aux->parent;
  }
  return false;
}

bool WorldItem::canCollide(const WorldItem* other) const {
  return (! this->isDescendant(other) &&
          ! other->isDescendant(this));
}

WorldItem::WorldItem(WorldItem* parent_):
  pose_in_parent(0,0,0){
  if (parent_){
    parent=parent_;
    parent->children.pushBack(this);
  }
  World* w=getWorld();
  if (w==this)
    return;
  if (!w)
    throw std::runtime_error("no world");
  
  w->items.pushBack(this);
}
  
Isometry2f WorldItem::poseInWorld() const {
  if (! parent)
    return pose_in_parent;
  return parent->poseInWorld()*pose_in_parent;
}

bool WorldItem::collides(const WorldItem* other) const {
  Isometry2f my_pose_in_world= poseInWorld(); 
  Isometry2f other_pose_in_world= other->poseInWorld();
  Scalar distance=(my_pose_in_world.t-other_pose_in_world.t).norm();
  return  distance < (radius + other->radius);
}

void WorldItem::timerTick(float dt) {
  for (int i=0; i<children.size(); ++i)
    if (children.at(i))
      children.at(i)->timerTick(dt);
}

bool World::collides(const WorldItem* other) const {
  cerr << "AAAARGHHHHH" << endl;
  return false;
}

const WorldItem* World::checkCollision(const WorldItem* current) {
  for (int i=0; i<items.size(); ++i) {
    const WorldItem* o=items.at(i);
    if (o
        && o!=current
        && o->canCollide(current)
        && o->collides(current))
      return o;
  }
  return 0;
}

void WorldItem::draw(Canvas& canvas) const {
  for (int i=0; i<children.size(); ++i) {
    const WorldItem* o=children.at(i);
    if (o)
      o->draw(canvas);
  }
}

World* World::getWorld() {
  return this;
}

