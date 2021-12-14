#ifndef LINE_HPP
#define LINE_HPP



class Line {
private:
  std::vector<float> polynomial;

public:
  Line(std::vector<float> polynomial);

  float get_y(float x);
};


#endif
