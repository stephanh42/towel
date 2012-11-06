#include <boost/python.hpp>
#include <boost/utility.hpp>
#include <time.h>

extern "C" {
#include "mersenne/mt19937ar.h"
}

#include <iostream>

#include "FieldOfVision.h"
#include "Grid.h"
#include "astar.h"
#include "std_pair_conversion.h"

using namespace boost::python;

object find_path(object passable, Position source, Position dest)
{
  PathFinder pf(passable, source, dest);
  while (pf.incomplete()) {
    pf.run(1000);
  }
  if (pf.found()) {
    return pf.path();
  } else {
    return object();
  }
}

template <class T>
T copy_object(const T& obj)
{
  return obj;
}

object random_walk(int N)
{
  list result;
  unsigned long random_bits = 0;
  int bits_left = 0;
  int x = 0;
  int y = 0;
  result.append(make_tuple(x, y));

  for (int i = 0; i < N; i++) {
    if (bits_left < 2) {
      random_bits = genrand_int32();
      bits_left = 32;
    }
    switch (random_bits & 3) {
      case 0: x++; break;
      case 1: x--; break;
      case 2: y++; break;
      case 3: y--; break;
    }
    random_bits = random_bits >> 2;
    bits_left -= 2;

    result.append(make_tuple(x, y));
  }
  return result;
}

BOOST_PYTHON_MODULE(towel)
{
  init_genrand(time(0));

  class_<FieldOfVision>("FieldOfVision", 
     "An object which computes field of vision (FOV) given the viewer location and some light sources.",
      init<object>())
    .def("place_viewer", &FieldOfVision::place_viewer)
    .def("place_lightsource", &FieldOfVision::place_lightsource)
    .def("is_visible", &FieldOfVision::is_visible)
    .def("is_lit", &FieldOfVision::is_lit)
    .def("is_visible_and_lit", &FieldOfVision::is_visible_and_lit)
    .def("get_visible_subgrid", &FieldOfVision::get_visible_subgrid)
    .def("get_lit_subgrid", &FieldOfVision::get_lit_subgrid)
    .def("get_visible_and_lit_subgrid", &FieldOfVision::get_visible_and_lit_subgrid)
    .def("__copy__", &copy_object<FieldOfVision>)
    ;

  class_<PathFinder>("PathFinder",
      "An object which shortest paths between two points using the A* algorithm.",
      init<object,Position, Position>())
    .def("run", &PathFinder::run)
    .add_property("incomplete", &PathFinder::incomplete)
    .add_property("found", &PathFinder::found)
    .add_property("unreachable", &PathFinder::unreachable)
    .def("path", &PathFinder::path)
    ;

  def("find_path", &find_path);
  def("random_walk", &random_walk);

  boost_adaptbx::std_pair_conversions::to_and_from_tuple<int,int>();
}
