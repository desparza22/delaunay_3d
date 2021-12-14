#include "delaunay.hpp"

Delaunay::Delaunay() {
}

std::vector<struct PointPair> Delaunay::gen_delaunay_midpoints(std::vector<Point> points,
							       double proximity_error) {
  std::vector<struct PointPair> lines_found;

  for(int first_idx = 0;
      first_idx < points.size();
      first_idx++) {
    //Find each point that connects to the point at first_idx.
    Point first_point = points[first_idx];
    for(int second_idx = first_idx+1;
	second_idx < points.size();
	second_idx++) {
      Point second_point = points[second_idx];
      
      struct MidpointInfo midpoint_info = first_point.midpoint(second_point);
      std::shared_ptr<Point> midpoint = midpoint_info.midpoint;
      double distance_to_either_sqrd = midpoint_info.distance_to_either_sqrd;
      
      bool points_connect =
	shortest_distance_to_point(*midpoint, points) + proximity_error
	>= distance_to_either_sqrd;
	
      if(points_connect) {
	struct PointPair pair;
	pair.index_a = first_idx;
	pair.index_b = second_idx;
	lines_found.push_back(pair);
      }
    }
  }

  return lines_found;
}

std::vector<struct PointPair> Delaunay::gen_delaunay_rays(std::vector<Point> points,
							  double proximity_error) {
  
  std::vector<struct PointInfo> point_infos;
  point_infos.reserve(points.size());
  for(int i = 0; i < points.size(); i++) {
    Point point = points[i];
    struct PointInfo point_info;
    point_info.p = std::make_shared<Point>(point[0], point[1], point[2]);
    point_info.index = i;
    point_infos.push_back(point_info);
  }

  std::unordered_map<int, bool> lines_found_map;
  std::vector<struct PointPair> lines_found;
  ClosestFinder closest_finder(point_infos);
  
  for(struct PointInfo point_info: point_infos) {
    int this_index = point_info.index;
    std::vector<int> other_indeces = sample_voronoi_walls(point_info,
							  closest_finder,
							  proximity_error);

    for(int other_index: other_indeces) {
      if(!line_found(this_index, other_index, lines_found_map)) {
	struct PointPair connection;
	connection.index_a = this_index;
	connection.index_b = other_index;
	lines_found.push_back(connection);
	mark_line_found(this_index, other_index, lines_found_map);
      }
    }
  }

  return lines_found;
}

std::vector<int> Delaunay::sample_voronoi_walls(struct PointInfo point_info,
						ClosestFinder& closest_finder,
						double proximity_error) {
  std::vector<int> neighboring_regions;

  std::shared_ptr<Point> voronoi_center = point_info.p;
  for(int i = 0; i < NUM_DIRECS; i++) {
    double low = 0.0;
    double high = 100.0;
    struct PointInfo search_for;
    search_for.p = std::make_shared<Point>(0.0, 0.0, 0.0);
    struct PointInfo closest_region;
    int last_closest = -1;
    while(high - low > proximity_error) {
      double mid = (high + low) / 2.0;
      for(int j = 0; j < 3; j++) {
	(*search_for.p)[j] = (*voronoi_center)[j] +
	  voronoi_sample_directions[i][j] * mid;
      }

      closest_region = closest_finder.find_closest(search_for);
      if(closest_region.index != point_info.index) {
	high = mid;
	last_closest = closest_region.index;
      } else {
	low = mid;
      }
    }
    if(last_closest != -1) {
      neighboring_regions.push_back(last_closest);
    }
  }

  return neighboring_regions;
}

double Delaunay::shortest_distance_to_point(Point point,
					   std::vector<Point> points) {
  double shortest_sqrd = -1;
  bool shortest_found = false;
  for(Point other_point: points) {
    double dist_sqrd = 0;
    for(int i = 0; i < 3; i++) {
      double coord_dist = point[i] - other_point[i];
      dist_sqrd += coord_dist * coord_dist;
    }
    if(!shortest_found || (shortest_found && (dist_sqrd < shortest_sqrd))) {
      shortest_sqrd = dist_sqrd;
    }
  }
  return shortest_sqrd;
}


bool Delaunay::line_found(int first_idx, int second_idx,
			  std::unordered_map<int, bool>& lines_found) {
  return lines_found.contains(make_key(first_idx, second_idx));
}

void Delaunay::mark_line_found(int first_idx, int second_idx,
			       std::unordered_map<int, bool>& lines_found) {
  lines_found[make_key(first_idx, second_idx)] = true;
}

int Delaunay::make_key(int first_idx, int second_idx) {
  if(first_idx > second_idx) {
    return make_key(second_idx, first_idx);
  }

  return (first_idx << 14) + second_idx;
}


