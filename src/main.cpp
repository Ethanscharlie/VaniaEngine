#include "App.hpp"

int main() {
  srand(time(0));
  Vania::App app;

  while (app.isRunning()) {
    app.update();
  }
}
