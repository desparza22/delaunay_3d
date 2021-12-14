#ifndef PLANE_3D_HPP
#define PLANE_3D_HPP

#include <iostream>
#include <vector>
#include <cassert>
#include "point.hpp"
#include "line3d.hpp"

#define SPACE_DIMENSIONS 3

class Plane3D;

class Plane3D {
private:
  Point center;
  Line3D unit_vertical;
  Line3D unit_horizontal;
  Line3D unit_directional;
  bool forwards;

  void rotate(double ratio, bool vert_direction);
  
public:
  Plane3D(Point passed_center, Line3D vertical, Line3D horizontal,
	  bool forwards);

  Point translate_coordinates(double vert_dist_from_center,
			      double horiz_dist_from_center);

  void change_center(Point new_center);
  Point get_center();

  Line3D get_vertical();
  Line3D get_horizontal();
  Line3D get_directional();
  friend std::ostream& operator<<(std::ostream& os, const Plane3D &p);

  void shift_forward(double distance);
  void shift_right(double distance);
  void shift_center(Line3D unit_direction, double distance);
  void rotate_horizontal(double ratio_to_right);
  void rotate_vertical(double ratio_to_up);
};


#endif
