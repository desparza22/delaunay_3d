#include "drawer.hpp"


Drawer::Drawer(std::shared_ptr<SDLScreen> canvas):
  canvas(canvas) {
  distance_grid.reserve(canvas->get_width());
  last_update_grid.reserve(canvas->get_width());

  for(int i = 0; i < canvas->get_width(); i++) {
    distance_grid.push_back(std::vector<double>(canvas->get_height(), -1.0));
    last_update_grid.push_back(std::vector<int>(canvas->get_height(), -1));
  }
}

void Drawer::show() {
  canvas->show();
}

void Drawer::attempt_draw_pixel(int x, int y, double distance,
				Uint8 r, Uint8 g, Uint8 b) {
  if(x >= 0 && x < distance_grid.size() &&
     y >= 0 && y < distance_grid[0].size() &&
     (last_update_grid[x][y] != round ||
      distance_grid[x][y] > distance)) {

    canvas->color_point(x, y,
			r, g, b, 255);
    last_update_grid[x][y] = round;
    distance_grid[x][y] = distance;
  }
}

void Drawer::draw_point(Screen camera,
			struct PointRelativity relative_position) {
  Point in_r2 = camera.r3_to_r2(relative_position);
  if(camera.is_in_view(relative_position, in_r2)) {
    double size = point_size_at_dist_one / relative_position.distance * 4;
    if(size < 4) {
      size = 4;
    }
    Uint8 color_intensity =
      150 +
      (Uint8) (105.0 / (relative_position.distance - camera.get_min_dist()));
    
    for(double x_dist = 0; x_dist < size / 2.0; x_dist++) {
      for(double y_dist = 0;
	  x_dist * x_dist + y_dist * y_dist < (size * size) / 4.0;
	  y_dist++) {
	attempt_draw_pixel((int) (in_r2[0] + x_dist),
			   (int) (in_r2[1] + y_dist),
			   relative_position.distance,
			   color_intensity, color_intensity, 0);
	attempt_draw_pixel((int) (in_r2[0] + x_dist),
			   (int) (in_r2[1] - y_dist),
			   relative_position.distance,
			   color_intensity, color_intensity, 0);
	attempt_draw_pixel((int) (in_r2[0] - x_dist),
			   (int) (in_r2[1] + y_dist),
			   relative_position.distance,
			   color_intensity, color_intensity, 0);
	attempt_draw_pixel((int) (in_r2[0] - x_dist),
			   (int) (in_r2[1] - y_dist),
			   relative_position.distance,
			   color_intensity, color_intensity, 0);
      }
    }
  }
}

void Drawer::draw_line(Screen camera,
		       struct PointRelativity position_a,
		       struct PointRelativity position_b) {
  Point a_in_r2 = camera.r3_to_r2(position_a);
  Point b_in_r2 = camera.r3_to_r2(position_b);

  if(camera.is_in_view(position_a, a_in_r2) &&
     camera.is_in_view(position_b, b_in_r2)) {
    
    double size_a = line_size_at_dist_one / position_a.distance;
    if(size_a < 2) size_a = 2;
    double size_b = line_size_at_dist_one / position_b.distance;
    if(size_b < 2) size_b = 2;
    
    Line2D unit_connecting = Line2D(Line2D(a_in_r2, b_in_r2).get_unit());
    Line2D unit_perpendicular = unit_connecting.get_perpendicular();
    
    double x_dist = a_in_r2[0] - b_in_r2[0];
    double y_dist = a_in_r2[1] - b_in_r2[1];
    double distance_2d = sqrt(x_dist * x_dist + y_dist * y_dist);

    int segment_separation = 2;
    int num_segments = distance_2d / segment_separation;
    if(num_segments == 0) {
      return;
    }
    double width_change = (size_b - size_a) / num_segments;

    Point perp_base = a_in_r2;
    double segment_width = size_a;
    double segment_distance = position_a.distance;
    double distance_change =
      (position_b.distance - position_a.distance) /
      num_segments;
    for(int i = 0; i < num_segments; i++) {
      for(int dist_from_base = 0; dist_from_base < segment_width/2;
	  dist_from_base++) {
        attempt_draw_pixel((int) (perp_base[0] +
				  unit_perpendicular[0] * dist_from_base),
			   (int) (perp_base[1] +
				  unit_perpendicular[1] * dist_from_base),
			   segment_distance,
			   255, 255, 0);
	attempt_draw_pixel((int) (perp_base[0] -
				  unit_perpendicular[0] * dist_from_base),
			   (int) (perp_base[1] -
				  unit_perpendicular[1] * dist_from_base),
			   segment_distance,
			   255, 255, 0);
      }

      segment_width += width_change;
      perp_base[0] += unit_connecting[0] * segment_separation;
      perp_base[1] += unit_connecting[1] * segment_separation;
    }
  }
}

void Drawer::clear() {
  canvas->clear(0, 0, 255, 255);
  round++;
}
