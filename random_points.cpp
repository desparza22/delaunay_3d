#include "random_points.hpp"

RandomPoints::RandomPoints() {
}

int RandomPoints::seeded_random(int seed) {
  srand(1234567 + seed);
  rand();
  rand();
  return rand();
}

int RandomPoints::gen_seed(double x, double y, double z) {
  double combined = x + 5 * y + 7 * z;
  return double_to_int_bit(combined);
}

int RandomPoints::double_to_int_bit(double d) {
  double* d_ptr = &d;
  long* l_ptr = (long*) d_ptr;
  long l = *l_ptr;
  int i = (int) (l ^ (l >> 32));
  return i;
}

Point RandomPoints::point_in_cube_from_random(Point cube_corner,
					      double cube_width,
					      int random) {
  if(random < 0) {
    random *= -1;
  }
  int x_part = random % 1000;
  int y_part = (random/1000) % 1000;
  int z_part = (random/1000000) % 1000;
  double x_percent = ((double) x_part) / 1000.0;
  double y_percent = ((double) y_part) / 1000.0;
  double z_percent = ((double) z_part) / 1000.0;
  return Point(cube_corner[0] + cube_width * x_percent,
	       cube_corner[1] + cube_width * y_percent,
	       cube_corner[2] + cube_width * z_percent);
}

std::vector<Point> RandomPoints::gen_random_points(Point cube_corner,
						   double cube_width,
						   int num_points) {
  std::vector<Point> random_points;
  random_points.reserve(num_points);
  Point last = cube_corner;
  for(int i = 0; i < num_points; i++) {
    int seed = gen_seed(last[0], last[1], last[2]);
    int random = seeded_random(seed);
    Point next = point_in_cube_from_random(cube_corner,
					   cube_width,
					   random);
    random_points.push_back(next);
    last = next;
  }
  return random_points;
}

#ifdef TEST_RANDOM_POINTS

int main(int argc, char* argv[]) {
  RandomPoints rp;
  std::vector<Point> points_in_unit =
    rp.gen_random_points(Point(0.0, 0.0, 0.0), 1.0, 20);
  for(Point point: points_in_unit) {
    std::cout << point << std::endl;
  }
  return 0;
}

#endif
