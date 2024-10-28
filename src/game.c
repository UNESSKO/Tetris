//
// Created by KWAZAR_ on 13.05.2024.
//
#define _POSIX_C_SOURCE 199309L
#include "game.h"

Windows_t *set_windows() {
  static Windows_t winInfo;
  return &winInfo;
}

void initialize_ncurses() {
  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);
}

void close_game(Windows_t *windows) {
  delwin(windows->game_win);
  delwin(windows->info_win);
  endwin();
}

UserAction_t UserAction() {
  UserAction_t action = Up;
  int ch = getch();
  if (ch == 'q') {
    action = Terminate;
  }
  switch (ch) {
    case '\n':
      action = Start;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case ' ':
      action = Action;
      break;
    case 'p':
      action = Pause;
      break;
    default:
      break;
  }
  userInput(action, false);
  return action;
}

int main() {
  initialize_ncurses();
  Windows_t *windows = set_windows();
  windows->game_win = newwin(FIELD_HEIGHT + 2, FIELD_WIDTH + 2, 0, 0);
  windows->info_win = newwin(FIELD_HEIGHT + 2, 20, 0, FIELD_WIDTH + 5);
  init_game();
  render(updateCurrentState());
  struct timespec lastFallTime;
  clock_gettime(1, &lastFallTime);
  while (1) {
    UserAction_t action = UserAction();
    if (action == Terminate) break;
    if (getCurrentGameInfo()->game_over) {
      mvprintw(FIELD_HEIGHT / 2, FIELD_WIDTH / 2 - 3, "GAME OVER");
      refresh();
      while (1) {
        action = UserAction();
        if (action == Start) {
          cleanup_game();
          init_game();
          render(updateCurrentState());
          break;
        } else if (action == Terminate) {
          cleanup_game();
          close_game(windows);
          break;
        }
      }
      if (action == Terminate) break;
      clock_gettime(1, &lastFallTime);
    }
    struct timespec currentTime;
    clock_gettime(1, &currentTime);
    long elapsedNanoSec =
        (currentTime.tv_sec - lastFallTime.tv_sec) * TICK_RATE +
        (currentTime.tv_nsec - lastFallTime.tv_nsec);
    int fall_delay = TICK_RATE / getCurrentGameInfo()->speed;
    if (elapsedNanoSec >= fall_delay) {
      lastFallTime = currentTime;
      updateCurrentState();
    } else
      updateCurrentStateWithoutFall();

    render(*getCurrentGameInfo());
  }
  cleanup_game();
  close_game(windows);
  return 0;
}