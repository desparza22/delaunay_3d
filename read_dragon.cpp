#include "read_dragon.hpp"

std::vector<Point> read_dragon() {
  std::string dragon_filename("dragon_stand/face_left.txt");
  std::ifstream file_object(dragon_filename);
  std::string line;
  std::vector<Point> points;
  if(file_object.is_open()) {
    int lines_read = 0;
    while(getline(file_object, line)) {
      if(lines_read % 10 == 0) {
	std::string::size_type sz;
	double x = std::stod(line, &sz);
	line = line.substr(sz);
	double y = std::stod(line, &sz);
	line = line.substr(sz);
	double z = std::stod(line);
	points.push_back(Point(x*10000, y*10000, z*10000));
      }
      lines_read++;
    }
  }
  file_object.close();
  return points;
}

