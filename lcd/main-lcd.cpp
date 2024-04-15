// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 09/2019
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         OpenCV simulator of LCD
//
// **************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lcd_lib.h"
#include "graph_class.hpp"

int main()
{
	lcd_init(); // LCD initialization

	Circle circle = Circle({100, 100}, 70, {
											   255,
											   0,
											   0,
										   },
						   {0, 0, 0});
	circle.draw();

	Character c = Character({0, 0}, 'A', {255, 255, 0}, {0, 0, 0});
	c.draw();
	Line topLeftToBottomRight = Line({0, 0}, {LCD_WIDTH, LCD_HEIGHT}, {255, 255, 255}, {0, 0, 0});
	Line topRightToBottomLeft = Line({LCD_WIDTH, 0}, {0, LCD_HEIGHT}, {255, 255, 255}, {0, 0, 0});
	topLeftToBottomRight.draw();
	topRightToBottomLeft.draw();
}
