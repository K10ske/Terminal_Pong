#include <curses.h>
#include <unistd.h>

using namespace std;
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(){
	initscr();
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	WINDOW *win;
	WINDOW *ins;
	ins = newwin(1,50,0,0);
	win = create_newwin(25,50,1,0);
	
	curs_set(0);
	keypad(win,TRUE);
	

	
	nodelay(win,TRUE);
	char player = '|';
	char ball = 'o';
	int frames= 0;
	int px, py[3];
	int bx, by;
	int bxDirection, byDirection;
	int press;
	bool loop = TRUE;

	wbkgd(win,COLOR_PAIR(1) | ' ');
	wbkgd(ins,COLOR_PAIR(1) | ' ');
	box(win,'|','=');
	
	px = 2;
	py[0] = getmaxy(win)/2;
	py[1] = py[0] + 1;
	py[2] = py[0] - 1;
	bx = getmaxx(win)/2;
	by = getmaxy(win)/2;
	bxDirection = 1;
	byDirection = 0;
	
	while (loop){
		
		//Controls for the player paddle
		
		press = wgetch(win);
		switch (press){
		case KEY_UP:
			if(py[2] == 1){
				break;
			}else{
				py[0] += -1;
				py[1] = py[0] + 1;
				py[2] = py[0] - 1;
				break;
			}

		case KEY_DOWN:
			if(py[1] == getmaxy(win)-2){
				break;
			}else{
				py[0] += 1;
				py[1] = py[0] + 1;
				py[2] = py[0] - 1;
				break;
			}
		
		case KEY_F(1):
			loop = FALSE;
			break;
		}

		//Ball Movement

		


		
		bx += bxDirection;
		by += byDirection;

		if(bx == getmaxx(win) - 2){
			bxDirection = -1;
		}else if (bx == 1){
			bxDirection = 1;
		}

		
		if(by == getmaxy(win) - 2){
			byDirection = -1;
		}else if (by == 1){
			byDirection = 1;
		}

		for (size_t i = 0; i <= 2; i++)
		{
			if(bx == px + 1 && by == py[i]){
				bxDirection = 1;
			}
		}
		
		

		werase(win);
		box(win,'|','=');
		
		for (size_t i = 0; i <= 2; i++){

			mvwaddch(win, py[i], px, player);
		}
		mvwaddch(win, by, bx, ball);
		
		mvwaddstr(ins,0,0,"[Press F1 to quit]");
		mvwprintw(ins,0,21,"%d",bx);
		mvwprintw(ins,0,24,"%d",by);
		wrefresh(ins);
		usleep(50000);
	}
	

	
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