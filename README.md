towel
=====

Towel 0.1 documentation
Stephan Houben

1. Introduction

Towel is a Python library, written in C++, for performing
some common tasks in "roguelikes" and other 2D tile-based games.
Currently it focuses on the following tasks:

1. Field-of-view (FOV) calculation.
2. Pathfinding (using the A* algorithm).
3. Generating random walks.

The library takes the principle that it doesn't want to take over your
program and your data structures. Therefore, rather than providing
a "level map" object of its own, all algorithms take a Python callback
function that should provide some information regarding the map.

All positions are represented as a tuple of two integers (x, y).
Note that all callback get always a single argument consisting of
such a tuple, never a separate x and y coordinate.

A reasonable data structure for not-too-large maps is simply a Python dict,
mapping such position tuples to whatever you use to represent tiles.
For example:
     FLOOR = '.'
     WALL = '#'
     WATER = '~'

     map = {}
     map[(0,0)] = FLOOR
     map[(0,1)] = WATER

     def get_tile(pos):
          # everything not explicitely set is wall
          return map.get(pos, WALL) 
   
This setup will be used in the examples.

The library is imported in Python with:

  import towel


2. Field-of-view (FOV) calculation

  Class: towel.FieldOfVision

  Constructor: __init__(callback)
    
    callback - a Python function or other callable object

    The callback should provide information whether a certain tile is
    transparent or not.
    The callback takes a single argument, namely the position of a tile.
    The callback should then return True if the tile is indeed transparent,
    and False otherwise.

    Example:
       fov = towel.FieldOfVision(lambda p: tile(p) != WALL)

    This assumes all tiles except walls are transparent (so WATER is also
    transparent).
   
  Method: place_viewer(position, radius)
  
    position - a pair of integers indicating a position in the 2D plane
    radius - distance that the viewer can see (an integer)

    This method marks all tiles as "seen" which are visible from 'position'
    and not obstructed by some non-transparent tile.
    Typically you would use the position of the player character(s) for this.
    Note that you can call this multiple times to have multiple viewers.

  Method: place_light(position, radius)
  
    position - a pair of integers indicating a position in the 2D plane
    radius - distance that the viewer can see (an integer)

    This method marks all tiles as "lighted" which are visible from 'position'
    and not obstructed by some non-transparent tile.
    Typically you would use the position of the lightsources for this.
    Note that you can call this multiple times to have multiple lightsources.



