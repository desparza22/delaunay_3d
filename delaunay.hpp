#ifndef DELAUNAY_HPP
#define DELAUNAY_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "point.hpp"
#include "closest_finder.hpp"

#define NUM_DIRECS 14

struct PointPair;

struct PointPair {
  int index_a;
  int index_b;
};

class Delaunay;

class Delaunay {
private:
  bool line_found(int first_idx, int second_idx,
		  std::unordered_map<int, bool>& lines_found);

  void mark_line_found(int first_idx, int second_idx,
		       std::unordered_map<int, bool>& lines_found);

  int make_key(int first_idx, int second_idx);

  std::vector<int> sample_voronoi_walls(struct PointInfo point_info,
					ClosestFinder& closest_finder,
					double proximity_error);

  double voronoi_sample_directions[NUM_DIRECS][3] = {
    {-1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, -1.0},
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0},
    {1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0},
    {-1.0, 1.0, 1.0},
    {-1.0, 1.0, -1.0},
    {-1.0, -1.0, 1.0},
    {-1.0, -1.0, -1.0}			    
  };
  
public:
  Delaunay();

  std::vector<struct PointPair> gen_delaunay_midpoints(std::vector<Point> points,
						       double proximity_error);

  std::vector<struct PointPair> gen_delaunay_rays(std::vector<Point> point,
						  double proximity_error);
  double shortest_distance_to_point(Point point, std::vector<Point> points);
};




#endif
