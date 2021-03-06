#ifndef LINE_3D_HPP
#define LINE_3D_HPP

#include <Eigen/Geometry>
#include <Eigen/Core>
#include <cassert>
#include <vector>

#include "line.hpp"
#include "point.hpp"

class Line3D;


class Line3D: public Line {
private:
  Eigen::Array3d slopes_cast;
  
public:
  Line3D(Point passed_reference_point, Eigen::ArrayXd slopes);
  Line3D(Point passed_reference_point, Point other);

  Line3D(Line line);
  
  Line3D cross_multiply(Line3D other);

};





#endif
