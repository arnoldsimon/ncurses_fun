#include <curses.h>

int main()
{
	initscr();

	int h, w;
	getyx(stdscr, h, w);

	mvprintw(h/2, w/2, "Hello World!");
	getch();

	endwin();
	return 0;
}
