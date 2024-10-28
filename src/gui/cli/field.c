//
// Created by KWAZAR_ on 13.05.2024.
//

#include "field.h"

void render_border(WINDOW *win) { box(win, 0, 0); }

void render(GameInfo_t gameInfo) {
  Windows_t *windows = set_windows();

  werase(windows->game_win);
  werase(windows->info_win);

  render_border(windows->game_win);
  render_border(windows->info_win);

  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      mvwaddch(windows->game_win, y + 1, x + 1,
               gameInfo.field[y][x] ? ACS_BLOCK : ' ');
    }
  }

  int info_panel_start_y = 1;
  mvwprintw(windows->info_win, info_panel_start_y++, 2, "Score: %d",
            gameInfo.score);
  mvwprintw(windows->info_win, info_panel_start_y += 2, 2, "High Score: %d",
            gameInfo.high_score);
  mvwprintw(windows->info_win, info_panel_start_y += 2, 2, "Level: %d",
            gameInfo.level);

  Tetromino *nextTetromino = getNextTetromino();
  mvwprintw(windows->info_win, info_panel_start_y += 2, 2, "Next:");
  int offset_y = info_panel_start_y;

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      mvwaddch(windows->info_win, offset_y + y, 9 + x,
               nextTetromino->shape[y][x] ? ACS_BLOCK : ' ');
    }
  }

  wrefresh(windows->game_win);
  wrefresh(windows->info_win);
}