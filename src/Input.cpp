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
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios orig_term;

void input_init() {
  struct termios newt;
  tcgetattr(STDIN_FILENO, &orig_term);
  newt = orig_term;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  // set non-blocking
  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void input_restore() {
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

int input_poll() {
  unsigned char c;
  ssize_t r = read(STDIN_FILENO, &c, 1);
  if (r <= 0) return -1;
  return (int)c;
}

#endif
