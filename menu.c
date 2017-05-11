/* To Display The Menu and Allow User To Set Up */

#include <lcd.h>

uint16_t menuSize = 2, menuSel = 1, stage = 0;
char *menu[4] = {"MODE", "Countup", "Countdown"};

uint16_t menuStopSize = 4, menuStopSel = 1;
char *menuStop[5] = {"COUNT UP", "Start", "Stop", "Reset", "Back"};

uint16_t menuCountSize = 5, menuCountSel = 1;
char *menuCount[6] = {"COUNT DOWN", "Set", "Start", "Stop", "Reset", "Back"};

uint16_t menuTimeSetSize = 3;
char *menuTimeSet[6] = {"TIME SET", "Hours", "Minutes", "Seconds"};

void title(char* title, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	rectangle r;
	r.top = y;
	r.left = x;
	r.right = r.left + width;
	r.bottom = y + height;

	fill_rectangle(r, RED);
	display_color(WHITE, RED);
	display_string_xy(title, x + 4, y + 5);
}

void showMenu(char** items, uint16_t itemsCount, uint16_t x, uint16_t y, uint16_t width, uint16_t highlight, uint16_t pointer)
{
	uint16_t top = y, height = 15;

	rectangle r;
	r.top = y;
	r.left = x;
	r.right = r.left + width;
	r.bottom = r.top + (1.5 * height) + (height * itemsCount);
	fill_rectangle(r, BLACK);

	title(items[0], x, y, width, height);

	top+=height * 1.5;
	uint16_t c, leftOffset = 25;
	for(c = 1; c < itemsCount + 1; c++)
	{
		if(c == pointer)
		{
			display_color(RED, BLACK);
			display_string_xy(" >>", x, top);
		}

		if(c == highlight)
		{
			display_color(RED, BLACK);
		}
		else
		{
			display_color(WHITE, BLACK);
		}	

		display_string_xy(items[c], x + leftOffset, top);
		
		top = top + height;
	}
}

void drawMenu(uint8_t *timeSet)
{
	showMenu(menu, menuSize, 10, 120, 80, stage, menuSel);

	if(stage)
	{
		if(menuSel == 1)
		{
			showMenu(menuStop, menuStopSize, 100, 120, 80, 0, menuStopSel);
		}
		else if(menuSel == 2)
		{
			showMenu(menuCount, menuCountSize, 100, 120, 80, 0, menuCountSel);
			
			if(*timeSet < 3)
			{
				showMenu(menuTimeSet, menuTimeSetSize, 190, 120, 80, menuTimeSetSize - *timeSet, 0);
			}
			else
			{
				rectangle r;
				r.top = 120;
				r.left = 190;
				r.right = LCDHEIGHT;
				r.bottom = LCDWIDTH;
				fill_rectangle(r, BLACK);
			}
		}
	}
	else
	{
		rectangle r;
		r.top = 120;
		r.left = 100;
		r.right = LCDHEIGHT;
		r.bottom = LCDWIDTH;
		fill_rectangle(r, BLACK);
	}
}

void adjustMenu(int direction)
{
	if(stage)
	{
		if(menuSel == 1)
		{
			menuStopSel+=direction;
			if(menuStopSel < 1)
			{
				menuStopSel = 1;
			}
			else if(menuStopSel > menuStopSize)
			{
				menuStopSel = menuStopSize;
			}

		}
		else if(menuSel == 2)
		{
			menuCountSel+=direction;
			if(menuCountSel < 1)
			{
				menuCountSel = 1;
			}
			else if(menuCountSel > menuCountSize)
			{
				menuCountSel = menuCountSize;
			}
		}
	}
	else
	{
		menuSel += direction;
		if(menuSel < 1)
		{
			menuSel = 1;
		}
		else if(menuSel > menuSize)
		{
			menuSel = menuSize;
		}
	}
}

void selectPush(int *direction, uint8_t *timeSet, uint8_t* time)
{
	if(*timeSet < 3)
	{
		if(!timeSet)
		{
			*timeSet = 3;
		}
		else
		{
			*timeSet = *timeSet - 1;
		}
		
		return;
	}

	if(stage)
	{
		if(menuSel == 1)
		{
			switch(menuStopSel)
			{
			case 1:
				*direction = 1;
				break;

			case 2:
				*direction = 0;
				break;

			case 3:
				*direction = 0;
				time[0] = 0;
				time[1] = 0;
				time[2] = 0;
				break;
			case 4:
				stage = 0;
				break;
			}
		}
		else if(menuSel == 2)
		{
			switch(menuCountSel)
			{
			case 1:
				*timeSet = 2;
				break;

			case 2:
				*direction = -1;
				break;

			case 3:
				*direction = 0;
				break;
				
			case 4:
				*direction = 0;
				time[0] = 0;
				time[1] = 0;
				time[2] = 0;
				break;
			
			case 5:
				stage = 0;
				break;
			}
		}
	}
	else
	{
		stage = menuSel;
	}
}
