#include <iostream>

int main(int argc, char* argv[]) {
  srand(128132);
  for(int i = 0; i < 15; i++) {
    std::cout << rand() << std::endl;
  }

  std::cout << (-502 % 5) << std::endl;
  std::cout << (502 % -5) << std::endl;
  std::cout << (-502 % -5) << std::endl;
}
