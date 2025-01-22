#pragma once

#include "grid.hpp"
#include <functional>
#include <limits>
#include <memory>
#include <queue>
#include <random>
#include <unordered_map>

struct A_Star_Node {
  A_Star_Node(int g, int f, std::pair<int, int> coord)
    : g(g), f(f), coord(coord)
  {
  }
  int g, f;
  std::pair<int, int> coord;
};

enum heuristic { MANHATTAN, EUCLIDEAN };

class A_Star {
public:
  A_Star(std::shared_ptr<Grid> grid) : gen(rd()), grid_(grid)
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

  int manhattan_distance(std::pair<int, int> a, std::pair<int, int> b)
  {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
  }

  float euclidean_distance(std::pair<int, int> a, std::pair<int, int> b)
  {
    return std::sqrt(std::pow(a.first - b.first, 2) +
                     std::pow(a.second - b.second, 2));
  }

  void run(heuristic strat)
  {
    std::cout << "Running A*" << std::endl;
    Grid distances(grid_->rows, grid_->cols, std::numeric_limits<int>::max());
    distances.grid.at(start_pos.first).at(start_pos.second) = 0;

    // priority queue is max-heap by default, which means it tries to extract
    // the largest element from the queue. We want the smalled element (shortest
    // distance), so we can pass it a custom comparison function to accomplish
    // that
    std::function<bool(const A_Star_Node, const A_Star_Node)> cmp =
      [](const A_Star_Node a, const A_Star_Node b) { return a.f > b.f; };
    // something new, decltype. We have to use decltype because the lambda
    // function is declared with the auto keyword, and decltype tells the
    // compiler to figure out the type at compile time. Without this you have
    // to make a struct that overrides the () operator like:
    // struct Compare {
    //   bool operator()(const A_Star_Node &a, const A_Star_Node &b) const {
    //     return a.f > b.f; // for example, you can do anything
    //   }
    // }
    // and then you pass that into the third parameter slot in priority_queue
    std::priority_queue<A_Star_Node, std::vector<A_Star_Node>, decltype(cmp)>
      queue(cmp);

    queue.push({0, 0, start_pos});

    std::vector<std::pair<int, int>> directions{
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::unordered_map<std::pair<int, int>, std::pair<int, int>, pairhash> map;

    while (!queue.empty()) {
      struct A_Star_Node current =
        queue.top(); // grab the first element from the queue
      queue.pop();   // remove the first element from the queue
      if (current.coord == goal_pos) {
        std::cout << "Shortest path distance: " << current.f << std::endl;
        std::pair<int, int> current = goal_pos;
        while (map.find(current) != map.end()) {
          grid_->grid.at(current.first).at(current.second) = 2;
          current = map[current];
        }
        grid_->grid.at(start_pos.first).at(start_pos.second) = 3;
        grid_->grid.at(goal_pos.first).at(goal_pos.second) = 4;
        return;
      }

      int row = current.coord.first;
      int col = current.coord.second;

      // now we process the neighbors
      for (const auto &dir : directions) {
        std::pair<int, int> neighbor = {row + dir.first, col + dir.second};
        // bounds checking, and also check if the neighbor is not a wall
        if (neighbor.first >= 0 && neighbor.first < grid_->rows &&
            neighbor.second >= 0 && neighbor.second < grid_->cols &&
            grid_->grid.at(neighbor.first).at(neighbor.second) == 0) {
          int g = current.g + 1;
          int h;
          if (strat == heuristic::MANHATTAN) {
            h = manhattan_distance(neighbor, goal_pos);
          } else if (strat == heuristic::EUCLIDEAN) {
            h = euclidean_distance(neighbor, goal_pos);
          }
          int f = g + h;
          if (f < distances.grid.at(neighbor.first).at(neighbor.second)) {
            distances.grid.at(neighbor.first).at(neighbor.second) = f;
            queue.push({g, f, neighbor});
            map[neighbor] = current.coord;
          }
        }
      }
    }

    std::cout << "No path found from start to goal" << std::endl;
    grid_->grid.at(start_pos.first).at(start_pos.second) = 3;
    grid_->grid.at(goal_pos.first).at(goal_pos.second) = 4;
  }

private:
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
  std::random_device rd;
  std::mt19937 gen;
  std::shared_ptr<Grid> grid_;
};
