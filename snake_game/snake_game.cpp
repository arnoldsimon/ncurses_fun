#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <curses.h>

#define GAME_WIDTH 60
#define GAME_HEIGHT 30

#define SCORE_WIDTH 60
#define SCORE_HEIGHT 3

int main()
{
	//..init curses
	initscr(); noecho(); cbreak(); 
	curs_set(0);
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE); //..terminal screen size

	int scr_h, scr_w;

	//..all windows
	WINDOW *game_context, *score_board;

	//..snake attributes
	char s_food = '*';
	char s_head = '@';
	char s_body = '#';
	
	//snake and board 
	int snake_dir;
	std::vector<std::vector<int>> snake; //the rep as a vector
	bool board[GAME_HEIGHT][GAME_WIDTH] = {0};

	//..misc
	int move, score;
	int food_x, food_y;
	bool food_exist;

	//..terminate if terminal too small
	getmaxyx(stdscr, scr_h, scr_w); if (scr_h < (GAME_HEIGHT + 2) + SCORE_HEIGHT || scr_w < GAME_WIDTH + 2)
	{
		mvwprintw(stdscr, scr_h/2, scr_w/2, "Error: Terminal window too small");
		getch();
		endwin();
		return -1;
	}

	//..creating the game context window
	game_context = newwin(GAME_HEIGHT + 2, GAME_WIDTH + 2, (scr_h/2) - (GAME_HEIGHT/2), (scr_w/2) - (GAME_WIDTH/2));
	score_board = newwin(SCORE_HEIGHT, SCORE_WIDTH + 2, (scr_h/2) - (GAME_HEIGHT/2) - SCORE_HEIGHT, (scr_w/2) - (SCORE_WIDTH/2)); //..+2 to account for width in game_context 
	refresh();

	//..init vars
	food_exist = false;
	snake_dir = KEY_UP;
	snake.push_back(std::vector<int>{GAME_HEIGHT/2, GAME_WIDTH/2}); //snake begin at center of screen
	board[snake[0][0]][snake[0][1]] = true;							//marking snake head in board
	//..main game loop
	while (1)
	{
		//..RENDERING THE SNAKE AND FOOD
		//..loop through snake vector and print elements at appro location in window
		werase(game_context);
		for (unsigned int i = 0; i < snake.size(); ++i)
		{
			if (i == 0)
				mvwprintw(game_context, snake[i][0] + 1, snake[i][1] + 1, "%c", s_head);
			else
				mvwprintw(game_context, snake[i][0] + 1, snake[i][1] + 1, "%c", s_body);
		}
		if (food_exist)
			mvwprintw(game_context, food_x + 1, food_y + 1, "%c", s_food);
		box(game_context, 0, 0);
		wrefresh(game_context);

		//..RENDERING THE SCORE
		score = snake.size();
		werase(score_board);
		box(score_board, 0, 0);
		mvwprintw(score_board, 1, 1, " SCORE: %d", score);
		wrefresh(score_board);


		//..READ INPUT AND MOVE SNAKE ACCORDINGLY
		move = getch();
		if (move == 'q')
		{
			break;
		}
		else if (move != ERR || move != snake_dir)
		{
			if (snake_dir == KEY_UP || snake_dir == KEY_DOWN)
			{
				if (move == KEY_LEFT || move == KEY_RIGHT)
					snake_dir = move;
			}
			else if (snake_dir == KEY_LEFT || snake_dir == KEY_RIGHT)
			{
				if (move == KEY_UP || move == KEY_DOWN)
					snake_dir = move;
			}
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

		//..snake doesn't grow if head not in food
		if (snake[0][0] != food_x || snake[0][1] != food_y)
		{
			board[snake.back()[0]][snake.back()[1]] = false;
			snake.pop_back();
		}
		else
			food_exist = false;

		if (food_exist == false)
		{
			while (1)
			{
				bool is_colliding = false;

				food_x = rand() % GAME_HEIGHT;
				food_y = rand() % GAME_WIDTH;

				for (unsigned int i = 0; i < snake.size(); ++i)
				{
					if (food_x == snake[i][0] && food_y == snake[i][1])
					{
						is_colliding = true;
						break;
					}
				}

				if (is_colliding == false)
					break;
			}
			food_exist = true;
		}
		
		std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(100)); 
	}

	//..close curses
	endwin();
	return 0;
}
