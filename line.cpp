#include "line.hpp"

Eigen::ArrayXd Line::calculate_slopes(Point a, Point b) {
  assert(a.num_dimensions() == b.num_dimensions() &&
	 "Points forming a line have different dimensions.\n");
  Eigen::ArrayXd slopes(a.num_dimensions());
  for(int dimension = 0; dimension < a.num_dimensions(); dimension++) {
    double slope_in_dimension = b[dimension] - a[dimension];
    slopes[dimension] = slope_in_dimension;
  }

  return slopes;
}

Point Line::gen_second_point(Point passed_reference_point,
			     Eigen::ArrayXd passed_slopes) {
  std::vector<double> second_coordinates;
  second_coordinates.reserve(passed_slopes.size());
  for(int i = 0; i < passed_slopes.size(); i++) {
    second_coordinates.push_back(passed_reference_point[i] +
				 passed_slopes[i]);
  }
  Point created_second_point(second_coordinates);
  return created_second_point;
}

Line::Line(Point passed_reference_point, Eigen::ArrayXd passed_slopes):
  reference_point(passed_reference_point),
  slopes(passed_slopes),
  second_point(gen_second_point(passed_reference_point, passed_slopes)) {
}

Line::Line(Point passed_reference_point, Point other):
  reference_point(passed_reference_point),
  second_point(other),
  slopes(calculate_slopes(passed_reference_point, other)) {
}

Point Line::get_reference_point() {
  return reference_point;
}

Eigen::ArrayXd Line::get_slopes() {
  return slopes;
}

Point Line::eval_at(double value, int dimension) {
  assert((dimension >= 0 && dimension < num_dimensions()) &&
	 "Dimension out of bounds for line.\n");
  assert((slopes[dimension] != 0 || reference_point[dimension] == value) &&
	 "Line cannot reach value at dimension as the corresponding slope is 0.\n");
  double distance = slopes[dimension] == value?
    0.0:
    (value - reference_point[dimension]) / slopes[dimension];
  std::vector<double> point_coordinates;
  for(int dimension = 0; dimension < num_dimensions(); dimension++) {
    double coordinate = reference_point[dimension] + slopes[dimension] * distance;
    point_coordinates.push_back(coordinate);
  }
  return Point(point_coordinates);
}

Line Line::get_unit() {
  double magnitude = get_magnitude();
  Eigen::ArrayXd unit_slopes = slopes / magnitude;
  Line unit_line(reference_point, unit_slopes);
  
  return unit_line;
}

void Line::set_reference_point(Point new_reference_point) {
  assert((new_reference_point.num_dimensions() == num_dimensions()) &&
	 "referece point should have the same number of dimensions as the line.\n");
  this->reference_point = new_reference_point;
}

int Line::num_dimensions() {
  return slopes.size();
}

double Line::get_magnitude() {
  return sqrt((slopes * slopes).sum());
}

void Line::flip() {
  for(int i = 0; i < num_dimensions(); i++) {
    slopes[i] *= -1;
  }
}

double& Line::operator[](int index) {
  return slopes[index];
}

std::ostream& operator<<(std::ostream& os, const Line &l) {
  return os << "[Line: " << l.reference_point << ", " << l.slopes << "]";
}

