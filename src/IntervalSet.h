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
#ifndef INTERVALSET_H
#define INTERVALSET_H

class IntervalSet
{
  public:
    IntervalSet(double lower_bound, double upper_bound);
    ~IntervalSet();

    double lower_bound() const { return m_lower_bound; }
    double upper_bound() const { return m_upper_bound; }

    bool intersects(double lo, double hi) const;
    bool remove_interval(double lo, double hi);

    bool is_empty() const { return m_is_empty; }

    void debug_print() const;
    int num_intervals() const;

    static void selftest();

  private:
    void copy_from(IntervalSet *set);
    
    bool m_is_empty;
    double m_lower_bound;
    double m_upper_bound;
    IntervalSet *m_lower_part;
    IntervalSet *m_upper_part;
};

#endif /*INTERVALSET_H*/
