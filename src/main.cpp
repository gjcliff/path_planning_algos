#include "grid.hpp"
#include "dijkstra.hpp"

int main(int argc, char *argv[]) {
  int w, h;
  if (argc < 2) {
    w = 10, h = 10;
  } else if (argc < 3) {
    std::cerr << "Invalid number of arguments provided" << std::endl;
    return 1;
  } else if (argc < 4) {
    w = std::stoi(argv[1]);
    h = std::stoi(argv[2]);
  }

  Grid grid(w, h);
  grid.initialize();
  grid.print();

  Dijkstra dijkstra(std::make_shared<Grid>(grid));
  dijkstra.run();

  return 0;
}
