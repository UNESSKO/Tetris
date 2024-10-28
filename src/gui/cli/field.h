/**
 * @file field.h
 * @brief Header file for rendering game field and borders using ncurses.
 */

#ifndef C7_BRICKGAME_V1_0_1_FIELD_H
#define C7_BRICKGAME_V1_0_1_FIELD_H

#include <limits.h>
#include <ncurses.h>
#include <string.h>

#include "../../brick_game/tetris/tetris.h"
#include "../../game.h"

/**
 * @brief Renders the border of a window using ncurses.
 *
 * @param win The ncurses window where the border will be rendered.
 */
void render_border(WINDOW *win);

/**
 * @brief Renders the game interface using ncurses.
 *
 * @param gameInfo Game information structure containing data to render.
 */
void render(GameInfo_t gameInfo);

#endif  // C7_BRICKGAME_V1_0_1_FIELD_H
