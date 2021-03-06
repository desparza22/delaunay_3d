#ifndef SDL_SCREEN_HPP
#define SDL_SCREEN_HPP


#include <SDL2/SDL.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#define SDL_WINDOW_NO_FLAGS 0

class SDLScreen;

class SDLScreen {
private:
  SDL_Renderer* renderer_a;
  SDL_Window* window_a;
  SDL_Event e;
  
  int width;
  int height;

  void init(Uint32 flags);

  SDL_Window* create_window(const std::string title,
			    int w, int h,
			    Uint32 flags);

  SDL_Renderer* create_renderer(SDL_Window* window);
  
public:
  bool debugging = false;
  
  SDLScreen(Uint32 width, Uint32 height);
  void show();
  void clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  void color_point(int x, int y,
		   Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  int get_width();
  int get_height();
  friend std::ostream& operator<<(std::ostream& os, const SDLScreen &screen);
};

#endif
