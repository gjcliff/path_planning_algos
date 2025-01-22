#pragma once

#include "grid.hpp"
#include <memory>
#include <queue>
#include <unordered_set>

struct breadth_hash {
public:
  template <typename T, typename U>
  int operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

// struct practice {
// public:
//   template <typename T, typename U>
//   int operator()(const std::pair<T, U> &x) const
//   {
//     return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
//   }
// };
//
// struct more_practice {
// public:
//   template <typename T, typename U>
//   std::size_t operator()(const std::pair<T, U> &x) const
//   {
//     return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
//   }
// };
//
// struct more_more {
// public:
//   template <typename T, typename U>
//     int operator()(const std::pair<T, U> &x) const
//     {
//       return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
//     }
// };

struct Node {
  Node(int dist, std::pair<int, int> coord, std::shared_ptr<Node> prev)
    : dist(dist), coord(coord), prev(prev)
  {
  }
  int dist;
  std::pair<int, int> coord;
  std::shared_ptr<Node> prev;
};

class Breadth_First {
public:
  Breadth_First(std::shared_ptr<Grid> grid) : grid_(grid), rd(), gen(rd())
  {
    std::uniform_int_distribution<> row(0, grid_->rows - 1);
    std::uniform_int_distribution<> col(0, grid_->cols - 1);
    do {
      start_pos = {row(gen), col(gen)};
    } while (grid_->grid.at(start_pos.first).at(start_pos.second) == 1);
    do {
      goal_pos = {row(gen), col(gen)};
    } while (grid_->grid.at(goal_pos.first).at(goal_pos.second) == 1);
  }

  void run()
  {
    std::vector<std::pair<int, int>> directions{
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::shared_ptr<Node> current =
      std::make_shared<Node>(0, start_pos, nullptr);

    std::queue<std::shared_ptr<Node>> queue;
    queue.push(current);

    std::unordered_set<std::pair<int, int>, pairhash> visited;

    while (!queue.empty()) {
      int rows = grid_->rows;
      int cols = grid_->cols;

      current = queue.front();
      queue.pop();

      visited.insert(current->coord);

      if (current->coord == goal_pos) {
        std::cout << "Shortest path distance: " << current->dist << std::endl;

        while (current->prev != nullptr) {
          grid_->grid.at(current->coord.first).at(current->coord.second) = 2;
          current = current->prev;
          grid_->grid.at(start_pos.first).at(start_pos.second) = 3;
          grid_->grid.at(goal_pos.first).at(goal_pos.second) = 4;
        }
        return;
      }

      for (const auto &dir : directions) {
        std::pair<int, int> next_coord = {current->coord.first + dir.first,
                                          current->coord.second + dir.second};
        std::shared_ptr<Node> next =
          std::make_shared<Node>(current->dist + 1, next_coord, current);

        if (next->coord.first >= 0 && next->coord.first < rows &&
            next->coord.second >= 0 && next->coord.second < cols &&
            visited.find(next->coord) == visited.end()) {
          if (grid_->grid.at(next->coord.first).at(next->coord.second) != 1) {
            queue.push(next);
          }
        }
      }
    }

    std::cout << "Couldn't find a path to the goal" << std::endl;
    grid_->grid.at(start_pos.first).at(start_pos.second) = 3;
    grid_->grid.at(goal_pos.first).at(goal_pos.second) = 4;
  }

private:
  std::shared_ptr<Grid> grid_;
  std::random_device rd;
  std::mt19937 gen;
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
};
