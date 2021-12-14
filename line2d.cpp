#include "line2d.hpp"


Line2D::Line2D(Point passed_reference_point, Eigen::ArrayXd slopes):
  Line(passed_reference_point, slopes) {
  assert(slopes.size() == 2 &&
	 passed_reference_point.num_dimensions() == 2 &&
	 "Parameters should have dimension 2. num_dimensions() indicates otherwise.");
}

Line2D::Line2D(Point passed_reference_point, Point other):
  Line(passed_reference_point, other) {
  assert(num_dimensions() == 2 &&
	 "Parameters should have dimension 2. num_dimensions() indicates otherwise.");
}

Line2D::Line2D(Line line_in_r2):
  Line2D(line_in_r2.get_reference_point(), line_in_r2.get_slopes()) {
}

Line2D Line2D::get_perpendicular() {
  double x_slope = slopes[0];
  double y_slope = slopes[1];

  Eigen::Array2d inverse_slopes(y_slope, -x_slope);
  return Line2D(reference_point, inverse_slopes);
}
