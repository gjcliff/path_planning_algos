#pragma once

#include "grid.hpp"
#include <limits>
#include <memory>
#include <queue>
#include <random>

struct Node {
  Node(int dist, std::pair<int, int> coord) : dist(dist), coord(coord) {}
  int dist;
  std::pair<int, int> coord;
};

class Dijkstra {
public:
  Dijkstra(std::shared_ptr<Grid> grid) : gen(rd()), grid_(grid)
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
    Grid distances(grid_->rows, grid_->cols, std::numeric_limits<int>::max());
    distances.grid.at(start_pos.first).at(start_pos.second) = 0;

    // priority queue is max-heap by default, which means it tries to extract
    // the largest element from the queue. We want the smalled element (shortest
    // distance), so we can pass it a custom comparison function to accomplish
    // that
    auto cmp = [](const Node a, const Node b) { return a.dist > b.dist; };
    std::priority_queue<struct Node, std::vector<struct Node>, decltype(cmp)>
      queue(cmp);

    queue.push({0, start_pos});

    std::vector<std::pair<int, int>> directions{
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (!queue.empty()) {
      struct Node current =
        queue.top(); // grab the first element from the queue
      queue.pop();   // remove the first element from the queue
      if (current.coord == goal_pos) {
        std::cout << "Shortest path distance: " << current.dist << std::endl;
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
          int new_dist = current.dist + 1;

          if (new_dist <
              distances.grid.at(neighbor.first).at(neighbor.second)) {
            distances.grid.at(neighbor.first).at(neighbor.second) = new_dist;
            queue.push({new_dist, neighbor});
          }
        }
      }
    }
    std::cout << "No path found from start to goal" << std::endl;
  }

private:
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
  std::random_device rd;
  std::mt19937 gen;
  std::shared_ptr<Grid> grid_;
};
