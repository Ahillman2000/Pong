//
// Created by A3-hillman on 27/11/2019.
//

#ifndef PONG_AHILLMAN2000_PONGBALL_H
#define PONG_AHILLMAN2000_PONGBALL_H

#include <Engine/Sprite.h>

class PongBall
{
 public:
  PongBall();
  void AssignVariables(ASGE::Sprite* sprite);

 private:
  float pong_ball_x = 0;
  float pong_ball_y = 0;
  float pong_ball_width = 0;
  float pong_ball_height = 0;

  //@feedback, ball shouldn't know about the game window, game should manage the
  // ball
  float game_width = 1280;
  float game_height = 720;

  //@feedback, store the sprite here instead, it has position and width data
  ASGE::Sprite* ball_sprite = nullptr;
};

#endif // PONG_AHILLMAN2000_PONGBALL_H
