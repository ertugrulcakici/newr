#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lcd_lib.h"
#include "graph_class.hpp"

int main()
{
	lcd_init(); // LCD initialization

	int semiHeight = LCD_HEIGHT / 2;
	int semiWidth = LCD_WIDTH / 2;
	int radius = semiHeight < semiWidth ? semiHeight : semiWidth;
	Circle circle = Circle({semiWidth, semiHeight}, radius, {
																255,
																0,
																0,
															},
						   {0, 0, 0});
	circle.draw();

	Character c = Character({semiWidth, 0}, 'A', {255, 255, 0}, {0, 0, 0});
	c.draw();
	// Line topLeftToBottomRight = Line({0, 0}, {LCD_WIDTH, LCD_HEIGHT}, {255, 255, 255}, {0, 0, 0});
	// Line topRightToBottomLeft = Line({LCD_WIDTH, 0}, {0, LCD_HEIGHT}, {255, 255, 255}, {0, 0, 0});
	// topLeftToBottomRight.draw();
	// topRightToBottomLeft.draw();
}
