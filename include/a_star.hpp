#include <cmath>
#include <memory>
#include <vector>

using Grid = std::vector<std::vector<int>>;

struct Node {
  Node(int x, int y, float g = 0, float h = 0,
       std::shared_ptr<Node> parent = nullptr)
    : x(x), y(y), g(g), h(h), parent(parent)
  {
  }
  bool operator==(const Node &other) const
  {
    return x == other.x && y == other.y;
  }
  bool operator<(const Node &other) const
  {
    return f > other.f; // for use in priority queue (min-heap)
  }

  int x, y;
  float g, h, f;
  std::shared_ptr<Node> parent = nullptr;
};

bool is_valid(int x, int y, std::vector<std::vector<int>> &grid)
{
  return x >= 0 && x < grid.size() && y >= 0 && y < grid.at(0).size() &&
         grid.at(y).at(x) == 0;
}

float manhattan_distance(const Node &a, const Node &b)
{
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
