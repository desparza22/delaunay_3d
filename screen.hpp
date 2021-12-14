#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <Eigen/Core>
#include <Eigen/Cholesky>
#include <memory>
#include <iostream>

#include "point.hpp"
#include "plane3d.hpp"

struct PointRelativity;

struct PointRelativity {
  double distance;
  double vertical_from_center;
  double horizontal_from_center;

  friend std::ostream& operator<<(std::ostream& os,
				  const PointRelativity &relative_position);
};



class Screen;

class Screen {
private:
  double height;
  double width;
  double min_dist;
  double max_dist;
  
  Plane3D plane;
  double expansion_rate;

  Eigen::LDLT<Eigen::Matrix3f> solver;
  bool solver_constructed = false;
  Eigen::LDLT<Eigen::Matrix3f> get_solver();
  
public:
  Screen(Plane3D plane,
	 double width, double height,
	 double min_dist, double max_dist,
	 double expansion_rate);

  double get_min_dist();
  double get_max_dist();

  Point r3_to_r2(Point point_in_r3);
  bool is_in_view(Point point_in_r3);

  //Returns negative  value if point is not in view
  double distance(Point point_in_r3);


  struct PointRelativity get_position_relative(Point p);
  Point r3_to_r2(struct PointRelativity position_relative);
  bool is_in_view(struct PointRelativity position_relative);
  bool is_in_view(struct PointRelativity position_relative,
		  Point point_in_r2);
  
  double distance(struct PointRelativity position_relative);

  double get_width();
  double get_height();
  Point get_center();
  void change_center(Point new_center);

  void shift_forward(double distance);
  void shift_right(double distance);
  void rotate_horizontal(double ratio);
  void rotate_vertical(double ratio);

  Plane3D get_plane();
};

#endif
