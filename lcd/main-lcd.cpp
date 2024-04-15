#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lcd_lib.h"
#include "mbed.h"
#include "graph_class.hpp"

#define CLOCK_CENTER_X (LCD_WIDTH / 2)
#define CLOCK_CENTER_Y (LCD_HEIGHT / 2)
#define CLOCK_RADIUS (LCD_HEIGHT / 2 < LCD_WIDTH / 2 ? LCD_HEIGHT / 2 : LCD_WIDTH / 2)
#define HAND_LENGTH (CLOCK_RADIUS * 0.75)
#define ANGLE_RANGE 360

int g_tab_sin[61] = {0, 104, 207, 309, 406, 499, 587, 669, 743, 809, 866, 913, 951, 978, 994, 1000, 994, 978, 951, 913, 866, 809, 743, 669, 587, 499, 406, 309, 207, 104, 0, -104, -207, -309, -406, -499, -587, -669, -743, -809, -866, -913, -951, -978, -994, -1000, -994, -978, -951, -913, -866, -809, -743, -669, -587, -499, -406, -309, -207, -104, 0};
int g_tab_cos[61] = {1000, 994, 978, 951, 913, 866, 809, 743, 669, 587, 500, 406, 309, 207, 104, 0, -104, -207, -309, -406, -499, -587, -669, -743, -809, -866, -913, -951, -978, -994, -1000, -994, -978, -951, -913, -866, -809, -743, -669, -587, -500, -406, -309, -207, -104, 0, 104, 207, 309, 406, 500, 587, 669, 743, 809, 866, 913, 951, 978, 994, 1000};

int32_t g_angle = 0;
RGB g_hand_color = {0, 255, 0}; // Green hand color
int g_timer_state = 1;			// 1 for running, 0 for stopped

void draw_clock_face()
{
	Circle clock_face({CLOCK_CENTER_X, CLOCK_CENTER_Y}, CLOCK_RADIUS, {255, 0, 0}, {0, 0, 0});
	clock_face.draw();
}

void draw_hand()
{
	int32_t hand_x = CLOCK_CENTER_X + HAND_LENGTH * g_tab_cos[g_angle] / 1000;
	int32_t hand_y = CLOCK_CENTER_Y + HAND_LENGTH * g_tab_sin[g_angle] / 1000;
	Line hand({CLOCK_CENTER_X, CLOCK_CENTER_Y}, {hand_x, hand_y}, g_hand_color, {0, 0, 0});
	hand.draw();
}

void update_timer()
{
	Line hand({CLOCK_CENTER_X, CLOCK_CENTER_Y}, {CLOCK_CENTER_X + HAND_LENGTH * g_tab_cos[g_angle] / 1000, CLOCK_CENTER_Y + HAND_LENGTH * g_tab_sin[g_angle] / 1000}, {0, 0, 0}, {0, 0, 0});
	hand.hide();
	g_angle = (g_angle + 6) % ANGLE_RANGE;
	draw_hand();
}

void control_timer()
{
	if (g_timer_state == 0)
	{
		// Timer is stopped, do nothing
	}
	else
	{
		update_timer();
	}
}

int main()
{
	lcd_init();
	// draw_clock_face();
	// while (1)
	// {
	// 	control_timer();
	// }
	return 0;
}