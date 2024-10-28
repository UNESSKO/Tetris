/**
 * @file game.h
 * @brief Header file for managing the game interface and user actions.
 */

#ifndef C7_BRICKGAME_V1_0_1_GAME_H
#define C7_BRICKGAME_V1_0_1_GAME_H

#include <time.h>
#include <unistd.h>

#include "brick_game/tetris/tetris.h"
#include "gui/cli/field.h"

#define TICK_RATE 1000000000L

/**
 * @struct Windows_t
 * @brief Structure for managing game and info windows.
 */
typedef struct {
  WINDOW *game_win; /**< Game window. */
  WINDOW *info_win; /**< Info window. */
} Windows_t;

/**
 * @brief Sets up and initializes the game windows.
 *
 * @return Pointer to the initialized Windows_t structure.
 */
Windows_t *set_windows();

/**
 * @brief Initializes the ncurses library.
 */
void initialize_ncurses();

/**
 * @brief Closes the game by cleaning up ncurses windows.
 *
 * @param windows Pointer to the Windows_t structure containing game and info
 * windows.
 */
void close_game(Windows_t *windows);

/**
 * @brief Gets the user action based on keyboard input.
 *
 * @return UserAction_t The user action determined from keyboard input.
 */
UserAction_t UserAction();

#endif  // C7_BRICKGAME_V1_0_1_GAME_H
