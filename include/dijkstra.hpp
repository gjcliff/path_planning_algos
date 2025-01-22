#pragma once

#include "grid.hpp"
#include <limits>
#include <memory>
#include <queue>
#include <random>
#include <unordered_map>

struct Dijkstra_Node {
  Dijkstra_Node(int dist, std::pair<int, int> coord) : dist(dist), coord(coord)
  {
  }
  int dist;
  std::pair<int, int> coord;
};

class Dijkstra {
public:
  Dijkstra(Grid &grid, std::pair<int, int> start_pos,
           std::pair<int, int> goal_pos)
    : gen(rd()), grid_(grid), start_pos(start_pos), goal_pos(goal_pos)
  {
  }

  Grid &run()
  {
    std::cout << "Running Dijkstra's Algorithm" << std::endl;
    Grid distances(grid_.rows, grid_.cols, std::numeric_limits<int>::max());
    distances.grid.at(start_pos.first).at(start_pos.second) = 0;

    // priority queue is max-heap by default, which means it tries to extract
    // the largest element from the queue. We want the smalled element (shortest
    // distance), so we can pass it a custom comparison function to accomplish
    // that
    auto cmp = [](const Dijkstra_Node a, const Dijkstra_Node b) {
      return a.dist > b.dist;
    };
    std::priority_queue<struct Dijkstra_Node, std::vector<struct Dijkstra_Node>,
                        decltype(cmp)>
      queue(cmp);

    queue.push({0, start_pos});

    std::vector<std::pair<int, int>> directions{
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    std::unordered_map<std::pair<int, int>, std::pair<int, int>, pairhash> map;

    while (!queue.empty()) {
      struct Dijkstra_Node current =
        queue.top(); // grab the first element from the queue
      queue.pop();   // remove the first element from the queue
      if (current.coord == goal_pos) {
        std::cout << "Shortest path distance: " << current.dist << std::endl;
        std::pair<int, int> current = goal_pos;
        while (map.find(current) != map.end()) {
          grid_.grid.at(current.first).at(current.second) = 2;
          current = map[current];
        }
        grid_.grid.at(start_pos.first).at(start_pos.second) = 3;
        grid_.grid.at(goal_pos.first).at(goal_pos.second) = 4;
        return grid_;
      }

      int row = current.coord.first;
      int col = current.coord.second;

      // now we process the neighbors
      for (const auto &dir : directions) {
        std::pair<int, int> neighbor = {row + dir.first, col + dir.second};
        // bounds checking, and also check if the neighbor is not a wall
        if (neighbor.first >= 0 && neighbor.first < grid_.rows &&
            neighbor.second >= 0 && neighbor.second < grid_.cols &&
            grid_.grid.at(neighbor.first).at(neighbor.second) == 0) {
          int new_dist = current.dist + 1;

          if (new_dist <
              distances.grid.at(neighbor.first).at(neighbor.second)) {
            distances.grid.at(neighbor.first).at(neighbor.second) = new_dist;
            queue.push({new_dist, neighbor});
            map[neighbor] = current.coord;
          }
        }
      }
    }

    std::cout << "No path found from start to goal" << std::endl;
    grid_.grid.at(start_pos.first).at(start_pos.second) = 3;
    grid_.grid.at(goal_pos.first).at(goal_pos.second) = 4;
    return grid_;
  }

private:
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
  std::random_device rd;
  std::mt19937 gen;
  Grid grid_;
};
