#include "point.hpp"
#include "line.hpp"
#include "line2d.hpp"
#include "line3d.hpp"
#include "plane3d.hpp"
#include "screen.hpp"
#include "sdl_screen.hpp"
#include "drawer.hpp"
#include "random_points.hpp"
#include "delaunay.hpp"
#include "closest_finder.hpp"
#include "read_dragon.hpp"

#include <Eigen/Core>
#include <cmath>
#include <numbers>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "{";
    int last = v.size() - 1;
    for(int i = 0; i < v.size(); i++) {
        os << v[i];
        if (i != last) {
            os << ", ";
	}
    }
    os << "}";
    return os;
}


void plot_circles(SDLScreen sdl_screen, double dimensions_f,
		  Screen camera) {
  std::vector<double> center_coordinates {
    dimensions_f/2.0,
    dimensions_f/2.0,
    dimensions_f
  };

  Point center(center_coordinates);
  Plane3D circle_surface(center,
			 Line3D(center, Eigen::Array3d(100.0, 1.0, -10.0)),
			 Line3D(center, Eigen::Array3d(1.0, 100.0, 20.0)),
			 false);


  
  double increment = std::numbers::pi / 100.0;
  double prev_x = 0;
  double prev_y = 0;
  for(double theta = 0.0;
      prev_x <= dimensions_f && prev_x >= 0 &&
	prev_y <= dimensions_f && prev_y >= 0 &&
	center_coordinates[2] >= 0 /*&&
	theta < 2 * std::numbers::pi*/;
      theta+=increment) {
    double x = std::cosf(theta) * 100;
    double y = std::sinf(theta) * 100;
    
    Point point_on_plane = circle_surface.translate_coordinates(x, y);
    Point in_r2 = camera.r3_to_r2(point_on_plane);

    double projected_x = in_r2[0];
    double projected_y = in_r2[1];

    sdl_screen.color_point((int) projected_x,
			   (int) projected_y,
			   250, 250, 0, 255);
    
    sdl_screen.show();
    center_coordinates[0] -= .024;
    center_coordinates[1] -= .048;
    center_coordinates[2] -= .24;
    circle_surface.change_center(Point(center_coordinates));

    prev_x = projected_x;
    prev_y = projected_y;
  }
}

void plot_boundaries(SDLScreen sdl_screen, double dimensions_f,
		     Screen camera) {
  Point close_tl(0.0, 0.0, 0.0);
  Point close_tr(dimensions_f, 0.0, 0.0);
  Point close_bl(0.0, dimensions_f, 0.0);
  Point close_br(dimensions_f, dimensions_f, 0.0);
  Point far_tl(0.0, 0.0, dimensions_f);
  Point far_tr(dimensions_f, 0.0, dimensions_f);
  Point far_bl(0.0, dimensions_f, dimensions_f);
  Point far_br(dimensions_f, dimensions_f, dimensions_f);

  Line3D tl(close_tl, far_tl);
  Line3D tr(close_tr, far_tr);
  Line3D bl(close_bl, far_bl);
  Line3D br(close_br, far_br);
  Line3D top(far_tl, far_tr);
  Line3D left(far_tl, far_bl);
  Line3D bottom(far_bl, far_br);
  Line3D right(far_tr, far_br);
  
  std::vector<Line3D> lines {
    tl, tr, bl, br,
    top, left, bottom, right};
  for(int i = 0; i < lines.size(); i++) {
    lines[i] = lines[i].get_unit();
  }


  std::vector<Point> outline;
  for(Line3D line: lines) {
    Point starting = line.get_reference_point();
    outline.push_back(starting);

    double distance = 5.0;
    while(distance < dimensions_f) {
      Point on_line(starting[0] + line[0] * distance,
		    starting[1] + line[1] * distance,
		    starting[2] + line[2] * distance);
      outline.push_back(on_line);
      distance += 5;
    }
  }

  for(Point on_outline: outline) {
    Point on_screen = camera.r3_to_r2(on_outline);
    sdl_screen.color_point((int) on_screen[0],
			   (int) on_screen[1],
			   255, 255, 0, 255);
  }

  sdl_screen.show();
}

