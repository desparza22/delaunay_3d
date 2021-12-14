#include "screen.hpp"

Screen::Screen(Plane3D plane,
	       double width, double height,
	       double min_dist, double max_dist,
	       double expansion_rate):
  plane(plane),
  width(width), height(height),
  min_dist(min_dist), max_dist(max_dist),
  expansion_rate(expansion_rate) {
}

double Screen::get_min_dist() {
  return min_dist;
}

double Screen::get_max_dist() {
  return max_dist;
}

Eigen::LDLT<Eigen::Matrix3f> Screen::get_solver() {
  if(!solver_constructed) {
    Eigen::Matrix3f vectors;
    int row = 0;
    for(Line directional_vector: std::vector {plane.get_horizontal(),
					      plane.get_vertical(),
					      plane.get_directional()}) {
      vectors(row, 0) = directional_vector[0];
      vectors(row, 1) = directional_vector[1];
      vectors(row, 2) = directional_vector[2];
      row++;
    }

    solver = vectors.ldlt();
    solver_constructed = true;
  }
  return solver;
}

Point Screen::r3_to_r2(Point point_in_r3) {
  struct PointRelativity position_relative = get_position_relative(point_in_r3);
  return r3_to_r2(position_relative);
}

bool Screen::is_in_view(Point point_in_r3) {
  struct PointRelativity position_relative =
    get_position_relative(point_in_r3);
  return is_in_view(position_relative);
}


double Screen::distance(Point point_in_r3) {
  struct PointRelativity position_relative =
    get_position_relative(point_in_r3);
  return distance(position_relative);
}

struct PointRelativity Screen::get_position_relative(Point point_in_r3) {
  Eigen::LDLT<Eigen::Matrix3f> solver = get_solver();
  Eigen::Vector3f point_as_vector(point_in_r3[0] - plane.get_center()[0],
				  point_in_r3[1] - plane.get_center()[1],
				  point_in_r3[2] - plane.get_center()[2]);
  Eigen::Vector3f cofactors = solver.solve(point_as_vector);
  struct PointRelativity relative_position;
  relative_position.horizontal_from_center = cofactors[0];
  relative_position.vertical_from_center = cofactors[1];
  relative_position.distance = cofactors[2];
  return relative_position;
}

Point Screen::r3_to_r2(struct PointRelativity position_relative) {
  Point center = plane.get_center();
  double height_of_projected_plane =
    position_relative.distance * expansion_rate + height;
  double width_of_projected_plane =
    position_relative.distance * expansion_rate + width;

  double proportional_dist_to_top =
    (position_relative.vertical_from_center) /
    (height_of_projected_plane / 2.0);
  double proportional_dist_to_right =
    (position_relative.horizontal_from_center) /
    (width_of_projected_plane / 2.0);

  return Point(std::vector<double> {width/2.0 +
				   width/2.0 * proportional_dist_to_right,
				   height/2.0 +
				   height/2.0 * proportional_dist_to_top});
}

bool Screen::is_in_view(struct PointRelativity position_relative) {
  Point point_in_r2 = r3_to_r2(position_relative);
  return is_in_view(position_relative, point_in_r2);
}

bool Screen::is_in_view(struct PointRelativity position_relative,
			Point point_in_r2) {
  return point_in_r2[0] >= 0 && point_in_r2[0] < width &&
    point_in_r2[1] >= 0 && point_in_r2[1] < height &&
    position_relative.distance >= min_dist &&
    position_relative.distance <= max_dist;
}

double Screen::distance(struct PointRelativity position_relative) {
  return position_relative.distance;
}

double Screen::get_width() {
  return width;
}

double Screen::get_height() {
  return height;
}

Point Screen::get_center() {
  return plane.get_center();
}

void Screen::change_center(Point new_center) {
  plane.change_center(new_center);
}

void Screen::shift_forward(double distance) {
  plane.shift_forward(distance);
}

void Screen::shift_right(double distance) {
  plane.shift_right(distance);
}

void Screen::rotate_horizontal(double ratio) {
  solver_constructed = false;
  plane.rotate_horizontal(ratio);
}

void Screen::rotate_vertical(double ratio) {
  solver_constructed = false;
  plane.rotate_vertical(ratio);
}

Plane3D Screen::get_plane() {
  return plane;
}

std::ostream& operator<<(std::ostream& os,
			 const PointRelativity &relative_position) {
  return os << "[PtRel: hor-" << relative_position.horizontal_from_center
	    << ", ver-" << relative_position.vertical_from_center
	    << ", dist-" << relative_position.distance
	    << "]" << std::endl;
}

