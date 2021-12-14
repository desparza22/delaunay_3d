#include "plane3d.hpp"


Plane3D::Plane3D(Point passed_center,
		 Line3D horizontal,
		 Line3D vertical,
		 bool forwards):
  center(passed_center),
  unit_horizontal(horizontal.get_unit()),
  unit_vertical(vertical.get_unit()),
  unit_directional(unit_vertical.cross_multiply(unit_horizontal)),
  forwards(forwards) {

  assert(center.num_dimensions() == SPACE_DIMENSIONS &&
	 "Center point, vertical and horizontal lines should have the same dimensions\n.");
  
  unit_horizontal.set_reference_point(center);
  unit_vertical.set_reference_point(center);
  unit_directional.set_reference_point(center);
  if(!forwards) {
    unit_directional.flip();
  }
}


Point Plane3D::translate_coordinates(double vert_dist_from_center,
				     double horiz_dist_from_center) {
  std::vector<double> point_coordinates;
  for(int dimension = 0; dimension < SPACE_DIMENSIONS; dimension++) {
    double coordinate = center[dimension] +
      unit_vertical[dimension] * vert_dist_from_center +
      unit_horizontal[dimension] * horiz_dist_from_center;
    
    point_coordinates.push_back(coordinate);
  }
  return Point(point_coordinates);
}

void Plane3D::change_center(Point new_center) {
  assert((new_center.num_dimensions() == SPACE_DIMENSIONS) &&
	 "Center point must maintain the same dimensions as the old line.\n");
  this->center = new_center;
  this->unit_vertical.set_reference_point(new_center);
  this->unit_horizontal.set_reference_point(new_center);
}

Point Plane3D::get_center() {
  return center;
}

Line3D Plane3D::get_vertical() {
  return unit_vertical;
}

Line3D Plane3D::get_horizontal() {
  return unit_horizontal;
}

Line3D Plane3D::get_directional() {
  return unit_directional;
}

void Plane3D::shift_forward(double distance) {
  shift_center(get_directional(), distance);
}


void Plane3D::shift_right(double distance) {

}

void Plane3D::shift_center(Line3D unit_direction, double distance) {
  for(int i = 0; i < 3; i++) {
    center[i] += unit_direction[i] * distance;
  }
}

void Plane3D::rotate_horizontal(double ratio) {
  rotate(ratio, false);
}

void Plane3D::rotate_vertical(double ratio) {
  rotate(ratio, true);
}

void Plane3D::rotate(double ratio, bool vert_direction) {
  assert(ratio >= -1 && ratio <= 1 &&
	 "Ratio should be between -1 and 1");
  
  double abs_ratio = (ratio < 0)? ratio * -1: ratio;
  double compliment = 1.0 - abs_ratio;
  Line3D unit_side = vert_direction? unit_vertical: unit_horizontal;
  for(int i = 0; i < 3; i++) {
    unit_side[i] = unit_side[i] * compliment - unit_directional[i] * ratio;
  }
  if(vert_direction) {
    unit_vertical = unit_side.get_unit();
  } else {
    unit_horizontal = unit_side.get_unit();
  }

  unit_directional = unit_vertical.cross_multiply(unit_horizontal).get_unit();
  if(!forwards) {
    unit_directional.flip();
  }
}

std::ostream& operator<<(std::ostream &os, const Plane3D &p) {
  return os << "[Plane: " << p.center << " " << p.unit_vertical << " " << p.unit_horizontal << " " << p.unit_directional << "]";
}