void plot_rect(Drawer drawer, Screen camera) {
  double edge_length = 300;
  int length_scale = 5;
  Point close_tl(150, 150, 1800);
  Point close_tr(close_tl[0] + edge_length, close_tl[1], close_tl[2]);
  Point close_br(close_tr[0], close_tr[1] + edge_length, close_tr[2]);
  Point close_bl(close_br[0] - edge_length, close_br[1], close_br[2]);
  Point far_tl(close_tl[0], close_tl[1],
	       close_tl[2] + length_scale * edge_length);
  Point far_tr(close_tr[0], close_tr[1],
	       close_tr[2] + length_scale * edge_length);
  Point far_br(close_br[0], close_br[1],
	       close_br[2] + length_scale * edge_length);
  Point far_bl(close_bl[0], close_bl[1],
	       close_bl[2] + length_scale * edge_length);
  std::vector<Point> points {
    close_tl, close_tr, close_br, close_bl,
    far_tl, far_tr, far_br, far_bl
  };
  
  Point camera_center = camera.get_center();

  int incr = 2;
  int start = 0;
  int end = start + 600 * incr;
  for(int i = 0; i < 10; i++) {
    int temp = start;
    start = end;
    end = temp;
    incr *= -1;
    for(int dist = start; dist != end; dist+=incr) {
      camera_center[2] = dist;
      camera.change_center(camera_center);
      std::vector<struct PointRelativity> relative_positions;
      for(Point point: points) {
	relative_positions.push_back(camera.get_position_relative(point));
      }
            
      drawer.clear();

      //Plot points
      for(PointRelativity relative_position: relative_positions) {
	drawer.draw_point(camera, relative_position);
      }

      //Outline squares
      for(int j = 0; j < 3; j++) {
	drawer.draw_line(camera,
			 relative_positions[j], relative_positions[j+1]);
	drawer.draw_line(camera,
			 relative_positions[j+4], relative_positions[j+5]);
      }
      drawer.draw_line(camera, relative_positions[0], relative_positions[3]);
      drawer.draw_line(camera, relative_positions[4], relative_positions[7]);

      //Connect squares
      for(int j = 0; j < 4; j++) {
	drawer.draw_line(camera,
			 relative_positions[j], relative_positions[j+4]);
      }
      drawer.show();
    }
  }  
}

void plot_random(Drawer drawer, Screen camera) {
  RandomPoints rp;
  Point top_left(400, 150, 1800);
  double width = 300;
  int num_points = 100;
  std::vector<Point> random_points =
    rp.gen_random_points(top_left, width, num_points);

  drawer.clear();
  drawer.show();

  int increments = 800;
  double ratio = 0.2 / increments;
  for(int j = 0; j < 6; j++) {
    for(int i = 0; i < increments; i++) {
      for(Point point: random_points) {
	struct PointRelativity relative_position =
	  camera.get_position_relative(point);
	drawer.draw_point(camera, relative_position);
      }
      drawer.show();
      drawer.clear();
      camera.rotate_horizontal(ratio);
    }
    ratio *= -1;
  }
}

