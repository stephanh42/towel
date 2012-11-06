#include "astar.h"
#include <algorithm>
#include <math.h>


PathFinder::PathFinder(CostFunction f, Position start, Position end)
: m_cost(f), m_start(start), m_end(end), m_closed(0), m_state(INCOMPLETE)
{
  Move first_move;
  first_move.estimated_total_cost = estimated_cost(m_start);
  first_move.cost_so_far = 0;
  first_move.position = m_start;
  first_move.direction = 0;

  m_open.push(first_move);
}

namespace {
  const double sqrt2 = sqrt(2);

  const int dx[8] = {1, 1, 0, -1, -1, -1,  0,  1};
  const int dy[8] = {0, 1, 1,  1,  0, -1, -1, -1};
}

double PathFinder::estimated_cost(Position pos) const
{
  int dx = abs(pos.first - m_end.first);
  int dy = abs(pos.second - m_end.second);
  int diagonal = std::min(dx, dy);
  int straight = std::max(dx, dy) - diagonal;
  return straight + sqrt2*diagonal;
}

bool PathFinder::is_closed(int x, int y) const
{
  return m_closed.get(x, y);
}

double PathFinder::cost(Position pos) const
{
  double delta_cost;
  delta_cost = boost::python::extract<double>(m_cost(boost::python::make_tuple(pos.first, pos.second)));
  return delta_cost;
}

SearchResult PathFinder::real_run(int steps)
{
  while (steps > 0) {
    if (m_open.empty()) {
      return UNREACHABLE;
    }

    Move move(m_open.top());
    m_open.pop();

    if (is_closed(move.position)) {
      continue;
    }
    m_closed.set(move.position.first, move.position.second, move.direction+1);
    steps--;

    if (move.position == m_end) {
      return FOUND;
    }

    for (int j = 0; j < 8; j++) {
      Move new_move;
      new_move.position = move.position;
      new_move.position.first += dx[j];
      new_move.position.second += dy[j];
      if (is_closed(new_move.position)) {
        continue;
      }
      double delta_cost = cost(new_move.position);
      if (delta_cost <= 0) {
        continue;
      }
      if (j % 2 != 0) {
        delta_cost *= sqrt2;
      }
      new_move.cost_so_far = move.cost_so_far + delta_cost;
      new_move.estimated_total_cost = new_move.cost_so_far + estimated_cost(new_move.position);
      new_move.direction = j;

      m_open.push(new_move);
    }
  }
  return INCOMPLETE;
}

bool PathFinder::run(int steps)
{
  if (incomplete()) {
    m_state = real_run(steps);
  }
  return incomplete();
}

boost::python::list PathFinder::path() const
{
  boost::python::list result;
  Position pos = m_end;
  while (true) {
    result.append(boost::python::make_tuple(pos.first, pos.second));
    if (pos == m_start) break;

    int direction = m_closed.get(pos.first, pos.second);
    if (direction == 0) break;

    direction--;
    int delta_x = dx[direction];
    int delta_y = dy[direction];
    pos.first -= delta_x;
    pos.second -= delta_y;
  }

  result.reverse();

  return result;
}
