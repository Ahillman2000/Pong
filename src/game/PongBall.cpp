//
// Created by A3-hillman on 27/11/2019.
//

#include "PongBall.h"

PongBall::PongBall()
{
  pong_ball_x = game_width / 2;
  pong_ball_y = game_height / 2;
  pong_ball_width = 32;
  pong_ball_height = 32;
}

void PongBall::AssignVariables(ASGE::Sprite* sprite)
{
  sprite->loadTexture("/data/images/pong_ball.png");
  sprite->xPos(pong_ball_x);
  sprite->yPos(pong_ball_y);
  sprite->width(pong_ball_width);
  sprite->height(pong_ball_height);
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