void navigate_random(Drawer drawer, Screen camera) {
  RandomPoints rp;
  Point top_left(-4, -4, 100);
  double width = 700;
  int num_points = 100;

  std::vector<Point> random_points =
    rp.gen_random_points(top_left, width, num_points);
  
  Delaunay delaunay;
  std::vector<struct PointPair> connecting_points =
    delaunay.gen_delaunay_rays(random_points, .001);
  
  drawer.clear();
  std::vector<struct PointRelativity> relative_positions;
  relative_positions.reserve(random_points.size());
  for(Point point: random_points) {
    relative_positions.push_back(camera.get_position_relative(point));
  }
  
  for(int i = 0; i < random_points.size(); i++) {
    struct PointRelativity relative_position = relative_positions[i];
    drawer.draw_point(camera, relative_position);
  }
  for(struct PointPair pair: connecting_points) {
    struct PointRelativity position_a = relative_positions[pair.index_a];
    struct PointRelativity position_b = relative_positions[pair.index_b];
    drawer.draw_line(camera, position_a, position_b);
  }
  drawer.show();

  bool keep_going = true;
  SDL_Event navigation_event;
  while(keep_going && SDL_WaitEvent(&navigation_event)) {
    if(navigation_event.type == SDL_KEYDOWN) {
      bool recompute = false;
      switch(navigation_event.key.keysym.sym) {
      case SDLK_UP:
	camera.shift_forward(20);
	recompute = true;
	break;
      case SDLK_DOWN:
	camera.shift_forward(-20);
	recompute = true;
	break;
      case SDLK_w:
	camera.rotate_vertical(-.012);
	recompute = true;
	break;
      case SDLK_s:
	camera.rotate_vertical(.012);
	recompute = true;
	break;
      case SDLK_a:
	camera.rotate_horizontal(-.012);
	recompute = true;
	break;
      case SDLK_d:
	camera.rotate_horizontal(.012);
	recompute = true;
	break;
      case SDLK_q:
	keep_going = false;
	break;
      }
      
      if(recompute) {
	drawer.clear();
        std::vector<struct PointRelativity> relative_positions;
	relative_positions.reserve(random_points.size());
	for(Point point: random_points) {
	  relative_positions.push_back(camera.get_position_relative(point));
	}
	
	for(int i = 0; i < random_points.size(); i++) {
	  struct PointRelativity relative_position = relative_positions[i];
	  drawer.draw_point(camera, relative_position);
	}
	for(struct PointPair pair: connecting_points) {
	  struct PointRelativity position_a = relative_positions[pair.index_a];
	  struct PointRelativity position_b = relative_positions[pair.index_b];
	  drawer.draw_line(camera, position_a, position_b);
	}
	drawer.show();
      }
    }
  }
}

