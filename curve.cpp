

Line::Line(std::vector<float> polynomial) {
  this->polynomial = polynomial;
}

float Line::get_y(float x) {
  float res = 0f;

  double running_x_to_the_n = 1f;
  for(float cofactor: polynomial) {
    res += cofactor * running_x_to_the_n;
    running_x_to_the_n *= x;
  }

  return res;
}
