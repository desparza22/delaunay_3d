#include "closest_finder.hpp"


ClosestFinder::ClosestFinder(std::vector<struct PointInfo> points):
  ClosestFinder(points, 0, initial_boundaries()) {
}


ClosestFinder::ClosestFinder(std::vector<struct PointInfo> points,
			     int axis,
			     std::vector<std::vector<double>> boundaries):
  axis(axis),
  boundaries(boundaries) {
  int rand_val = rand();
  int split_index = rand_val % points.size();
  split = points[split_index];

  if(points.size() > 1) {
    double split_val = (*split.p)[axis];
    std::vector<struct PointInfo> low_points;
    std::vector<struct PointInfo> high_points;
    for(int i = 0; i < points.size(); i++) {
      if(i != split_index) {
	struct PointInfo point_info = points[i];
	if((*point_info.p)[axis] < split_val) {
	  low_points.push_back(point_info);
	} else {
	  high_points.push_back(point_info);
	}
      }
    }
    if(low_points.size() == 0) {
      low_points.push_back(split);
    }
    if(high_points.size() == 0) {
      high_points.push_back(split);
    }
    
    std::vector<std::vector<double>> low_boundaries = deep_copy_boundaries();
    std::vector<std::vector<double>> high_boundaries = deep_copy_boundaries();
    low_boundaries[axis][1] = split_val;
    high_boundaries[axis][0] = split_val;
    low = std::make_shared<ClosestFinder>(low_points, (axis + 1) % 3,
					  low_boundaries);
    high = std::make_shared<ClosestFinder>(high_points, (axis + 1) % 3,
					   high_boundaries);
    has_children = true;
  } else {
    has_children = false;
  }
}

struct PointInfo ClosestFinder::find_closest(struct PointInfo target) {
  struct PointInfo best = split;
  if(has_children) {
    if((*target.p)[axis] < (*split.p)[axis]) {
      low->find_closer(target, best);
      high->find_closer(target, best);
      
    } else {
      high->find_closer(target, best);
      low->find_closer(target, best);
    }
  }

  return best;
}

void ClosestFinder::find_closer(struct PointInfo target, struct PointInfo& best) {
  double cur_best = dist_sqrd(target, best);
  if(dist_to_region_sqrd(target) < cur_best) {
    double compare = dist_sqrd(target, split);
    if(compare < cur_best) {
      cur_best = compare;
      best = split;
    }

    if(has_children) {
      if((*target.p)[axis] < (*split.p)[axis]) {
	low->find_closer(target, best);
	high->find_closer(target, best);
      } else {
	high->find_closer(target, best);
	low->find_closer(target, best);
      }
    }
  }
}

double ClosestFinder::dist_to_region_sqrd(struct PointInfo target) {
  double dist_sqrd = 0;
  for(int i = 0; i < 3; i++) {
    double coord_dist = 0;
    if((*target.p)[i] < boundaries[i][0]) {
      coord_dist = boundaries[i][0] - (*target.p)[i];
    } else if((*target.p)[i] > boundaries[i][1]) {
      coord_dist = (*target.p)[i] - boundaries[i][1];
    }
    dist_sqrd = coord_dist * coord_dist;
  }
  return dist_sqrd;
}

double ClosestFinder::dist_sqrd(struct PointInfo a, struct PointInfo b) {
  double dist_sqrd = 0;
  for(int i = 0; i < 3; i++) {
    double coord_dist = (*a.p)[i] - (*b.p)[i];
    dist_sqrd += coord_dist * coord_dist;
  }
  return dist_sqrd;
}


std::vector<std::vector<double>> ClosestFinder::initial_boundaries() {
  std::vector<std::vector<double>> boundaries;
  for(int i = 0; i < 3; i++) {
    boundaries.push_back(std::vector<double> {
	std::numeric_limits<double>::min(),
	std::numeric_limits<double>::max()});
  }
  return boundaries;
}

std::vector<std::vector<double>> ClosestFinder::deep_copy_boundaries() {
  std::vector<std::vector<double>> deep_copy;
  for(int i = 0; i < 3; i++) {
    deep_copy.push_back(std::vector<double> {
	boundaries[i][0], boundaries[i][1]});
  }
  return deep_copy;
}
