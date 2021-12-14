#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <vector>
#include <memory>

#include "screen.hpp"
#include "sdl_screen.hpp"
#include "point.hpp"
#include "line2d.hpp"

class Drawer;

class Drawer {
private:
  std::shared_ptr<SDLScreen> canvas;
  std::vector<std::vector<double>> distance_grid;
  std::vector<std::vector<int>> last_update_grid;
  int round = 0;

  double point_size_at_dist_one = 700.0;
  double line_size_at_dist_one = 200.0;
  
  void attempt_draw_pixel(int x, int y, double distance,
			  Uint8 r, Uint8 g, Uint8 b);

public:
  Drawer(std::shared_ptr<SDLScreen> canvas);

  void show();

  void draw_point(Screen camera, struct PointRelativity relative_position);
  void draw_line(Screen camera,
		 struct PointRelativity position_a,
		 struct PointRelativity position_b);

  void clear();
};


#endif
