//
// Created by a3-hillman on 28/11/2019.
//

#ifndef PONG_AHILLMAN2000_BAT_H
#define PONG_AHILLMAN2000_BAT_H

#include <Engine/Sprite.h>
//@feedback, never used???
class Player
{
 public:
  void initBat();
  void createBat();

 private:
  ASGE::Sprite* paddle_sprite = nullptr;
  float x = 0;
  float y = 0;
  int score = 0;
};

#endif // PONG_AHILLMAN2000_BAT_H
