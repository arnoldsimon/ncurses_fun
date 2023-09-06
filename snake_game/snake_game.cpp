#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <curses.h>

#define GAME_WIDTH 60
#define GAME_HEIGHT 30

int main()
{
	//..init curses
	initscr(); noecho(); cbreak(); 
	curs_set(0);
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE); //..terminal screen size

	int scr_h, scr_w;

	//..all windows
	WINDOW *game_context;

	//..snake attributes
	//char s_food = '*';
	char s_head = '@';
	char s_body = '#';
	
	//snake and board
	bool board[GAME_HEIGHT][GAME_WIDTH] = {0};
	std::vector<std::vector<int>> snake; //the rep as a vector
	int snake_dir;

	//..misc
	int count, move;

	//..terminate if terminal too small
	getmaxyx(stdscr, scr_h, scr_w);
	if (scr_h < GAME_HEIGHT + 2 || scr_w < GAME_WIDTH + 2)
	{
		mvwprintw(stdscr, scr_h/2, scr_w/2, "Error: Terminal window too small");
		getch();
		endwin();
		return -1;
	}

	//..creating the game context window
	game_context = newwin(GAME_HEIGHT + 2, GAME_WIDTH + 2, (scr_h/2) - (GAME_HEIGHT/2), (scr_w/2) - (GAME_WIDTH/2));
	refresh();

	//..init vars
	count = 0;
	snake_dir = KEY_UP;
	snake.push_back(std::vector<int>{GAME_HEIGHT/2, GAME_WIDTH/2}); //snake begin at center of screen
	board[snake[0][0]][snake[0][1]] = true;							//marking snake head in board
	//..main game loop
	while (1)
	{
		//..RENDERING THE SNAKE
		//..loop through snake vector and print elements at appro location in window
		werase(game_context);
		for (unsigned int i = 0; i < snake.size(); ++i)
		{
			if (i == 0)
				mvwprintw(game_context, snake[i][0] + 1, snake[i][1] + 1, "%c", s_head);
			else
				mvwprintw(game_context, snake[i][0] + 1, snake[i][1] + 1, "%c", s_body);
		}
		box(game_context, 0, 0);
		wrefresh(game_context);


		//..READ INPUT AND MOVE SNAKE ACCORDINGLY
		move = getch();
		if (move == 'q')
		{
			break;
		}
		else if (move != ERR || move != snake_dir)
		{
			if (move == KEY_UP || move == KEY_DOWN || move == KEY_LEFT || move == KEY_RIGHT)
				snake_dir = move;
		}

		//..check collision and move head acc to inp
		if (snake_dir == KEY_UP)
		{
			if (snake[0][0] == 0 || board[snake[0][0] - 1][snake[0][1]] == true)
				break;

			board[snake[0][0] - 1][snake[0][1]] = true;
			snake.insert(snake.begin(), std::vector<int>{snake[0][0] - 1, snake[0][1]});
		}
		else if (snake_dir == KEY_DOWN)
		{
			if (snake[0][0] == GAME_HEIGHT-1 || board[snake[0][0] + 1][snake[0][1]] == true)
				break;

			board[snake[0][0] + 1][snake[0][1]] = true;
			snake.insert(snake.begin(), std::vector<int>{snake[0][0] + 1, snake[0][1]});
		}
		else if (snake_dir == KEY_LEFT)
		{
			if (snake[0][1] == 0 || board[snake[0][0]][snake[0][1] - 1] == true)
				break;

			board[snake[0][0]][snake[0][1] - 1] = true;
			snake.insert(snake.begin(), std::vector<int>{snake[0][0], snake[0][1] - 1});
		}
		else if (snake_dir == KEY_RIGHT)
		{
			if (snake[0][1] == GAME_WIDTH-1 || board[snake[0][0]][snake[0][1] + 1] == true)
				break;

			board[snake[0][0]][snake[0][1] + 1] = true;
			snake.insert(snake.begin(), std::vector<int>{snake[0][0], snake[0][1] + 1});
		}

		/*
		//..snake grow (+1) after every 5 moves
		++count;
		if (count % 5 == 0)
			count = 0;
		else
		{
			board[snake.back()[0]][snake.back()[1]] = false;
			snake.pop_back();
		}
		*/
		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(500)); 
		board[snake.back()[0]][snake.back()[1]] = false;
		snake.pop_back();
	}

	//..close curses
	endwin();
	return 0;
}
