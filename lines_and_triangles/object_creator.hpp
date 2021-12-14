#ifndef OBJECT_CREATOR_HPP
#define OBJECT_CREATOR_HPP

class ObjectCreator;

class ObjectCreator {
private:

public:
  ObjectCreator();

  BoundObject create_bound_object(std::vector<SidedLine> bounds);
  BoundObject read_bound_object(std::string filename);
};


#endif
