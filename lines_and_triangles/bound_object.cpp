
BoundObject::BoundObject(std::vector<SidedLine> bounds) {
  this->bounds = bounds;
}

bool BoundObject::is_point_in_bounds(float x_coor, float y_coor) {
  bool is_in_bounds = true;
  for(SidedLine bound: bounds) {
    if(!bound.is_point_onside(x_coor, y_coor)) {
      is_in_bounds = false;
    }
  }

  return is_in_bounds;
}
