#include "a_star.hpp"
#include "dijkstra.hpp"
#include "grid.hpp"
#include "breadth_first.hpp"
#include "depth_first.hpp"

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

  Grid grid = Grid(w, h);
  // std::pair<int,int> start_pos;
  // std::pair<int,int> goal_pos;
  auto [start_pos, goal_pos] = grid.initialize();

  Dijkstra dijkstra(grid, start_pos, goal_pos);
  Grid dijkstra_grid = dijkstra.run();
  dijkstra_grid.print();

  A_Star a_star(grid, start_pos, goal_pos);
  Grid a_star_grid = a_star.run(algo == "manhattan" ? heuristic::MANHATTAN : heuristic::EUCLIDEAN);
  a_star_grid.print();

  Breadth_First breadth_first(grid, start_pos, goal_pos);
  Grid breadth_first_grid = breadth_first.run();
  breadth_first_grid.print();

  Depth_First depth_first(grid, start_pos, goal_pos);
  Grid depth_first_grid = depth_first.run();
  depth_first_grid.print();

  return 0;
}
