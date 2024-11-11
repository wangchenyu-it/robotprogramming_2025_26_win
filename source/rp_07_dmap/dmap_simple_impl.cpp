int DistanceMap::d2(const DMapCell* c1,
                    const DMapCell* c2) const {
  auto p1=ptr2rc(c1);
  auto p2=ptr2rc(c2);
  const int dr = p1.first-p2.first;
  const int dc = p1.second-p2.second;
  return dr*dr+dc*dc;
}
  
int DistanceMap::d2(const DMapCell* c) const {
  if (! c->parent)
    return std::numeric_limits<int>::max();
  return d2(c, c->parent);
}
