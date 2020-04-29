#include <deepstate/DeepState.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ITERS 28

using namespace deepstate;

#define H 7
#define W 12
char maze[H][W] = {"+-+---+---+", "| |     |#|", "| | --+ | |", "| |   | | |",
                   "| +-- | | |", "|     |   |", "+-----+---+"};



TEST(FuzzingWins, JSONTest) {

  int x, y;   // Player position
  int ox, oy; // Old player position
  int i = 0;  // Iteration number

  // Initial position
  x = 1;
  y = 1;
  maze[y][x] = 'X';

  // Print some info
  // Read the directions 'program' to execute...
  char *program = DeepState_CStr(ITERS, "wsad");

  // Iterate and run 'program'
  while (i < ITERS) {
    // Save old player position
    ox = x;
    oy = y;
    // Move polayer position depending on the actual command
    switch (program[i]) {
    case 'w':
      y--;
      break;
    case 's':
      y++;
      break;
    case 'a':
      x--;
      break;
    case 'd':
      x++;
      break;
    default:
      return;
    }

    // If hit the price, You Win!!
    if (maze[y][x] == '#') {
      printf("You win!\n");
      printf("Your solution <%42s>\n", program);
      ASSERT(1 != 1); // stupid thing to make deepstate go boom
    }
    // If something is wrong do not advance
    if (maze[y][x] != ' ' &&
        !((y == 2 && maze[y][x] == '|' && x > 0 && x < W))) {
      x = ox;
      y = oy;
    }
    // If crashed to a wall! Exit, you loose
    if (ox == x && oy == y) {
      return;
    }
    // put the player on the maze...
    maze[y][x] = 'X';
    // draw it
    // increment iteration
    i++;
  }
}

