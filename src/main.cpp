#include "a_star.hpp"
#include "dijkstra.hpp"
#include "grid.hpp"

void print_help()
{
  std::cout
    << "This is a program I wrote to practice path planning "
       "algorithms. It displays a maze randomly generated using Prim's "
       "algorithm, and you have the option of using Dijkstra's algorithm "
       "or the A* algorithm to solve the maze. Sometimes the maze will "
       "not be solveable";
  std::cout << "Usage: main [options]\n";
  std::cout << "Options:\n";
  std::cout << "  -h, --help    Show this help message and exit\n";
  std::cout << "  <algo>        (string) Specify the algorithm to use, manhattan or "
               "euclidean distance\n";
  std::cout << "  <rows>        (int) Specify the number of rows in the maze\n";
  std::cout << "  <cols>        (int) Specify the number of cols in the maze\n";
}

int main(int argc, char *argv[])
{
  int w, h;
  std::string algo;

  if (argc < 2) {
    std::cout << "No arguments provide. Use -h for help\n\n";
    w = 10, h = 10;
  } else if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
    print_help();
    return 0;
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
  a_star.run(algo == "manhattan" ? heuristic::MANHATTAN : heuristic::EUCLIDEAN);
  grid->print();

  return 0;
}
