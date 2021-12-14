#ifndef LINE_2D_HPP
#define LINE_2D_HPP

#include <Eigen/Core>
#include <cassert>

#include "point.hpp"
#include "line.hpp"

class Line2D;

class Line2D: public Line {
public:
  Line2D(Point passed_reference_point, Eigen::ArrayXd slopes);
  Line2D(Point passed_reference_point, Point other);
  Line2D(Line line_in_r2);
  
  Line2D get_perpendicular();
};

#endif
