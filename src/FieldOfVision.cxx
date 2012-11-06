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
#include "FieldOfVision.h"
#include "IntervalSet.h"
#include <assert.h>

FieldOfVision::FieldOfVision(boost::python::object f)
: m_f(new TransparencyFunction(f)), m_data(0)
{}

FieldOfVision::~FieldOfVision()
{}

bool FieldOfVision::get_transparency(int x, int y) const
{
  return (*m_f)(x>>1, y>>1);
}

bool FieldOfVision::check_mask(Position p, char mask) const 
{
  int x = p.first;
  int y = p.second;
  for (int dx = 0; dx < 2; dx++) {
    for (int dy = 0; dy < 2; dy++) {
      char data = get_data(2*x + dx, 2*y + dy);
      if ((data & mask) == mask) return true;
    }
  }
  return false;
}

int FieldOfVision::get_subgrid(Position p, char mask) const 
{
  int x = p.first;
  int y = p.second;
  int result = 0;
  int bits = 1;
  for (int dy = 0; dy < 2; dy++) {
    for (int dx = 0; dx < 2; dx++) {
      char data = get_data(2*x + dx, 2*y + dy);
      if ((data & mask) == mask) {
        result = result | bits;
      }
      bits = bits << 1;
    }
  }
  return result;
}


bool FieldOfVision::is_visible(Position p) const
{
  return check_mask(p, 1);
}

bool FieldOfVision::is_lit(Position p) const
{
  return check_mask(p, 2);
}

bool FieldOfVision::is_visible_and_lit(Position p) const
{
  return check_mask(p, 3);
}

int FieldOfVision::get_visible_subgrid(Position p) const
{
  return get_subgrid(p, 1);
}

int FieldOfVision::get_lit_subgrid(Position p) const
{
  return get_subgrid(p, 2);
}

int FieldOfVision::get_visible_and_lit_subgrid(Position p) const
{
  return get_subgrid(p, 3);
}

void FieldOfVision::transform(int &x, int &y, int dir)
{
  if (dir & 1) {
    int tmp = x;
    x = y;
    y = tmp;
  }
  if (dir & 2) {
    x = -x-1;
  }
  if (dir & 4) {
    y = -y-1;
  }
  x += m_xcenter;
  y += m_ycenter;
}

void FieldOfVision::compute_fov(int x, int y, int radius, char mask)
{
  m_xcenter = x;
  m_ycenter = y;
  set_data(x, y, mask);
  set_data(x-1, y, mask);
  set_data(x, y-1, mask);
  set_data(x-1, y-1, mask);
  for (int dir = 0; dir < 8; dir++) {
    compute_fov_in_direction(radius, dir, mask);
  }
}

void FieldOfVision::compute_fov_in_direction(int radius, int dir, char mask)
{
  IntervalSet set(1e-6, 1.0);
  int radius2 = radius*radius;
  for (int x = 1; x <= radius; x++) {
    int max_y2 = radius2 - x*x;
    for (int y = 0; (y <= x) && (y*y <= max_y2); y++) {
      int xt = x;
      int yt = y;
      transform(xt, yt, dir);
      double lo = y/(x+1.0);
      double hi = (y+1.0)/x;
      if (get_transparency(xt, yt)) {
	if (set.intersects(lo, hi)) {
	  set_data(xt, yt, mask);
	}
      } else {
	if (set.remove_interval(lo, hi)) {
	  set_data(xt, yt, mask);
	  if (set.is_empty()) return;
	}
      }
    }
  }
}

char
FieldOfVision::get_data(int x, int y) const
{
  return m_data.get(x, y);
}

void 
FieldOfVision::set_data(int x, int y, char mask)
{
  m_data.set(x, y, m_data.get(x, y)|mask);
}
