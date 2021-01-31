#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "game.h"
#include <math.h>

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
Pong::Pong()
{
  game_name = "ASGE Game";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
Pong::~Pong()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool Pong::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  if (!initPongBall())
  {
    return false;
  }

  if (!initPlayerOne())
  {
    return false;
  }

  if (!initPlayerTwo())
  {
    return false;
  }

  initVelocity();

  createPongBall();
  createPlayerOne();
  createPlayerTwo();

  toggleFPS();

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Pong::keyHandler, this);

  /*mouse_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Pong::clickHandler, this);*/

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void Pong::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/
  // Scaling_and_MultiResolution_in_2D_Games.php

  // 720p is a pretty modest starting point, consider 1080p
  game_width = 1280;
  game_height = 720;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void Pong::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (in_menu)
  {
    //@feedback use if/pressed/released instead
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      // .. do stuff
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      // .. do stuff
    }

    if (key->key == ASGE::KEYS::KEY_LEFT &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      menu_option = 1 - menu_option;
      // ASGE::DebugPrinter{} << menu_option << std::endl;
    }
    else if (key->key == ASGE::KEYS::KEY_RIGHT &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
      menu_option = 1 - menu_option;
      // ASGE::DebugPrinter{} << menu_option << std::endl;
    }
    else if (key->key == ASGE::KEYS::KEY_ENTER &&
             key->action == ASGE::KEYS::KEY_RELEASED)
    {
      if (menu_option == 1)
      {
        signalExit();
      }
      else
      {
        // leaves menu screen and enters game screen
        in_menu = false;
        in_game_screen = true;
      }
    }
  }

  else if (in_game_screen)
  {
    handleGame(key);
  }
  else if (win)
  {
    handleWinState(key);
  }
}
void Pong::handleWinState(const ASGE::KeyEvent* key)
{
  if (key->key == ASGE::KEYS::KEY_ENTER &&
      key->action == ASGE::KEYS::KEY_RELEASED)
  {
    // Ability to restart game
    win = false;
    in_menu = true;
    player_1_score = 0;
    player_2_score = 0;
    init();
  }
}
void Pong::handleGame(const ASGE::KeyEvent* key)
{ // ASGE::DebugPrinter{} << "IN GAME SCREEN" << std::endl;
  if (key->key == ASGE::KEYS::KEY_W)
  {
    // move player_1 up
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      player_1_up = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player_1_up = false;
    }

    /*player_1_y -= 10;
    player_1->yPos(player_1_y);*/
  }
  else if (key->key == ASGE::KEYS::KEY_S)
  {
    // move player_1 down
    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      player_1_down = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player_1_down = false;
    }
  }

  if (key->key == ASGE::KEYS::KEY_I)
  {
    // move player_2 up

    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      player_2_up = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player_2_up = false;
    }

    /*player_2_y -= 10;
    player_2->yPos(player_2_y);*/
  }
  else if (key->key == ASGE::KEYS::KEY_K)
  {
    // move player_2 down

    if (key->action == ASGE::KEYS::KEY_PRESSED)
    {
      player_2_down = true;
    }
    else if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player_2_down = false;
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
/*void Pong::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());
  double x_pos = click->xpos;
  double y_pos = click->ypos;

  // ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  // ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}*/

bool Pong::initPongBall()
{
  pong_ball = renderer->createRawSprite();

  ball_variables.AssignVariables(pong_ball);

  return true;
}

void Pong::createPongBall()
{
  float x_pos = game_width / 2.0;
  float y_pos = game_height / 2.0;

  pong_ball->xPos(x_pos);
  pong_ball->yPos(y_pos);
}

bool Pong::initPlayerOne()
{
  player_1 = renderer->createRawSprite();

  if (!player_1->loadTexture("/data/images/player1.png"))
  // if (!pong_ball->loadTexture("/data/images/ball.png"))
  {
    ASGE::DebugPrinter{} << "init::Failed to load player1" << std::endl;
    return false;
  }

  player_1->width(32);
  player_1->height(128);
  player_1->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

  return true;
}

void Pong::createPlayerOne()
{
  float x_pos = 50;
  float y_pos = game_height / 2.0;

  player_1->xPos(x_pos);
  player_1->yPos(y_pos);
}

bool Pong::initPlayerTwo()
{
  player_2 = renderer->createRawSprite();

  if (!player_2->loadTexture("/data/images/player2.png"))
  // if (!pong_ball->loadTexture("/data/images/ball.png"))
  {
    ASGE::DebugPrinter{} << "init::Failed to load player2" << std::endl;
    return false;
  }

  player_2->width(32);
  player_2->height(128);
  player_2->setFlipFlags(ASGE::Sprite::FlipFlags::FLIP_X);

  return true;
}

void Pong::createPlayerTwo()
{
  float x_pos = game_width - (player_2->height() - 50);
  float y_pos = game_height / 2.0;

  player_2->xPos(x_pos);
  player_2->yPos(y_pos);
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::initVelocity()
{
  pong_ball_x_velocity = -250;
  pong_ball_y_velocity = 250;
}

void Pong::update(const ASGE::GameTime& game_time)
{
  auto dt_sec = game_time.delta.count() / 1000.0;
  // make sure you use delta time in any movement calculations!

  if (in_game_screen)
  {
    //@feedback, needs refactoring
    float x_pos = pong_ball->xPos();
    float y_pos = pong_ball->yPos();

    // moves pong ball by a set x and y velocity per frame
    x_pos += pong_ball_x_velocity * dt_sec;
    y_pos += pong_ball_y_velocity * dt_sec;
    // ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;

    pong_ball->xPos(x_pos);
    pong_ball->yPos(y_pos);

    if (player_1_down)
    {
      player_1->yPos(player_1->yPos() + PLAYER_VELOCITY * dt_sec);
    }
    else if (player_1_up)
    {
      player_1->yPos(player_1->yPos() - PLAYER_VELOCITY * dt_sec);
    }
    else if (player_2_down)
    {
      player_2->yPos(player_2->yPos() + PLAYER_VELOCITY * dt_sec);
    }
    else if (player_2_up)
    {
      player_2->yPos(player_2->yPos() - PLAYER_VELOCITY * dt_sec);
    }

    if (x_pos + pong_ball->width() <= 0)
    {
      // if ball goes off screen to the left, increment score, resets speed and
      // redraw ball
      player_2_score++;
      initVelocity();
      createPongBall();
    }

    if (x_pos >= (game_width - pong_ball->width()))
    {
      // if ball goes off screen to the right, increment score, resets speed and
      // redraw ball
      player_1_score++;
      initVelocity();
      createPongBall();
    }

    if (y_pos <= 0 || y_pos >= (game_height - pong_ball->height()))
    {
      // if ball touched top or bottom of screen then invert the y velocity
      pong_ball_y_velocity *= -1;
      // ASGE::DebugPrinter{} << "Velocity: " << pong_ball_velocity <<
      // std::endl; ASGE::DebugPrinter{} << "hit boundary"<< std::endl;
    }

    hitDetection();

    if (player_1_score == 11)
    {
      // ASGE::DebugPrinter{} << "WIN" << std::endl;
      // player 1 win
      winner = 1;
      // leaves game screen
      in_game_screen = false;
      // enters win screen
      win = true;

      // resets players movement to stationary
      player_1_up = false;
      player_1_down = false;
      player_2_up = false;
      player_2_down = false;
    }

    else if (player_2_score == 11)
    {
      // ASGE::DebugPrinter{} << "WIN" << std::endl;
      // player 2 win
      winner = 2;

      // leaves game screen
      in_game_screen = false;
      // enters win screen
      win = true;

      // resets players movement to stationary
      player_1_up = false;
      player_1_down = false;
      player_2_up = false;
      player_2_down = false;
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void Pong::render(const ASGE::GameTime& game_time)
{
  renderer->setFont(0);
  // renderer->setFont(fonts[FONT_LARGE]);

  if (in_menu)
  {
    // RENDER MAIN MENU
    renderer->renderText(
      "PONG", (game_width / 2), (game_height * 0.35), 2.0, ASGE::COLOURS::WHITE);

    renderer->renderText("PLAYER 1: UP: W, DOWN:S",
                         ((game_width / 2) - 200),
                         (game_height * 0.35 + 100),
                         2.0,
                         ASGE::COLOURS::WHITE);
    renderer->renderText("PLAYER 2: UP: I, DOWN:K",
                         ((game_width / 2) - 200),
                         (game_height * 0.35 + 200),
                         2.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 0 ? ">PLAY" : "PLAY",
                         (game_width * 0.35),
                         (game_height * 0.8),
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText(menu_option == 1 ? ">EXIT" : "EXIT",
                         (game_width * 0.55),
                         (game_height * 0.8),
                         1.0,
                         ASGE::COLOURS::WHITE);
  }

  else if (in_game_screen)
  {
    // RENDER GAME SCREEN
    for (int y = -48; y <= game_height; y += 80)
    {
      renderer->renderText(
        "| \n", (game_width / 2), (y), 2.0, ASGE::COLOURS::WHITE);
    }
    renderer->renderSprite(*pong_ball);
    renderer->renderSprite(*player_1);
    renderer->renderSprite(*player_2);

    renderer->renderText("player 1 Score: " + std::to_string(player_1_score),
                         (200),
                         (50),
                         1.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText("player 2 Score: " + std::to_string(player_2_score),
                         (game_width - 400),
                         (50),
                         1.0,
                         ASGE::COLOURS::WHITE);
  }

  if (win)
  {
    // RENDER WIN SCREEN
    // ASGE::DebugPrinter{} << "WIN" << std::endl;

    if (winner == 1)
    {
      renderer->renderText("WINNER: PLAYER 1",
                           (game_width * 0.35),
                           ((game_height / 2) - 43),
                           2.0,
                           ASGE::COLOURS::WHITE);
    }
    else if (winner == 2)
    {
      renderer->renderText("WINNER: PLAYER 2",
                           (game_width * 0.35),
                           ((game_height / 2) - 43),
                           2.0,
                           ASGE::COLOURS::WHITE);
    }

    renderer->renderText("Press ENTER to replay",
                         ((game_width / 2) - 200),
                         ((game_height / 2) + 20),
                         2.0,
                         ASGE::COLOURS::WHITE);

    renderer->renderText("Press ESCAPE to exit",
                         ((game_width / 2) - 200),
                         ((game_height / 2) + 115),
                         2.0,
                         ASGE::COLOURS::WHITE);
  }
}

bool Pong::hitDetection()
{
  if ((pong_ball->xPos() < player_1->xPos() + player_1->width()) &&
      (pong_ball->xPos() + pong_ball->width() > player_1->xPos()) &&

      (pong_ball->yPos() < player_1->yPos() + player_1->height()) &&
      (pong_ball->yPos() + pong_ball->height() > player_1->yPos()))
  {
    // if ball hits palayer 1 then invert x velocity and increase speed by 10%
    pong_ball_x_velocity *= -1;

    //@feedback, what the heck is 863 velocity? are you clamping speeds?
    if (pong_ball_x_velocity < 863.068 && pong_ball_x_velocity > -863.068)
    {
      pong_ball_x_velocity *= 1.1;
    }
    // ASGE::DebugPrinter{} << pong_ball_x_velocity << std::endl;

    // ASGE::DebugPrinter{} << "BOUNCE PLAYER 1" << std::endl;
  }

  if ((pong_ball->xPos() + pong_ball->width() > player_2->xPos()) &&
      (pong_ball->xPos() < player_2->xPos() + player_2->width()) &&
      (pong_ball->yPos() < player_2->yPos() + player_2->height()) &&
      (pong_ball->yPos() + pong_ball->height() > player_2->yPos()))
  {
    // if ball hits palayer 2 then invert x velocity and increase speed by 10%
    pong_ball_x_velocity *= -1;

    if (pong_ball_x_velocity < 863.068 && pong_ball_x_velocity > -863.068)
    {
      pong_ball_x_velocity *= 1.1;
    }
    ASGE::DebugPrinter{} << pong_ball_x_velocity << std::endl;
    // ASGE::DebugPrinter{} << "BOUNCE PLAYER 2" << std::endl;
  }
  else
  {
    return false;
  }
}