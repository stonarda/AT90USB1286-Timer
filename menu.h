/* Header for showing the menu */

#include "lcd.h"

void initMenu();
void showMenu(char** items, uint16_t itemsCount, uint16_t x, uint16_t y, uint16_t width, uint16_t highlight, uint16_t pointer);
void adjustMenu(int direction);
void selectPush(int *direction, uint8_t *timeSet, uint8_t* time);
