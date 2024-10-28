//
// Created by KWAZAR_ on 13.05.2024.
//

#include "tetris.h"

#include "../../gui/cli/field.h"

void init_game() {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino *currentTetromino = getCurrentTetromino();
  Tetromino *nextTetromino = getNextTetromino();

  srand(time(NULL));
  gameInfo->field = malloc(FIELD_HEIGHT * sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    gameInfo->field[i] = malloc(FIELD_WIDTH * sizeof(int));
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      gameInfo->field[i][j] = 0;
    }
  }

  gameInfo->next = malloc(4 * sizeof(int *));
  for (int i = 0; i < 4; ++i) {
    gameInfo->next[i] = malloc(4 * sizeof(int));
    for (int j = 0; j < 4; ++j) {
      gameInfo->next[i][j] = 0;
    }
  }

  gameInfo->score = 0;
  gameInfo->high_score = loadHighScore();
  gameInfo->level = 1;
  gameInfo->speed = 1;
  gameInfo->pause = 0;
  gameInfo->game_over = 0;

  generate_tetromino(currentTetromino);
  generate_tetromino(nextTetromino);
}

GameInfo_t *getCurrentGameInfo() {
  static GameInfo_t gameInfo;
  return &gameInfo;
}

Tetromino *getCurrentTetromino() {
  static Tetromino currentTetromino;
  return &currentTetromino;
}

Tetromino *getNextTetromino() {
  static Tetromino nextTetromino;
  return &nextTetromino;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino *currentTetromino = getCurrentTetromino();
  Tetromino *nextTetromino = getNextTetromino();

  if (gameInfo->score >= gameInfo->level * 600 && gameInfo->level < 10) {
    gameInfo->level++;
    gameInfo->speed++;
  }

  if (!gameInfo->pause && !gameInfo->game_over) {
    remove_tetromino(currentTetromino);

    if (!check_collision(currentTetromino, 0, 1)) {
      currentTetromino->y += 1;
    } else {
      merge_tetromino(currentTetromino);
      clear_lines();  // Немедленно очищаем линии
      *currentTetromino = *nextTetromino;
      generate_tetromino(nextTetromino);

      if (check_collision(currentTetromino, 0, 0)) {
        gameInfo->game_over = 1;
        if (gameInfo->score > gameInfo->high_score) {
          gameInfo->high_score = gameInfo->score;
          saveHighScore(gameInfo->high_score);
        }
      }
    }

    place_tetromino(currentTetromino);
  }

  return *gameInfo;
}

GameInfo_t updateCurrentStateWithoutFall() {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino *currentTetromino = getCurrentTetromino();

  if (gameInfo->score >= gameInfo->level * 600 && gameInfo->level < 10) {
    gameInfo->level++;
    gameInfo->speed++;
  }

  if (!gameInfo->pause && !gameInfo->game_over) {
    remove_tetromino(currentTetromino);
    place_tetromino(currentTetromino);
  }

  return *gameInfo;
}

void userInput(UserAction_t action, __attribute__((unused)) bool hold) {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino *currentTetromino = getCurrentTetromino();

  if (action == Pause) gameInfo->pause = !gameInfo->pause;

  if (!gameInfo->pause) {
    remove_tetromino(currentTetromino);

    switch (action) {
      case Down:
        while (!check_collision(currentTetromino, 0, 1)) {
          currentTetromino->y++;
        }
        break;

      case Left:
        if (!check_collision(currentTetromino, -1, 0)) {
          currentTetromino->x--;
        }
        break;

      case Right:
        if (!check_collision(currentTetromino, 1, 0)) {
          currentTetromino->x++;
        }
        break;

      case Action:
        rotate_tetromino(currentTetromino);
        break;

      default:
        break;
    }

    place_tetromino(currentTetromino);
  }
}

void saveHighScore(int high_score) {
  FILE *file = fopen(HIGH_SCORE_FILE, "w");
  if (file) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}

int loadHighScore() {
  FILE *file = fopen(HIGH_SCORE_FILE, "r");
  int high_score = 0;
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }
  return high_score;
}

int check_collision(Tetromino *tetromino, int dx, int dy) {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  int flag = 0;

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetromino->shape[y][x]) {
        int new_x = tetromino->x + x + dx;
        int new_y = tetromino->y + y + dy;

        if (new_x < 0 || new_x >= FIELD_WIDTH || new_y < 0 ||
            new_y >= FIELD_HEIGHT) {
          flag = 1;
          break;
        }

        if (gameInfo->field[new_y][new_x]) {
          flag = 1;
          break;
        }
      }
    }
  }
  return flag;
}

