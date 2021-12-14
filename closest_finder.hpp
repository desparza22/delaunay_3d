#ifndef CLOSEST_FINDER_HPP
#define CLOSEST_FINDER_HPP

#include <memory>
#include <vector>
#include <limits>

#include "point.hpp"

struct PointInfo;

struct PointInfo {
  std::shared_ptr<Point> p;
  int index;
};

class ClosestFinder;

class ClosestFinder {
private:
  int axis;
  struct PointInfo split;
  bool has_children;
  std::shared_ptr<ClosestFinder> low;
  std::shared_ptr<ClosestFinder> high;
  std::vector<std::vector<double>> boundaries;

  void find_closer(struct PointInfo target, struct PointInfo& best);
  double dist_to_region_sqrd(struct PointInfo target);
  double dist_sqrd(struct PointInfo a, struct PointInfo b);
  std::vector<std::vector<double>> initial_boundaries();
  std::vector<std::vector<double>> deep_copy_boundaries();
  
public:
  ClosestFinder(std::vector<struct PointInfo> points);
  ClosestFinder(std::vector<struct PointInfo> points,
		int axis,
	        std::vector<std::vector<double>> boundaries);
  
  struct PointInfo find_closest(struct PointInfo target);
};


#endif
