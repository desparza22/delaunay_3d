#ifndef RANDOM_POINTS_HPP
#define RANDOM_POINTS_HPP

#include <cassert>
#include <vector>
#include <iostream>

#include "point.hpp"

class RandomPoints;

class RandomPoints {
private:
  int double_to_int_bit(double d);
  int gen_seed(double x, double y, double z);
  int seeded_random(int seed);
  Point point_in_cube_from_random(Point cube_corner,
				  double cube_width,
				  int random);

  
public:
  RandomPoints();

  std::vector<Point> gen_random_points(Point cube_corner, double cube_width,
				       int num_points);
};
#endif