void merge_tetromino(Tetromino *tetromino) { place_tetromino(tetromino); }

void rotate_tetromino(Tetromino *tetromino) {
  Tetromino rotated = *tetromino;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      rotated.shape[y][x] = tetromino->shape[3 - x][y];
    }
  }
  if (!check_collision(&rotated, 0, 0)) {
    *tetromino = rotated;
  }
}

void clear_lines() {
  GameInfo_t *gameInfo = getCurrentGameInfo();
  int lines_cleared = 0;

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    bool full_line = true;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (gameInfo->field[y][x] == 0) {
        full_line = false;
        break;
      }
    }
    if (full_line) {
      lines_cleared++;
      for (int yy = y; yy > 0; yy--) {
        for (int xx = 0; xx < FIELD_WIDTH; xx++) {
          gameInfo->field[yy][xx] = gameInfo->field[yy - 1][xx];
        }
      }
      for (int xx = 0; xx < FIELD_WIDTH; xx++) {
        gameInfo->field[0][xx] = 0;
      }
    }
  }

  switch (lines_cleared) {
    case 1:
      gameInfo->score += 100;
      break;
    case 2:
      gameInfo->score += 300;
      break;
    case 3:
      gameInfo->score += 700;
      break;
    case 4:
      gameInfo->score += 1500;
      break;
  }

  if (gameInfo->score > gameInfo->high_score) {
    gameInfo->high_score = gameInfo->score;
    saveHighScore(gameInfo->high_score);
  }
}

void place_tetromino(Tetromino *tetromino) {
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetromino->shape[y][x] && tetromino->y + y < FIELD_HEIGHT &&
          tetromino->x + x < FIELD_WIDTH) {
        gameInfo->field[tetromino->y + y][tetromino->x + x] = 1;
      }
    }
  }
  clear_lines();
}

void remove_tetromino(Tetromino *tetromino) {
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (tetromino->shape[y][x] && tetromino->y + y < FIELD_HEIGHT &&
          tetromino->x + x < FIELD_WIDTH) {
        gameInfo->field[tetromino->y + y][tetromino->x + x] = 0;
      }
    }
  }
}

void generate_tetromino(Tetromino *tetromino) {
  int block_type = (int)(rand() % 7) + 1;
  memset(tetromino->shape, 0, sizeof(tetromino->shape));
  tetromino->x = FIELD_WIDTH / 2 - 2;
  tetromino->y = 0;
  switch (block_type) {
    case 1:  // I
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      tetromino->shape[1][2] = 1;
      tetromino->shape[1][3] = 1;
      break;
    case 2:  // J
      tetromino->shape[0][0] = 1;
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      tetromino->shape[1][2] = 1;
      break;
    case 3:  // L
      tetromino->shape[0][2] = 1;
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      tetromino->shape[1][2] = 1;
      break;
    case 4:  // O
      tetromino->shape[0][0] = 1;
      tetromino->shape[0][1] = 1;
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      break;
    case 5:  // S
      tetromino->shape[0][1] = 1;
      tetromino->shape[0][2] = 1;
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      break;
    case 6:  // T
      tetromino->shape[0][1] = 1;
      tetromino->shape[1][0] = 1;
      tetromino->shape[1][1] = 1;
      tetromino->shape[1][2] = 1;
      break;
    case 7:  // Z
      tetromino->shape[0][0] = 1;
      tetromino->shape[0][1] = 1;
      tetromino->shape[1][1] = 1;
      tetromino->shape[1][2] = 1;
      break;
  }
}

void cleanup_game() {
  GameInfo_t *gameInfo = getCurrentGameInfo();

  if (gameInfo->field != NULL) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
      if (gameInfo->field[i] != NULL) {
        free(gameInfo->field[i]);
      }
    }
    free(gameInfo->field);
    gameInfo->field = NULL;
  }

  if (gameInfo->next != NULL) {
    for (int i = 0; i < 4; ++i) {
      if (gameInfo->next[i] != NULL) {
        free(gameInfo->next[i]);
      }
    }
    free(gameInfo->next);
    gameInfo->next = NULL;
  }
}