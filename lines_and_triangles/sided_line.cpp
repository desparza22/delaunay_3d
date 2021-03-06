
SidedLine::SidedLine(Line line, side line_side) {
  this->line = line;
  this->line_side = line_side;
}


bool SidedLine::is_point_onside(float x_coor, float y_coor) {
  y_coor_on_line = line.get_y(x_coor);
  return (line_side == BELOW && y_coor <= y_coor_on_line) ||
    (line_side == ABOVE && y_coor <= y_coor_on_line);
}
