#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Player.h"
#include "PongBall.h"
#include "Vector.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class Pong : public ASGE::OGLGame
{
 public:
  Pong();
  ~Pong() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void setupResolution();

  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;
  void createPongBall();
  void initVelocity();

  bool hitDetection();
  bool initPongBall();

  PongBall ball_variables;
  // PongBat bat_variables;

  float pong_ball_x_velocity = -250;
  float pong_ball_y_velocity = 250;
  constexpr static const float PLAYER_VELOCITY = 500;

  ASGE::Sprite* pong_ball = nullptr;

  //@feedback, use a player class instead
  ASGE::Sprite* player_1 = nullptr;
  ASGE::Sprite* player_2 = nullptr;
  bool initPlayerOne();
  void createPlayerOne();
  bool initPlayerTwo();
  void createPlayerTwo();
  bool player_1_up = false;
  bool player_1_down = false;
  bool player_2_up = false;
  bool player_2_down = false;
  int player_1_score = 0;
  int player_2_score = 0;

  int winner = 0;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  int menu_option = 0;
  bool in_menu = true;
  bool in_game_screen = false;
  bool win = false;
  void handleGame(const ASGE::KeyEvent* key);
  void handleWinState(const ASGE::KeyEvent* key);
};