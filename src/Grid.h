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
#ifndef GRID_H
#define GRID_H

#include <map>

template <class T, int SHIFT = 4>
class Grid
{
  private:
    static const int N = 1<<SHIFT;
    
    struct square {
      square() {}
      square(const T &default_item)
      {
	for (int iy = 0; iy < N; iy++) {
	  for (int ix = 0; ix < N; ix++) {
	    item[ix][iy] = default_item;
	  }
	}      
      }

      T item[N][N];
    };
    
    typedef std::pair<int32_t, int32_t> pair_t;
    typedef std::map<pair_t, square> map_t;

  public:
    Grid(const T &default_item)
      : m_default_item(default_item)
      {}

    T get(int x, int y) const
    {
      pair_t p(x>>SHIFT, y>>SHIFT);
      typename map_t::const_iterator i = m_map.find(p);
      if (i == m_map.end()) {
	return m_default_item;
      } else {
	return i->second.item[x & (N-1)][y & (N-1)];
      }
    }

    void set(int x, int y, const T &new_value)
    {
      pair_t p(x>>SHIFT, y>>SHIFT);
      typename map_t::iterator i = m_map.find(p);
      if (i == m_map.end()) {
	square new_square(m_default_item);
	new_square.item[x & (N-1)][y & (N-1)] = new_value;
	m_map[p] = new_square;
      } else {
	i->second.item[x & (N-1)][y & (N-1)] = new_value;
      }
    }

    void erase(int x, int y)
    {
      pair_t p(x>>SHIFT, y>>SHIFT);
      typename map_t::iterator i = m_map.find(p);
      if (i != m_map.end()) {
	if (SHIFT == 0) {
	  m_map.erase(i);
	} else {
	  i->second.item[x & (N-1)][y & (N-1)] = m_default_item;
	}
      }
    }

    T get_and_set(int x, int y, const T &new_value)
    {
      pair_t p(x>>SHIFT, y>>SHIFT);
      typename map_t::iterator i = m_map.find(p);
      if (i == m_map.end()) {
	square new_square(m_default_item);
	new_square.item[x & (N-1)][y & (N-1)] = new_value;
	m_map[p] = new_square;
	return m_default_item;
      } else {
	T &location(i->second.item[x & (N-1)][y & (N-1)]);
	T old_value = location;
       	location = new_value;
	return old_value;
      }
    } 

    void clear()
    {
      m_map.clear();
    }

    template <class P>
    void for_each(P callback) const
    {
      for (typename map_t::const_iterator i = m_map.begin(); i != m_map.end(); ++i) {
	const pair_t &p = i->first;
	int x0 = p.first << SHIFT;
	int y0 = p.second << SHIFT;
	for (int x = 0; x < N; x++) {
	  for (int y = 0; y < N; y++) {
	    callback(x0+x, y0+y, i->second.item[x][y]);
	  }
	}
      }
    }

  private:
    map_t m_map;
    T m_default_item;
};

#endif /*GRID_H*/
