#include <curses.h>
#include <unistd.h>
#include <random>

using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int randomx(int min, int max){
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(min,max);
	return dist(mt);
}

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	WINDOW *win;
	WINDOW *ins;
	ins = newwin(1,80,0,0);
	win = create_newwin(25,80,1,0);
	
	curs_set(0);
	keypad(win,TRUE);
	
	
	nodelay(win,TRUE);
	char player = '|';
	char enemy = '|';
	char ball = 'o';
	int buffer = 10;
	int score[2];
	int px, py[3], ex, ey[3], pyDirection, eyDirection;
	int bx, by;
	int bxDirection, byDirection;
	int press;
	int randomby;
	bool loop = TRUE;

	wbkgd(win,COLOR_PAIR(1) | ' ');
	wbkgd(ins,COLOR_PAIR(1) | ' ');
	box(win,'|','=');
	
	px = 2;
	py[0] = getmaxy(win)/2;
	py[1] = py[0] + 1;
	py[2] = py[0] - 1;
	ex = getmaxx(win) - 3;
	ey[0] = getmaxy(win)/2;
	ey[1] = ey[0] + 1;
	ey[2] = ey[0] - 1;
	eyDirection = -1;
	bx = getmaxx(win)/2;
	by = getmaxy(win)/2;
	bxDirection = 1;
	byDirection = 1;
	score[0] = 0;
	score[1] = 0;

	//Game Loop
	while (loop){
		
		//Controls for the player paddle
		press = wgetch(win);
		switch (press){
		case KEY_UP:
			if(py[2] == 1){
				break;
			}else{
				pyDirection = 1;
				py[0] += -2;
				py[1] = py[0] + 1;
				py[2] = py[0] - 1;
				break;
			}

		case KEY_DOWN:
			if(py[1] == getmaxy(win)-2){
				break;
			}else{
				pyDirection = -1;
				py[0] += 2;
				py[1] = py[0] + 1;
				py[2] = py[0] - 1;
				break;
			}
		
		case KEY_F(1):
			loop = FALSE;
			break;
		}
		
		//Enemy paddle movement
		buffer--;
		if(buffer == 0){
			buffer = randomx(1,3);
	
			switch (eyDirection)
			{
			case -1:
				if(ey[2] == 1){
					break;
				}else{
					ey[0] += eyDirection;
					ey[1] = ey[0] + 1;
					ey[2] = ey[0] - 1;
					break;
				}
			
			case 1:
				if(ey[1] == getmaxy(win)-2){
					break;
				}else{
					ey[0] += eyDirection;
					ey[1] = ey[0] + 1;
					ey[2] = ey[0] - 1;
					break;
				}
			case 0:
				if (ey[0] > by){
					ey[0] += -1;
					ey[1] = ey[0] + 1;
					ey[2] = ey[0] - 1;
					break;
				}else if(ey[0] < by){
					ey[0] += 1;
					ey[1] = ey[0] + 1;
					ey[2] = ey[0] - 1;
					break;
				}else{
					break;
				}
			}
		}
		

		//Ball Movement
		randomby = randomx(1,3);
		int byDirectionRandom = randomby; 
		if (randomby == 2){
				byDirectionRandom = -1;
		}
		bx += bxDirection;
		by += byDirection;

		//Checks if ball hits pass player paddle and gives score and resets ball position.

		if(bx == getmaxx(win) - 2){
			bxDirection = -1;
			byDirection = 0;
			score[0]++;
			bx = getmaxx(win)/2;
			by = getmaxy(win)/2;
		}else if (bx == 1){
			bxDirection = 1;
			byDirection = 0;
			score[1]++;
			bx = getmaxx(win)/2;
			by = getmaxy(win)/2;
		}

		//Bounces ball if it hits upper and lowerd boundary
		if(by == getmaxy(win) - 2){
			byDirection = -1;
		}else if (by == 1){
			byDirection = 1;
		}

		//Updates enemy paddle direction to follow ball
		eyDirection = byDirection;
		
		//Bounces ball if paddles are hit
		for (size_t i = 0; i <= 2; i++)
		{
			if(bx == px + 1 && by == py[i]){
				bxDirection = 1;
			}
			if (bx == ex - 1 && by == ey[i]){
				bxDirection = -1 ;
			}
			if((bx == px + 1 && by == py[i]) && byDirection == 0){
				bxDirection = 1;
				byDirection = byDirectionRandom;
			}
			if((bx == ex - 1 && by == ey[i]) && byDirection == 0){
				bxDirection = -1;
				byDirection = byDirectionRandom;
			}
		}
		
		if(score[0] == 10 || score [1] == 10){
			loop = FALSE;
		}
		
		//Updates the screen
		werase(win);
		box(win,'|','=');
		
		for (size_t i = 0; i <= 2; i++){

			mvwaddch(win, py[i], px, player);
			mvwaddch(win, ey[i], ex, enemy);
		}
		mvwaddch(win, by, bx, ball);
		
		mvwaddstr(ins,0,0,"[Press F1 to quit]");
		mvwprintw(ins,0,21,"%d",score[0]);
		mvwprintw(ins,0,24,"%d",score[1]);
		wrefresh(ins);
		usleep(50000);
	}
	nodelay(win, FALSE);
	mvwprintw(win, getmaxy(win)/2, (getmaxx(win) - strlen("Thank you for playing!"))/2, "Thank you for playing!");
	wrefresh(win);
	box(win,'|','=');
	wgetch(win);
	
	endwin();
	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, ' ' , ' ');		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}