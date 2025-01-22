#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_set>
#include <vector>

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

class Grid {
public:
  int rows, cols;
  std::vector<std::vector<int>> grid;
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;

  Grid(int rows, int cols) : rows(rows), cols(cols)
  {
    grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
  }

  Grid(int rows, int cols, int val) : rows(rows), cols(cols)
  {
    grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, val));
  }

  // \brief Use Prim's algorithm to initialize the maze
  void initialize()
  {
    std::unordered_set<std::pair<int, int>, pairhash> visited;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> u1(0, rows - 1); // [0, max]
    std::uniform_int_distribution<> u2(0, cols - 1);
    std::uniform_int_distribution<> rand_frontier(0, 1); // placeholder
    int start_row = u1(gen);
    int start_col = u2(gen);
    std::pair<int, int> current(start_row, start_col);
    grid.at(start_row).at(start_col) = 1;

    std::vector<std::pair<int, int>> frontiers;
    std::vector<std::pair<int, int>> walls;
    std::vector<std::pair<int, int>> directions{
      {0, 2}, {2, 0}, {0, -2}, {-2, 0}};

    // needs to always run at least once
    do {
      for (const auto &dir : directions) {
        std::pair<int, int> candidate(current.first + dir.first,
                                      current.second + dir.second);
        if (candidate.first >= 0 && candidate.first < rows &&
            candidate.second >= 0 && candidate.second < cols &&
            grid.at(candidate.first).at(candidate.second) == 0 &&
            visited.find(candidate) == visited.end()) {
          // grid.at(candidate.first).at(candidate.second) = 1;
          frontiers.push_back(candidate);
          visited.insert(candidate);
        }
      }

      rand_frontier = std::uniform_int_distribution<>(0, frontiers.size() - 1);
      int index;
      int min_dist;
      int retries = 5;
      std::pair<int, int> diff;
      do {
        index = rand_frontier(gen);
        min_dist = std::numeric_limits<int>::max();
        for (const auto &wall : walls) {
          int dist = manhattan_distance(wall, frontiers.at(index));
          if (dist < min_dist) {
            min_dist = dist;
            current = wall;
          }
        }
        diff = {(frontiers.at(index).first + current.first) / 2,
                (frontiers.at(index).second + current.second) / 2};
        retries--;
      } while ((diff.first != current.first && diff.second != current.second) &&
               manhattan_distance(current, frontiers.at(index)) != 2 &&
               retries > 0);
      current = frontiers.at(index);
      frontiers.erase(std::remove(frontiers.begin(), frontiers.end(), current),
                      frontiers.end());
      grid.at(current.first).at(current.second) = 1;
      grid.at(diff.first).at(diff.second) = 1;
    } while (!frontiers.empty());
  }

  int manhattan_distance(std::pair<int, int> first, std::pair<int, int> second)
  {
    return std::abs(first.first - second.first) +
           std::abs(first.second - second.second);
  }

  // \brief Print the maze with pretty colors
  void print()
  {
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        int current = grid.at(row).at(col);
        if (current == 1) {
          // walls
          std::cout << " \033[31m" << grid.at(row).at(col) << "\033[0m";
        } else if (current == 2) {
          // path traveled
          std::cout << " \033[33m" << grid.at(row).at(col) << "\033[0m";
        } else if (current == 3) {
          // start
          std::cout << " \033[32m" << grid.at(row).at(col) << "\033[1m";
        } else if (current == 4) {
          // goal
          std::cout << " \033[32m" << grid.at(row).at(col) << "\033[1m";
        } else {
          // free space
          std::cout << " \033[37m" << grid.at(row).at(col) << "\033[0m";
        }
      }
      std::cout << std::endl;
    }
  }
};
