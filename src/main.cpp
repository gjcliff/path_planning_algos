#include "grid.hpp"

int main(int argc, char *arv[]) {
  int w, h;
  if (argc < 2) {
    w = 10, h = 10;
  }

  Grid grid(w, h);
  grid.initialize();
  grid.print();

  return 0;
}
