#ifndef ACTOR_H
#define ACTOR_H

#include <vector>
#include "SDL.h"

class Actor {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Actor(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        body_x(grid_width / 2),
        body_y(grid_height / 2) {}

  void Update();

  bool ActorCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  bool alive{true};
  float body_x;
  float body_y;

 private:
  int grid_width;
  int grid_height;
  void IsAlive(const SDL_Point& current_cell);
  void UpdatePosition();

};

#endif