#include <curses.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#define CTRL_KEY(k) ((k) & 0x1f)
struct termios origtermios;
void die(const char *s);
void DisiableRawMode(){
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origtermios) == -1)
      die("tcsetattr");
}

void EnableRawMode(){
  if (tcgetattr(STDIN_FILENO, &origtermios) == -1) die("tcgetattr");
  atexit(DisiableRawMode);
  struct termios raw = origtermios;

  tcgetattr(STDIN_FILENO,&raw);
  raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_lflag &= ~(OPOST);
  raw.c_lflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1) die("tcgetattr");

}


char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}
/*** input ***/
void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
      break;
  }
}
/*** init ***/
int main() {
  EnableRawMode();
  while (1) {
    editorProcessKeypress();
  }
  return 0;
}
