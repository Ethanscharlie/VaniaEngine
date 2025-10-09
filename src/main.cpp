#include "App.hpp"

int main() {
  Vania::App app;

  while (app.isRunning()) {
    app.update();
  }
}
