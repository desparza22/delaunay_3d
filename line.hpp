#ifndef LINE_HPP
#define LINE_HPP

#include <Eigen/Core>
#include <vector>
#include <cassert>
#include <math.h>
#include <iostream>

#include "point.hpp"

class Line;

class Line {
protected:
  Point reference_point;
  Point second_point;
  Eigen::ArrayXd slopes;

  static Eigen::ArrayXd calculate_slopes(Point a, Point b);

  Point gen_second_point(Point passed_referece_point,
			 Eigen::ArrayXd passed_slopes);

public:
  Line(Point passed_reference_point, Eigen::ArrayXd slopes);
  Line(Point passed_reference_point, Point other);

  Point get_reference_point();
  Eigen::ArrayXd get_slopes();
  Point eval_at(double value, int dimension);
  Line get_unit();
  void set_reference_point(Point new_reference_point);
  int num_dimensions();
  double get_magnitude();

  void flip();
  
  double& operator[](int index);
  friend std::ostream& operator<<(std::ostream& os, const Line &l);
};

#endif
