#pragma once

#include "grid.hpp"
#include <memory>
#include <stack>
#include <unordered_set>

struct depth_hash {
public:
  template <typename T, typename U>
  int operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

struct Depth_Node {
  Depth_Node(int dist, std::pair<int, int> coord,
             std::shared_ptr<Depth_Node> prev)
    : dist(dist), coord(coord), prev(prev)
  {
  }
  int dist;
  std::pair<int, int> coord;
  std::shared_ptr<Depth_Node> prev;
};

class Depth_First {
public:
  Depth_First(Grid &grid, std::pair<int, int> start_pos,
              std::pair<int, int> goal_pos)
    : grid_(grid), rd(), gen(rd()), start_pos(start_pos), goal_pos(goal_pos)
  {
  }

  Grid &run()
  {
    std::vector<std::pair<int, int>> directions{
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::shared_ptr<Depth_Node> current =
      std::make_shared<Depth_Node>(0, start_pos, nullptr);

    std::stack<std::shared_ptr<Depth_Node>> stack;
    stack.push(current);

    std::unordered_set<std::pair<int, int>, depth_hash> visited;

    while (!stack.empty()) {
      int rows = grid_.rows;
      int cols = grid_.cols;

      current = stack.top();
      stack.pop();

      visited.insert(current->coord);

      if (current->coord == goal_pos) {
        std::cout << "Running Depth First Search" << std::endl;
        std::cout << "Shortest path distance: " << current->dist << std::endl;

        while (current->prev != nullptr) {
          grid_.grid.at(current->coord.first).at(current->coord.second) = 2;
          current = current->prev;
          grid_.grid.at(start_pos.first).at(start_pos.second) = 3;
          grid_.grid.at(goal_pos.first).at(goal_pos.second) = 4;
        }
        return grid_;
      }

      for (const auto &dir : directions) {
        std::pair<int, int> next_coord = {current->coord.first + dir.first,
                                          current->coord.second + dir.second};
        std::shared_ptr<Depth_Node> next =
          std::make_shared<Depth_Node>(current->dist + 1, next_coord, current);

        if (next->coord.first >= 0 && next->coord.first < rows &&
            next->coord.second >= 0 && next->coord.second < cols &&
            visited.find(next->coord) == visited.end()) {
          if (grid_.grid.at(next->coord.first).at(next->coord.second) != 1) {
            stack.push(next);
          }
        }
      }
    }

    std::cout << "Couldn't find a path to the goal" << std::endl;
    grid_.grid.at(start_pos.first).at(start_pos.second) = 3;
    grid_.grid.at(goal_pos.first).at(goal_pos.second) = 4;
    return grid_;
  }

private:
  Grid grid_;
  std::random_device rd;
  std::mt19937 gen;
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
};
