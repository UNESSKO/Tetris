/**
 * @file tetris.h
 * @brief Header file containing definitions and functions for Tetris game
 * logic.
 */

#ifndef C7_BRICKGAME_V1_0_1_TETRIS_H
#define C7_BRICKGAME_V1_0_1_TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define MAX_LEVEL 10
#define HIGH_SCORE_FILE "highscore.txt"
#define TICK_RATE_BASE 300000

/**
 * @struct Tetromino
 * @brief Structure representing a Tetromino piece in the game.
 */
typedef struct {
  int x, y;         ///< Coordinates of the Tetromino on the game field.
  int shape[4][4];  ///< Shape matrix of the Tetromino.
} Tetromino;

/**
 * @enum UserAction_t
 * @brief Enumeration representing user actions in the game.
 */
typedef enum {
  Start,      ///< Start the game action.
  Pause,      ///< Pause the game action.
  Terminate,  ///< Terminate the game action.
  Left,       ///< Move Tetromino left action.
  Right,      ///< Move Tetromino right action.
  Up,         ///< Rotate Tetromino action.
  Down,       ///< Move Tetromino down action.
  Action      ///< Perform action (typically rotate) with Tetromino.
} UserAction_t;

/**
 * @struct GameInfo_t
 * @brief Structure containing game state information.
 */
typedef struct {
  int **field;     ///< The game field represented as a matrix.
  int **next;      ///< Next Tetromino preview.
  int score;       ///< Current game score.
  int high_score;  ///< Highest score achieved in the game.
  int level;       ///< Current game level.
  int speed;       ///< Falling speed of Tetrominoes.
  int pause;       ///< Flag indicating if the game is paused.
  int game_over;   ///< Flag indicating if the game is over.
} GameInfo_t;

/**
 * @brief Retrieves the next Tetromino for the game.
 *
 * @return Pointer to the next Tetromino structure.
 */
Tetromino *getNextTetromino();

/**
 * @brief Retrieves the current game information.
 *
 * @return Pointer to the GameInfo_t structure containing current game state.
 */
GameInfo_t *getCurrentGameInfo();

/**
 * @brief Retrieves the current active Tetromino in the game.
 *
 * @return Pointer to the current Tetromino structure.
 */
Tetromino *getCurrentTetromino();

/**
 * @brief Initializes the game state and field.
 */
void init_game();

/**
 * @brief Updates the current game state including Tetromino movement and
 * collision detection.
 *
 * @return Updated GameInfo_t structure after updating game state.
 */
GameInfo_t updateCurrentState();

/**
 * @brief Updates the current game state without advancing Tetromino falling.
 *
 * @return Updated GameInfo_t structure after updating game state without
 * falling.
 */
GameInfo_t updateCurrentStateWithoutFall();

/**
 * @brief Processes user input to control Tetromino movement and actions.
 *
 * @param action UserAction_t enumeration representing the action to be
 * performed.
 * @param hold Unused parameter for compatibility.
 */
void userInput(UserAction_t action, __attribute__((unused)) bool hold);

/**
 * @brief Generates a new Tetromino piece.
 *
 * @param tetromino Pointer to the Tetromino structure to generate.
 */
void generate_tetromino(Tetromino *tetromino);

/**
 * @brief Rotates the given Tetromino piece.
 *
 * @param tetromino Pointer to the Tetromino structure to rotate.
 */
void rotate_tetromino(Tetromino *tetromino);

/**
 * @brief Checks if the given Tetromino collides with the game field or other
 * Tetrominoes.
 *
 * @param tetromino Pointer to the Tetromino structure to check collision for.
 * @param offsetX X-axis offset for collision check.
 * @param offsetY Y-axis offset for collision check.
 * @return 1 if collision detected, 0 otherwise.
 */
int check_collision(Tetromino *tetromino, int offsetX, int offsetY);

/**
 * @brief Merges the Tetromino with the game field when it cannot move further
 * down.
 *
 * @param tetromino Pointer to the Tetromino structure to merge.
 */
void merge_tetromino(Tetromino *tetromino);

/**
 * @brief Clears completed lines in the game field.
 */
void clear_lines();

/**
 * @brief Places the Tetromino on the game field.
 *
 * @param tetromino Pointer to the Tetromino structure to place on the field.
 */
void place_tetromino(Tetromino *tetromino);

/**
 * @brief Removes the Tetromino from its current position on the game field.
 *
 * @param tetromino Pointer to the Tetromino structure to remove.
 */
void remove_tetromino(Tetromino *tetromino);

/**
 * @brief Saves the high score to a file.
 *
 * @param high_score Highest score achieved in the game.
 */
void saveHighScore(int high_score);

/**
 * @brief Loads the high score from a file.
 *
 * @return Highest score previously saved.
 */
int loadHighScore();

void cleanup_game();

#endif  // C7_BRICKGAME_V1_0_1_TETRIS_H
