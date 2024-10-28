//
// Created by KWAZAR_ on 27.06.2024.
//

#include <check.h>
#include <stdlib.h>

#include "../tetris.h"

START_TEST(test_init_game) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      ck_assert_int_eq(gameInfo->field[i][j], 0);
    }
  }

  ck_assert_int_eq(gameInfo->score, 0);
  ck_assert_int_eq(gameInfo->level, 1);
  ck_assert_int_eq(gameInfo->speed, 1);
  ck_assert_int_eq(gameInfo->pause, 0);
  ck_assert_int_eq(gameInfo->game_over, 0);
}
END_TEST

START_TEST(test_generate_tetromino) {
  Tetromino tetromino;
  generate_tetromino(&tetromino);

  ck_assert_int_eq(tetromino.x, FIELD_WIDTH / 2 - 2);
  ck_assert_int_eq(tetromino.y, 0);

  int filled = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetromino.shape[i][j]) {
        filled = 1;
        break;
      }
    }
  }
  ck_assert_int_eq(filled, 1);
}
END_TEST

START_TEST(test_generate_tetromino_2) {
  Tetromino tetromino;

  for (int block_type = 1; block_type <= 7; ++block_type) {
    srand(block_type);
    generate_tetromino(&tetromino);

    ck_assert_int_eq(tetromino.x, FIELD_WIDTH / 2 - 2);
    ck_assert_int_eq(tetromino.y, 0);

    int filled = 0;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (tetromino.shape[i][j]) {
          filled = 1;
          break;
        }
      }
    }
    ck_assert_int_eq(filled, 1);
  }
}
END_TEST

START_TEST(test_updateCurrentStateWithoutFall) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  fail_unless(gameInfo->level == 1);
  fail_unless(gameInfo->speed == 1);

  gameInfo->score = 600;
  updateCurrentStateWithoutFall();
  fail_unless(gameInfo->level == 2);
  fail_unless(gameInfo->speed == 2);

  gameInfo->pause = true;
  gameInfo->game_over = true;
  updateCurrentStateWithoutFall();
}
END_TEST

START_TEST(test_clear_lines) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    gameInfo->field[0][j] = 1;
  }

  clear_lines();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    ck_assert_int_eq(gameInfo->field[0][j], 0);
  }

  ck_assert_int_eq(gameInfo->score, 100);
}
END_TEST

START_TEST(test_clear_lines_2) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    gameInfo->field[0][j] = 1;
    gameInfo->field[1][j] = 1;
  }

  clear_lines();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    ck_assert_int_eq(gameInfo->field[0][j], 0);
  }

  ck_assert_int_eq(gameInfo->score, 300);
}
END_TEST

START_TEST(test_clear_lines_3) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    gameInfo->field[0][j] = 1;
    gameInfo->field[1][j] = 1;
    gameInfo->field[2][j] = 1;
  }

  clear_lines();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    ck_assert_int_eq(gameInfo->field[0][j], 0);
  }

  ck_assert_int_eq(gameInfo->score, 700);
}
END_TEST

START_TEST(test_clear_lines_4) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    gameInfo->field[0][j] = 1;
    gameInfo->field[1][j] = 1;
    gameInfo->field[2][j] = 1;
    gameInfo->field[3][j] = 1;
  }

  clear_lines();

  for (int j = 0; j < FIELD_WIDTH; ++j) {
    ck_assert_int_eq(gameInfo->field[0][j], 0);
  }

  ck_assert_int_eq(gameInfo->score, 1500);
}
END_TEST

START_TEST(test_user_input) {
  init_game();
  Tetromino *tetromino = getCurrentTetromino();

  tetromino->x = 1;
  userInput(Left, false);
  ck_assert_int_eq(tetromino->x, 0);

  tetromino->x = 0;
  userInput(Right, false);
  ck_assert_int_eq(tetromino->x, 1);

  int original_shape[4][4];
  memcpy(original_shape, tetromino->shape, sizeof(original_shape));
  userInput(Action, false);
  int rotated = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetromino->shape[i][j] != original_shape[i][j]) {
        rotated = 1;
        break;
      }
    }
  }
  ck_assert_int_eq(rotated, 1);
}
END_TEST

START_TEST(test_update_current_state) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();

  gameInfo->score = 500;
  gameInfo->level = 5;
  gameInfo->speed = 2;
  gameInfo->pause = 1;
  gameInfo->game_over = 1;

  updateCurrentState();

  ck_assert_int_eq(gameInfo->score, 500);
  ck_assert_int_eq(gameInfo->level, 5);
  ck_assert_int_eq(gameInfo->speed, 2);
  ck_assert_int_eq(gameInfo->pause, 1);
  ck_assert_int_eq(gameInfo->game_over, 1);
}
END_TEST

START_TEST(test_merge_tetromino) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino tetromino;

  generate_tetromino(&tetromino);

  merge_tetromino(&tetromino);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (tetromino.shape[i][j]) {
        ck_assert_int_eq(gameInfo->field[tetromino.y + i][tetromino.x + j], 1);
      }
    }
  }
}
END_TEST

START_TEST(test_save_high_score) {
  int test_high_score = 1000;

  saveHighScore(test_high_score);

  int saved_high_score = loadHighScore();
  ck_assert_int_eq(saved_high_score, test_high_score);
}
END_TEST

START_TEST(test_game_logic_update) {
  init_game();
  GameInfo_t *gameInfo = getCurrentGameInfo();
  Tetromino tetromino;

  gameInfo->score = 0;
  gameInfo->level = 1;
  gameInfo->speed = 1;
  gameInfo->pause = 0;
  gameInfo->game_over = 0;

  gameInfo->score = gameInfo->level * 600;
  updateCurrentState();

  ck_assert_int_eq(gameInfo->level, 2);
  ck_assert_int_eq(gameInfo->speed, 2);

  ck_assert_int_eq(gameInfo->pause, 0);
  ck_assert_int_eq(gameInfo->game_over, 0);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_game);
  tcase_add_test(tc_core, test_generate_tetromino);
  tcase_add_test(tc_core, test_generate_tetromino_2);
  tcase_add_test(tc_core, test_clear_lines);
  tcase_add_test(tc_core, test_clear_lines_2);
  tcase_add_test(tc_core, test_clear_lines_3);
  tcase_add_test(tc_core, test_clear_lines_4);
  tcase_add_test(tc_core, test_user_input);
  tcase_add_test(tc_core, test_update_current_state);
  tcase_add_test(tc_core, test_save_high_score);
  tcase_add_test(tc_core, test_merge_tetromino);
  tcase_add_test(tc_core, test_game_logic_update);
  tcase_add_test(tc_core, test_updateCurrentStateWithoutFall);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = tetris_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
