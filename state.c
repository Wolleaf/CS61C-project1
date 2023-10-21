#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t *game_state = (game_state_t*) malloc(sizeof(game_state_t));
  snake_t *snake = (snake_t*) malloc(sizeof(snake_t));
  snake->head_x = 5;
  snake->head_y = 4;
  snake->tail_x = 4;
  snake->tail_y = 4;
  snake->live = true;
  game_state->x_size = 14;
  game_state->y_size = 10;
  game_state->num_snakes = 1;
  game_state->snakes = snake;
  game_state->board = (char**)malloc(10 * sizeof(char*));
  for (int i = 0; i < 10; ++i) {
    game_state->board[i] = (char*)malloc(14 * sizeof(char));
  }
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 14; ++j) {
      if (i == 0 || i == 9 || j == 0 || j == 13) {
        game_state->board[i][j] = '#';
      }
      else {
        game_state->board[i][j] = ' ';
      }
    }
  }
  game_state->board[2][9] = '*';
  game_state->board[4][4] = 'd';
  game_state->board[4][5] = '>';
  return game_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  free(state->snakes);
  for (int i = 0; i < state->y_size; ++i) {
    free(state->board[i]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for (int i = 0; i < state->y_size; ++i) {
    fprintf(fp, state->board[i]);
    fprintf(fp, "\n");
  }
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  bool temp = false;
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    temp = true;
  }
  return temp; 
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  bool temp = false;
  if (c == 'w' || c == 'a' || c == 's' || c == 'd' || c == '^' || c == '<' || c == '>'|| c == 'v' || c == 'x') {
    temp = true;
  }
  return temp; 
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  char temp = '?';
  if (c == '^') {
    temp = 'w';
  }
  else if (c == '<') {
    temp = 'a';
  }
  else if (c == '>') {
    temp = 'd';
  }
  else if (c == 'v') {
    temp = 's';
  }
  return temp;
}

static int incr_x(char c) {
  // TODO: Implement this function.
  int temp = 0;
  if (c == 'd' || c == '>') {
    temp = 1;
  }
  else if (c == 'a' || c == '<') {
    temp = -1;
  }
  return temp;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  int temp = 0;
  if (c == 's' || c == 'v') {
    temp = 1;
  }
  else if (c == 'w' || c == '^') {
    temp = -1;
  }
  return temp;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].head_x;
  int y = state->snakes[snum].head_y;
  char head = get_board_at(state, x, y);
  if (head == '>' || head == '<') {
    x += incr_x(head);
  }
  else if (head == '^' || head == 'v') {
    y += incr_y(head);
  }
  return get_board_at(state, x, y);
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].head_x;
  int y = state->snakes[snum].head_y;
  char head = get_board_at(state, x, y);
  if (head == '>' || head == '<') {
    x += incr_x(head);
  }
  else if (head == '^' || head == 'v') {
    y += incr_y(head);
  }
  set_board_at(state, x, y, head);
  state->snakes[snum].head_x = x;
  state->snakes[snum].head_y = y;
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int x = state->snakes[snum].tail_x;
  int y = state->snakes[snum].tail_y;
  char tail = get_board_at(state, x, y);
  set_board_at(state, x, y, ' ');
  if (tail == 'd' || tail == 'a') {
    x += incr_x(tail);
  }
  else if (tail == 'w' || tail == 's') {
    y += incr_y(tail);
  }
  char body = get_board_at(state, x, y);
  set_board_at(state, x, y, body_to_tail(body));
  state->snakes[snum].tail_x = x;
  state->snakes[snum].tail_y = y;
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; ++i) {
    char next_place = next_square(state, i);
    if (next_place == '#' || is_snake(next_place)) {
      int head_x = state->snakes[i].head_x;
      int head_y = state->snakes[i].head_y;
      set_board_at(state, head_x, head_y, 'x');
      state->snakes[i].live = false;
    }
    else if (next_place == '*') {
      update_head(state, i);
      add_food(state);
    }
    else if (next_place == ' ') {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE *file = fopen(filename, "r");
  char str[256];
  int rows = 0;
  int cols = 0;
  while (!feof(file)) {
    rows += 1;
    fgets(str, 256, file);
  }
  while (str[cols]) {
    cols += 1;
  }
  cols -= 1;
  rows -= 1;

  game_state_t* game_state = (game_state_t*)malloc(sizeof(game_state_t));
  game_state->board = (char**)malloc(rows * sizeof(char*));
  for (int i = 0; i < rows; ++i) {
    game_state->board[i] = (char*)malloc(cols * sizeof(char));
  }
  game_state->y_size = rows;
  game_state->x_size = cols;

  rewind(file);
  char temp;
  for (int i = 0; i < rows; ++i) {
    int j = 0;
    while ((temp = fgetc(file)) != '\n') {
      game_state->board[i][j] = temp;
      ++j;
    }
  }
  return game_state;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int tail_x = state->snakes[snum].tail_x;
  int tail_y = state->snakes[snum].tail_y;
  int head_x = 0;
  int head_y = 0;
  char forward = get_board_at(state, tail_x, tail_y);
  while (is_snake(forward)) {
    head_x = tail_x;
    head_y = tail_y;
    tail_x += incr_x(forward);
    tail_y += incr_y(forward);
    forward = get_board_at(state, tail_x, tail_y);
  }
  state->snakes[snum].head_x = head_x;
  state->snakes[snum].head_y = head_y;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int num = 0;
  int tail_x[256];
  int tail_y[256];
  int rows = state->y_size;
  int cols = state->x_size;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (state->board[i][j] == 'w' || state->board[i][j] == 'a' || state->board[i][j] == 's' || state->board[i][j] == 'd') {
        tail_x[num] = j;
        tail_y[num] = i;
        num += 1;
      }
    }
  }
  state->snakes = (snake_t*)malloc(sizeof(snake_t) * num);
  state->num_snakes = num;
  for (int i = 0; i < num; ++i) {
    state->snakes[i].tail_x = tail_x[i];
    state->snakes[i].tail_y = tail_y[i];
    state->snakes[i].live = true;
    find_head(state, i);
  }
  return state;
}