void navigate_dragon(Drawer drawer, Screen camera) {
  std::vector<Point> dragon_points =
    read_dragon();
  
  std::string dragon_lines_output_name = "dragon_stand/dragon_delaunay.txt";
  /*UNCOMMENT TO GENERATE THE DELAUNAY TRIANGULATION FROM SCRATCH

  Delaunay delaunay;
  std::vector<struct PointPair> connecting_points =
  delaunay.gen_delaunay_rays(dragon_points, .001);

  std::ofstream dragon_lines_output(dragon_lines_output_name);
  if(dragon_lines_output.is_open()) {
    for(struct PointPair point_pair: connecting_points) {
      dragon_lines_output <<
	std::to_string(point_pair.index_a) <<
	" " <<
	std::to_string(point_pair.index_b) <<
	std::endl;
    }
  }
  dragon_lines_output.close();*/

  /*IF GENERATING FROM SCRATCH, COMMENT OUT FROM HERE TO NEXT COMMENT*/
  std::vector<struct PointPair> connecting_points;
  std::ifstream dragon_lines_in(dragon_lines_output_name);
  std::string line;
  if(dragon_lines_in.is_open()) {
    while(getline(dragon_lines_in, line)) {
      std::string::size_type sz;
      int index_a = std::stoi(line, &sz);
      int index_b = std::stoi(line.substr(sz));
      struct PointPair point_pair;
      point_pair.index_a = index_a;
      point_pair.index_b = index_b;
      connecting_points.push_back(point_pair);
    }
  } else {
    std::cerr << "trouble opening " << dragon_lines_output_name << std::endl;
  }
  
  drawer.clear();
  std::vector<struct PointRelativity> relative_positions;
  relative_positions.reserve(dragon_points.size());
  for(Point point: dragon_points) {
    relative_positions.push_back(camera.get_position_relative(point));
  }
  
  for(int i = 0; i < dragon_points.size(); i++) {
    struct PointRelativity relative_position = relative_positions[i];
    drawer.draw_point(camera, relative_position);
  }
  for(struct PointPair pair: connecting_points) {
    struct PointRelativity position_a = relative_positions[pair.index_a];
    struct PointRelativity position_b = relative_positions[pair.index_b];
    drawer.draw_line(camera, position_a, position_b);
  }
  drawer.show();

  bool keep_going = true;
  SDL_Event navigation_event;
  Plane3D camera_base = camera.get_plane();
  Point camera_center = camera.get_center();
  while(keep_going && SDL_WaitEvent(&navigation_event)) {
    if(navigation_event.type == SDL_KEYDOWN) {
      bool recompute = false;
      switch(navigation_event.key.keysym.sym) {
      case SDLK_UP:
	camera.shift_forward(80);
	recompute = true;
	break;
      case SDLK_DOWN:
	camera.shift_forward(-80);
	recompute = true;
	break;
      case SDLK_w:
	camera.rotate_vertical(-.012);
	recompute = true;
	break;
      case SDLK_s:
	camera.rotate_vertical(.012);
	recompute = true;
	break;
      case SDLK_a:
	camera.rotate_horizontal(-.012);
	recompute = true;
	break;
      case SDLK_d:
	camera.rotate_horizontal(.012);
	recompute = true;
	break;
      case SDLK_q:
	keep_going = false;
	break;
      case SDLK_p:
	camera_base = camera.get_plane();
	camera_center = camera.get_center();
	std::cout << camera_base.get_horizontal() << " " << camera_base.get_vertical() << " " << camera_base.get_directional() << std::endl;
	std::cout << camera_center[0] << " " << camera_center[1] << " " << camera_center[2] << std::endl;
      }
      
      if(recompute) {
	drawer.clear();
        std::vector<struct PointRelativity> relative_positions;
	relative_positions.reserve(dragon_points.size());
	for(Point point: dragon_points) {
	  relative_positions.push_back(camera.get_position_relative(point));
	}
	
	for(int i = 0; i < dragon_points.size(); i++) {
	  struct PointRelativity relative_position = relative_positions[i];
	  drawer.draw_point(camera, relative_position);
	}
	for(struct PointPair pair: connecting_points) {
	  struct PointRelativity position_a = relative_positions[pair.index_a];
	  struct PointRelativity position_b = relative_positions[pair.index_b];
	  drawer.draw_line(camera, position_a, position_b);
	}
	drawer.show();
      }
    }
  }
}

int main(int argc, char* argv[]) {
  int dimensions = 700;
  double dimensions_f = (double) dimensions;

  std::shared_ptr<SDLScreen> sdl_screen(new SDLScreen(dimensions, dimensions));

  Point camera_center(std::vector<double> {896.14,
					  -928.365,
					  12796.4});
  /*  Point camera_center(std::vector<double> {
      dimensions_f/2.0,
      dimensions_f/2.0,
      5000.0});*/
	
  Plane3D camera_surface(camera_center,
			 Line3D(camera_center,
				Eigen::Array3d(-0.997347,
					       -0.00940342,
					       0.0721845)),
			 Line3D(camera_center,
				Eigen::Array3d(-0.00294712,
					       -0.98559,
					       -0.169125)),
			 true);


  double expansion_rate = .1;
  double width = dimensions_f;
  double height = dimensions_f;
  double min_dist = 50;
  double max_dist = 70000;
  Screen camera(camera_surface,
		width, height, min_dist, max_dist,
		expansion_rate);

  Drawer drawer(sdl_screen);

  //  plot_circles(sdl_screen, dimensions_f, camera);
  //plot_boundaries(sdl_screen, dimensions_f, camera);
  //plot_rect(drawer, camera);
  //plot_random(drawer, camera);
  //  SDL_Delay(10000);
  navigate_dragon(drawer, camera);
}
