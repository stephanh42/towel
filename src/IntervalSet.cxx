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
#include "IntervalSet.h"
#include <iostream>
#include <assert.h>

IntervalSet::IntervalSet(double lower_bound, double upper_bound)
: m_is_empty(false), m_lower_bound(lower_bound), m_upper_bound(upper_bound),
  m_lower_part(0), m_upper_part(0)
{
  assert(m_lower_bound <= m_upper_bound);
}

IntervalSet::~IntervalSet()
{
  delete m_lower_part;
  delete m_upper_part;
}

bool IntervalSet::intersects(double lo, double hi) const
{
  if (is_empty()) return false;
  if (hi < lower_bound()) return false;
  if (lo > upper_bound()) return false;
  if (m_lower_part == 0) return true;
  return m_lower_part->intersects(lo, hi) || m_upper_part->intersects(lo, hi);
}

bool IntervalSet::remove_interval(double lo, double hi)
{
  assert(lo <= hi);
  if (is_empty()) return false;
  if (hi < lower_bound()) return false;
  if (lo > upper_bound()) return false;
  if (m_lower_part == 0) {
    if ((lo >= lower_bound()) && (hi <= upper_bound())) {
      m_lower_part = new IntervalSet(lower_bound(), lo);
      m_upper_part = new IntervalSet(hi, upper_bound());
    } else if ((lo < lower_bound()) && (hi > upper_bound())) {
      m_is_empty = true;
    } else if (lo >= lower_bound()) {
      m_upper_bound = lo;
    } else {
      m_lower_bound = hi;
    }
    assert(m_lower_bound <= m_upper_bound);
    return true;
  } else {
    bool r1 = m_lower_part->remove_interval(lo, hi);
    bool r2 = m_upper_part->remove_interval(lo, hi);

    if (m_lower_part->is_empty()) {
      copy_from(m_upper_part);
    } else if (m_upper_part->is_empty()) {
      copy_from(m_lower_part);
    } else {
      m_lower_bound = m_lower_part->lower_bound();
      m_upper_bound = m_upper_part->upper_bound();
      assert(m_lower_bound <= m_upper_bound);
    }
    return r1 || r2;
  }
}

int IntervalSet::num_intervals() const
{
  if (is_empty()) {
    return 0;
  } else if (m_lower_part == 0) {
    return 1;
  } else {
    return m_lower_part->num_intervals() + m_upper_part->num_intervals();
  }
}

void IntervalSet::copy_from(IntervalSet *set)
{
  IntervalSet *set1 = m_lower_part;
  IntervalSet *set2 = m_upper_part;
  
  *this = *set;
  set->m_lower_part = 0;
  set->m_upper_part = 0;
  
  delete set1;
  delete set2;
}

void IntervalSet::debug_print() const
{
  if (is_empty()) {
    std::cerr << "{}";
  } else if (m_lower_part == 0) {
    std::cerr << "[" << lower_bound() << "," << upper_bound() << "]";
  } else {
    m_lower_part->debug_print();
    std::cerr << "U";
    m_upper_part->debug_print();
  }
}

void IntervalSet::selftest()
{
  IntervalSet set(0, 1);
  assert(set.remove_interval(0.3, 0.4));
  assert(set.remove_interval(0.5, 0.7));
  assert(set.num_intervals() == 3);
  assert(!set.intersects(0.31, 0.39));
  assert(set.remove_interval(0.4, 0.5));
  assert(set.num_intervals() == 4);
  assert(set.remove_interval(0.39, 0.41));
  assert(set.remove_interval(0.49, 0.51));
  assert(set.num_intervals() == 2);
  assert(set.remove_interval(-1, 0.31));
  assert(set.remove_interval(0.69, 2));
  assert(set.is_empty());
}
