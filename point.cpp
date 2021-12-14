#include "point.hpp"

Point::Point(double x, double y, double z):
  Point(std::vector<double> {x, y, z}) {
}

Point::Point(std::vector<double> coordinates):
  coordinates(coordinates) {
}

int Point::num_dimensions() {
  return coordinates.size();
}


double& Point::operator[](int index) {
  return coordinates[index];
}

struct MidpointInfo Point::midpoint(Point other) {
  struct MidpointInfo midpoint_info;
  std::vector<double> mid_coordinates;
  double dist_sqrd = 0;
  for(int i = 0; i < num_dimensions(); i++) {
    double mid_coordinate = (coordinates[i] + other[i]) / 2;
    mid_coordinates.push_back(mid_coordinate);
    
    double coord_dist = mid_coordinate - coordinates[i];
    dist_sqrd += coord_dist * coord_dist;
  }

  midpoint_info.midpoint = std::make_shared<Point>(mid_coordinates);
  midpoint_info.distance_to_either_sqrd = dist_sqrd;
  return midpoint_info;
}

std::ostream& operator<<(std::ostream& os, const Point &p) {
  os << "[Pt: ";
  for(double coordinate: p.coordinates) {
    os << coordinate << ", ";
  }
  return os << "]";
}
