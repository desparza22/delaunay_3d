GCC=g++ -std=c++20 -g
EIGEN=-I/Users/diegoesparza/clibs/eigen-3.4.0
SDL_INCLUDE=-I/Users/diegoesparza/clibs/SDL/SDL_include
SDL_LINK=-L ~/clibs/SDL/SDL_lib -l SDL2-2.0.0
ALL_OBJECT=random_points.o sdl_screen.o screen.o plane3d.o line.o line2d.o line3d.o point.o drawer.o delaunay.o closest_finder.o read_dragon.o
ALL_HEADER=random_points.hpp sdl_screen.hpp screen.hpp plane3d.hpp line.hpp line2d.hpp line3d.hpp point.hpp drawer.hpp delaunay.hpp closest_finder.hpp read_dragon.hpp
RANDOM_POINTS_TEST=-D TEST_RANDOM_POINTS=1

.PHONY=all
all: main

main: main.cpp $(ALL_HEADER) $(ALL_OBJECT)
	$(GCC) -o main main.cpp $(ALL_OBJECT) $(EIGEN) $(SDL_LINK)

random_points: random_points.cpp random_points.hpp point.o point.hpp
	$(GCC) -o random_points random_points.cpp point.o $(RANDOM_POINTS_TEST) $(EIGEN)

random_points.o: random_points.hpp random_points.cpp point.o
	$(GCC) -c random_points.cpp $(EIGEN)

read_dragon.o: read_dragon.hpp read_dragon.cpp point.o
	$(GCC) -c read_dragon.cpp

delaunay.o: delaunay.hpp delaunay.cpp closest_finder.o point.o
	$(GCC) -c delaunay.cpp

sdl_screen.o: sdl_screen.hpp sdl_screen.cpp
	$(GCC) -c sdl_screen.cpp $(SDL_LIB)

drawer.o: drawer.hpp drawer.cpp line2d.o point.o sdl_screen.o screen.o
	$(GCC) -c drawer.cpp $(EIGEN)

screen.o: screen.hpp screen.cpp plane3d.o point.o
	$(GCC) -c screen.cpp $(EIGEN)

plane3d.o: plane3d.hpp plane3d.cpp line3d.o point.o
	$(GCC) -c plane3d.cpp $(EIGEN)

line.o: line.hpp line.cpp point.o
	$(GCC) -c line.cpp $(EIGEN)

line2d.o: line2d.hpp line2d.cpp line.o
	$(GCC) -c line2d.cpp $(EIGEN)

line3d.o: line3d.hpp line3d.cpp line.o
	$(GCC) -c line3d.cpp $(EIGEN)

point.o: point.hpp point.cpp
	$(GCC) -c point.cpp $(EIGEN)

closest_finder.o: closest_finder.hpp closest_finder.cpp point.o
	$(GCC) -c closest_finder.cpp

clean:
	rm -f main
	rm -f random_points.o
	rm -f sdl_screen.o
	rm -f screen.o
	rm -f plane3d.o
	rm -f line.o
	rm -f line3d.o
	rm -f point.o
	rm -f delaunay.o
	rm -f drawer.o
	rm -f closest_finder.o
	rm -f read_dragon.o
