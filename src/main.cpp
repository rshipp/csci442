/**
 * Contains the main() routine of what will eventually be your version of top.
 */

#include <cstdlib>
#include <ncurses.h>

using namespace std;


/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}


/**
 * Entry point for the program.
 */
int main() {
  // ncurses initialization
  initscr();

  // Don't show a cursor.
  curs_set(FALSE);

  // Set getch to return after 1000 milliseconds; this allows the program to
  // immediately respond to user input while not blocking indefinitely.
  timeout(1000);

  int tick = 1;

  while (true) {
    wclear(stdscr);

    // Display the counter using printw (an ncurses function)
    printw("Behold, the number:\n%d", tick++);

    // Redraw the screen.
    refresh();

    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}
