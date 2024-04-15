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

Ticker ticker;
volatile bool delay_flag = false;

int g_tab_sin[61] = {/* ... */};
int g_tab_cos[61] = {/* ... */};
int32_t g_angle = 0;
RGB g_hand_color = {255, 0, 0}; // Sabit el rengi
int g_timer_state = 0;
Line g_hand({0, 0}, {0, 0}, {0, 0, 0}, {0, 0, 0}); // Sabit el nesnesi tanımla

void delayCallback()
{
	delay_flag = true;
}

void delay(int ms)
{
	delay_flag = false;
	ticker.attach(delayCallback, ms / 1000.0f);
	while (!delay_flag)
		;
}

void draw_clock_face()
{
	Circle clock_face({CLOCK_CENTER_X, CLOCK_CENTER_Y}, CLOCK_RADIUS, {255, 0, 0}, {0, 0, 0});
	clock_face.draw();
}

void draw_hand()
{
	int32_t hand_x = CLOCK_CENTER_X + HAND_LENGTH * g_tab_cos[g_angle] / 1000;
	int32_t hand_y = CLOCK_CENTER_Y + HAND_LENGTH * g_tab_sin[g_angle] / 1000;
	g_hand.setStart({CLOCK_CENTER_X, CLOCK_CENTER_Y});
	g_hand.setEnd({hand_x, hand_y});
	g_hand.setColor(g_hand_color);
	g_hand.setBackgroundColor({0, 0, 0});
	g_hand.draw();
}

void update_timer()
{
	g_hand.hide();
	g_angle = (g_angle + 6) % ANGLE_RANGE;
	draw_hand();
}

void control_timer()
{
	if (g_timer_state == 0)
	{
		// Yeni bir durum, hiçbir şey yapma
	}
	else
	{
		update_timer();
	}
}

int main()
{
	lcd_init();
	draw_clock_face();
	g_timer_state = 1;

	while (1)
	{
		control_timer();
		delay(100);
	}

	return 0;
}