#include "a_star.hpp"
#include "dijkstra.hpp"
#include "grid.hpp"

int main(int argc, char *argv[])
{
  int w, h;
  std::string algo;
  if (argc < 2) {
    w = 10, h = 10;
  } else if (argc < 3) {
    algo = argv[1];
  } else if (argc < 4) {
    w = std::stoi(argv[1]);
    h = std::stoi(argv[2]);
  } else if (argc < 5) {
    algo = argv[1];
    w = std::stoi(argv[2]);
    h = std::stoi(argv[3]);
  }

  std::shared_ptr<Grid> grid = std::make_shared<Grid>(w, h);
  grid->initialize();

  Dijkstra dijkstra(grid);
  dijkstra.run();
  grid->print();

  grid->reset();
  A_Star a_star(grid);
  a_star.run(heuristic::MANHATTAN);
  grid->print();

  return 0;
}
