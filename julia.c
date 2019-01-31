#include <curses.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

#define SIZE_X		80
#define SIZE_Y		24
#define FPS		75	

static char pixels[] = "  .,:;o*[M#";

void cls(char **buf) {
  int x, y;
  for (y = 0; y < SIZE_Y; y++)
    for (x = 0; x <= SIZE_X; x++)
      buf[y][x] = (x == SIZE_X ? '\0' : ' ');
}

void blit(char **buf) {
  int y;
  for (y = 0; y < SIZE_Y; y++)
    mvaddstr(y, 0, buf[y]);
}

char **bufalloc() {
  int i;
  char **res;
  res = (char **)malloc(SIZE_Y * sizeof(char *));
  for (i = 0; i < SIZE_Y; i++) 
    res[i] = (char *)malloc((SIZE_X + 1) * sizeof(char));
  return res;
}

void buffree(char **buf) {
  int i;
  for (i = 0; i < SIZE_Y; i++)
    free(buf[i]);
  free(buf);
}

void putpixel(char **buf, int x, int y, int c) {
  buf[y][x] = pixels[c];
}

void drawjulia(char **buf, double re, double im) {
  int x, y, c;	
  double xre, xim, tmp;
  for (y = 0; y < SIZE_Y; y++)
    for (x = 0; x < SIZE_X; x++) {
      xre = (4.0 * (double)(x - SIZE_X / 2)) / SIZE_X;
      xim = (4.0 * (double)(SIZE_Y / 2 - y)) / SIZE_Y;
      c = 0;
      while (xre * xre + xim * xim < 25.0) {
	tmp = xre;
	xre = xre * xre - xim * xim + re;
	xim = 2.0 * tmp * xim + im;
        if (++c == 10) break;
      }
      putpixel(buf, x, y, c);
    }
}

void init() {
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
}  

void done() {
  endwin();
}

int kbhit() {
  return (getch() != ERR);
}

void proc(int sig) {}

int main() {
  double t1 = 0.0, t2 = 0.0;
  char **buffer = bufalloc();
  cls(buffer);
  init();
  while(!kbhit()) {
    drawjulia(buffer, cos(t1), sin(t1));
    t1 += 0.06; if (t1 < 6.283) t1 -= 6.283;
    t2 += 0.08; if (t2 < 6.283) t2 -= 6.283;
    blit(buffer);
    usleep(1000000 / FPS);
  }
  done();
  buffree(buffer);  
  return 0;
}
