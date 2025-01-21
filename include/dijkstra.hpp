#include "grid.hpp"
#include <memory>
#include <queue>
#include <random>

class Dijkstra {
public:
  Dijkstra(std::shared_ptr<Grid> grid) : gen(rd()), grid_(grid)
  {
    std::uniform_int_distribution<> row(0, grid_->grid.size() - 1);
    std::uniform_int_distribution<> col(0, grid_->grid.at(0).size() - 1);
    do {
      start_pos = {row(gen), col(gen)};
    } while (grid_->grid.at(start_pos.first).at(start_pos.second) == 1);
    do {
      goal_pos = {row(gen), col(gen)};
    } while (grid_->grid.at(goal_pos.first).at(goal_pos.second) == 1);
  }

  void run() { std::priority_queue<std::pair<int, int>> queue; }

private:
  std::pair<int, int> start_pos;
  std::pair<int, int> goal_pos;
  std::random_device rd;
  std::mt19937 gen;
  std::shared_ptr<Grid> grid_;
};
