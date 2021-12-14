#include "sdl_screen.hpp"

SDLScreen::SDLScreen(Uint32 width, Uint32 height):
  width((int)width), height((int)height) {
  init(SDL_INIT_VIDEO);
  std::string title("Window Title");
  Uint32 flags = SDL_WINDOW_NO_FLAGS;
  window_a = create_window(title,
			 width, height,
			 flags);
  renderer_a = create_renderer(window_a);
}

void SDLScreen::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(renderer_a, r, g, b, a);
  SDL_RenderClear(renderer_a);
}

void SDLScreen::show() {
  //https://stackoverflow.com/a/41044089/13809199
  SDL_PollEvent(&e);
  SDL_RenderPresent(renderer_a);
}

void SDLScreen::color_point(int x, int y,
			    Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(renderer_a, r, g, b, a);
  SDL_RenderDrawPoint(renderer_a, x, y);
}

void SDLScreen::init(Uint32 flags) {
  int ret = SDL_Init(flags);
  if(ret != 0) {
    const char* error = SDL_GetError();
    fprintf(stderr, "Error in %s: %s\n", __FUNCTION__, error);
    exit(1);
  }
}


SDL_Window* SDLScreen::create_window(const std::string title,
				     int w, int h,
				     Uint32 flags) {
  SDL_Window* created = SDL_CreateWindow(title.c_str(),
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED,
					 w, h, flags);
  if(created == NULL) {
    const char* error = SDL_GetError();
    fprintf(stderr, "Error in %s, %s\n", __FUNCTION__, error);
    exit(1);
  }
  return created;
}

SDL_Renderer* SDLScreen::create_renderer(SDL_Window* window) {
  int device = -1;
  SDL_Renderer* renderer = SDL_CreateRenderer(window, device,
					      SDL_RENDERER_SOFTWARE);
  if(renderer == NULL) {
    const char* error = SDL_GetError();
    fprintf(stderr, "Error in %s: %s\n", __FUNCTION__, error);
    exit(1);
  }
  return renderer;
}

int SDLScreen::get_width() {
  return width;
}

int SDLScreen::get_height() {
  return height;
}

std::ostream& operator<<(std::ostream &os, const SDLScreen &screen) {
  return os << "[SDLScreen: renderer_a ptr: " << screen.renderer_a
	    << ", window_a ptr: " << screen.window_a << ", width: "
	    << screen.width << ", height: " << screen.height
	    << std::endl;
}
