#ifndef POINT_HPP
#define POINT_HPP

#include <vector>
#include <iostream>
#include <iterator>
#include <memory>

struct MidpointInfo;
class Point;

struct MidpointInfo {
  std::shared_ptr<Point> midpoint;
  double distance_to_either_sqrd;
};

class Point {
private:
  std::vector<double> coordinates;
  
public:
  Point(std::vector<double> coordinates);
  Point(double x, double y, double z);

  int num_dimensions();
  double& operator[](int index);

  struct MidpointInfo midpoint(Point other);

  friend std::ostream& operator<<(std::ostream& os, const Point &p);
};

#endif
