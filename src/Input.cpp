#include "Input.h"
#include <cstdio>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>

void input_init() {
  // enable VT processing is done in main
}

void input_restore() {}

int input_poll() {
  if (_kbhit()) {
    return _getch();
  }
  return -1;
}

#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_term;

void input_init() {
  struct termios newt;
  tcgetattr(STDIN_FILENO, &orig_term);
  newt = orig_term;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void input_restore() { tcsetattr(STDIN_FILENO, TCSANOW, &orig_term); }

int input_poll() {
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
    if (FD_ISSET(STDIN_FILENO, &fds)) {
      unsigned char c;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        return (int)c;
      }
    }
  }
  return -1;
}

#endif
