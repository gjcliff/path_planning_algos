#include <iostream>
#include <random>
#include <vector>

class Grid {
public:
  Grid(int w, int h) : w_(w), h_(h)
  {
    grid_ = std::vector<std::vector<int>>(w, std::vector<int>(h, 0));
  }

  void initialize()
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> u1(0, w_ - 1);
    std::uniform_int_distribution<> u2(0, h_ - 1);
    int start_row = u1(gen);
    int start_col = u2(gen);
    grid_.at(start_row).at(start_col) = 2;
  }

  void print()
  {
    for (int row = 0; row < h_; row++) {
      for (int col = 0; col < w_; col++) {
        std::cout << " | " << grid_.at(row).at(col);
      }
      std::cout << " | " << std::endl;
    }
  }

private:
  int w_, h_;
  std::vector<std::vector<int>> grid_;
};
