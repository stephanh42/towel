#ifndef MESH_H
#define MESH_H

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <vector>
#include <algorithm>
#include <queue>
#include "Grid.h"

enum SearchResult {
  FOUND,
  UNREACHABLE,
  INCOMPLETE
};

typedef std::pair<int,int> Position;


typedef std::vector<Position> PositionVector;

struct Move {
  double estimated_total_cost;
  double cost_so_far;
  Position position;
  char direction;

  bool operator<(const Move &move) const { return estimated_total_cost > move.estimated_total_cost; }
};

typedef boost::python::object CostFunction;

class PathFinder {
  public:
    PathFinder(CostFunction f, Position start, Position end);

    double estimated_cost(Position pos) const;
    bool is_closed(int x, int y) const;
    bool is_closed(Position p) const { return is_closed(p.first, p.second); }
    double cost(Position pos) const;

    bool run(int steps);

    bool incomplete() const { return m_state == INCOMPLETE; }
    bool found() const { return m_state == FOUND; }
    bool unreachable() const { return m_state == UNREACHABLE; }

    boost::python::list path() const;

  private:
    SearchResult real_run(int steps);

    CostFunction m_cost;
    Position m_start;
    Position m_end;
    Grid<char> m_closed;
    std::priority_queue<Move> m_open;
    SearchResult m_state;
};

#endif /*MESH_H*/
