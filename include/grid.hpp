#include <algorithm>
#include <iostream>
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
  Grid(int rows, int cols) : rows_(rows), cols_(cols)
  {
    grid_ = std::vector<std::vector<int>>(rows_, std::vector<int>(cols_, 0));
  }

  // \brief Use Prim's algorithm to initialize the maze
  void initialize()
  {
    std::unordered_set<std::pair<int, int>, pairhash> visited;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> u1(0, rows_ - 1); // [0, max]
    std::uniform_int_distribution<> u2(0, cols_ - 1);
    std::uniform_int_distribution<> rand_frontier(0, 1); // placeholder
    int start_row = u1(gen);
    int start_col = u2(gen);
    std::pair<int, int> current(start_row, start_col);
    grid_.at(start_row).at(start_col) = 1;

    std::vector<std::pair<int, int>> frontier;
    std::vector<std::pair<int, int>> directions{
      {0, 2}, {2, 0}, {0, -2}, {-2, 0}};

    // do while loop so that it always runs at least once
    do {
      for (const auto &dir : directions) {
        std::pair<int, int> candidate(current.first + dir.first,
                                      current.second + dir.second);
        if (candidate.first >= 0 && candidate.first < rows_ &&
            candidate.second >= 0 && candidate.second < cols_ &&
            grid_.at(candidate.first).at(candidate.second) == 0 &&
            visited.find(candidate) == visited.end()) {
          // grid_.at(candidate.first).at(candidate.second) = 1;
          frontier.push_back(candidate);
          visited.insert(candidate);
        }
      }

      rand_frontier = std::uniform_int_distribution<>(0, frontier.size() - 1);
      int index = rand_frontier(gen);
      std::pair<int, int> diff = {
        (frontier.at(index).first + current.first) / 2,
        (frontier.at(index).second + current.second) / 2};
      current = frontier.at(index);
      frontier.erase(std::remove(frontier.begin(), frontier.end(), current),
                     frontier.end());
      grid_.at(current.first).at(current.second) = 1;
      grid_.at(diff.first).at(diff.second) = 1;
      // frontier = {};
    } while (!frontier.empty());
  }

  void print()
  {
    for (int row = 0; row < rows_; row++) {
      for (int col = 0; col < cols_; col++) {
        int current = grid_.at(row).at(col);
        if (current == 1) {
          std::cout << " \033[31m" << grid_.at(row).at(col) << "\033[0m";
        } else {
          std::cout << " \033[32m" << grid_.at(row).at(col) << "\033[0m";
        }
      }
      std::cout << std::endl;
    }
  }

private:
  int rows_, cols_;
  std::vector<std::vector<int>> grid_;
};
