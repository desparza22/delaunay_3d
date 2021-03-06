#include "line3d.hpp"

Line3D::Line3D(Point passed_reference_point, Eigen::ArrayXd slopes):
  Line(passed_reference_point, slopes) {
  assert(passed_reference_point.num_dimensions() == 3 &&
	 slopes.size() == 3 &&
	 "Line3D only accepts points and slopes with 3 dimensions.");

  slopes_cast = Eigen::Array3d(slopes[0], slopes[1], slopes[2]);
}

Line3D::Line3D(Point passed_reference_point, Point other):
  Line(passed_reference_point, other) {
  slopes_cast = Eigen::Array3d(slopes[0], slopes[1], slopes[2]);
}

Line3D::Line3D(Line line):
  Line3D(line.get_reference_point(), line.get_slopes()) {
  
}

Line3D Line3D::cross_multiply(Line3D other) {
  Eigen::Vector3d slopes_as_matrix = slopes_cast.matrix();
  Eigen::Vector3d other_slopes_as_matrix = other.slopes_cast.matrix();
  
  Eigen::Vector3d cross_with_other =
    slopes_as_matrix.cross(other_slopes_as_matrix);
  Eigen::Array3d cross_as_array = cross_with_other.array();
  return Line3D(reference_point, cross_as_array);
}
