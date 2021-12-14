#ifndef BOUNDED_REGION_HPP
#define BOUNDED_REGION_HPP

class BoundedRegion;

class BoundedRegion {
private:

public:
  virtual bool is_point_in_region(float x, float y);

  void draw_region(Screen screen);
};


#endif

#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen;

class Screen {
public:
  Screen(int width, int height);

  void color_point(int x, int y, Color color);
  

};

#endif


#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle;

class Triangle {
public:
  Triangle(float coordinates[][]);
  
  bool is_point_in_bounds(float x, float y) override;


};

#endif

#ifndef QUADRA_HPP
#define QUADRA_CPP

class Quadra;

class Quadra {
public:
  bool is_point_in_bounds(float coordinates[][]);


};


#endif


#ifndef SHAPE_DISCRETE_HPP
#define SHAPE_DISCRETE_HPP

class ShapeDiscrete;

class ShapeDiscrete {
public:
  ShapeDiscrete(float coordinates[][]);


};


#endif


#ifndef CIRCLE
#define CIRCLE

class Circle;

class Circle {
public:
  Circle(float radius_x, float radius_y, float radius);

  bool is_point_in_bounds(float x, float y);


};

#endif

#ifndef UNION_REGION
#define UNION_REGION

class UnionRegion;

class UnionRegion {
public:
  UnionRegion(Vector<BoundedRegion> regions);


};

#endif

#ifndef INTERSECT_REGION_HPP
#define INTERSECT_REGION_HPP

class IntersectRegion;

class IntersectRegion {
public:
  IntersectRegion(Vector<BoundedRegion> regions);

};

#endif


