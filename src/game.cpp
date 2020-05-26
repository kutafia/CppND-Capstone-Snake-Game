#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : actor(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) 
{
  PlaceFood();
  PlaceEnemies();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) 
{
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) 
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, actor);
    Update();
    renderer.Render(actor, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) 
    {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) 
    {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() 
{
  int x, y;
  while (true) 
  {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a actor item before placing
    // food.
    if (actor.GetActorPosition().x != x || actor.GetActorPosition().y != y) 
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceEnemies() 
{
  int x, y;
  while (enemies.size() < 5) 
  {
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a actor item before placing
    // enemy. If it is occupied by food, it's not a problem since enemy is moving.
    if (actor.GetActorPosition().x != x || actor.GetActorPosition().y != y) 
    {
      SDL_Point position;
      position.x = x;
      position.y = y;
      
      Enemy::Direction direction;
      int rand_direction = 1 + (rand() % 4); // based on the recommendation from https://www.bitdegree.org/learn/random-number-generator-cpp
      switch (rand_direction)
      {
      case 2:
        direction = Enemy::Direction::kLeft;
        break;

      case 3:
        direction = Enemy::Direction::kRight;
        break;

      case 4:
        direction = Enemy::Direction::kDown;
        break;
      
      default:
        direction = Enemy::Direction::kUp;
        break;
      }
      Enemy enemy (position, direction);
      enemies.push_back(enemy);
      return;
    }
  }
}

void Game::Update() 
{
  if (!actor.IsAlive()) return;

  actor.Update();

  int new_x = static_cast<int>(actor.GetActorPosition().x);
  int new_y = static_cast<int>(actor.GetActorPosition().y);

  // Check if there're enemies over here
  for(auto const& enemy: enemies) 
  {
    if (enemy.GetEnemyPosition().x == new_x && enemy.GetEnemyPosition().y == new_y) actor.SetAlive(false);
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) 
  {
    score++;
    PlaceFood();
    // Increase actor's speed.
    actor.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
