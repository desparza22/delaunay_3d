#ifndef SIDED_LINE_HPP
#define SIDED_LINE_HPP

enum side{BELOW, ABOVE};

class SidedLine;

class SidedLine {
private:
  Line line;
  side line_side;

public:
  SidedLine(Line line, side line_side);
  bool is_point_onside(float x_coor, float y_coor);
};

#endif
