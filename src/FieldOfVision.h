//    RogueMoon, a Science-Fiction Roguelike computer game
//    Copyright (C) 2007  Stephan Houben  
//    e-mail: "stephanh at planet dot nl"
//
//    RogueMoon is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    RogueMoon is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef FIELDOFVISION_H
#define FIELDOFVISION_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <vector>
#include "Grid.h"

class TransparencyFunction {
  public:
    typedef bool result_type;

    TransparencyFunction(boost::python::object f)
    : m_f(f), m_grid(0)
    {}

    bool operator()(int x, int y) const
    {
      char ch = m_grid.get(x, y);
      if (ch == 0) {
        ch = (boost::python::extract<bool>(m_f(boost::python::make_tuple(x, y))) ? 1 : 2);
        m_grid.set(x, y, ch);
      }
      return (ch == 1);
    }

  private:
    boost::python::object m_f;
    mutable Grid<char> m_grid;
};

class FieldOfVision
{
  public:
    typedef std::pair<int,int> Position;

    FieldOfVision(boost::python::object f);
    ~FieldOfVision();

    bool is_visible(Position p) const;
    bool is_lit(Position p) const;
    bool is_visible_and_lit(Position p) const;

    int get_visible_subgrid(Position p) const;
    int get_lit_subgrid(Position p) const;
    int get_visible_and_lit_subgrid(Position p) const;

    void place_viewer(Position p, int radius) { compute_fov(2*p.first+1, 2*p.second+1, 2*radius, 1); }
    void place_lightsource(Position p, int radius) { compute_fov(2*p.first+1, 2*p.second+1, 2*radius, 2); }

  private:
    bool check_mask(Position p, char mask) const;
    int get_subgrid(Position p, char mask) const;

    bool get_transparency(int x, int y) const;
    void transform(int &x, int &y, int dir);
    void compute_fov(int x, int y, int radius, char mask);
    void compute_fov_in_direction(int radius, int dir, char mask);

    char get_data(int x, int y) const;
    void set_data(int x, int y, char mask);
    
    int m_xcenter;
    int m_ycenter;

    boost::shared_ptr<TransparencyFunction> m_f;
    Grid<char> m_data;
};

#endif /*FIELDOFVISION_H*/
