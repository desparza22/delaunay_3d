#ifndef BOUND_OBJECT_HPP
#define BOUND_OBJECT_HPP

class BoundObject;

class BoundObject {
private:
  std::vector<SidedLine> bounds;

public:
  BoundObject(std::vector<SidedLine> bounds);
  bool is_point_in_bounds(float x_coor, float y_coor);
};


#endif
