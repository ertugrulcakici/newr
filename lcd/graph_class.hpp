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

#include "lcd_lib.h"
#include "font8x8.h"

// Simple graphic interface

struct Point2D
{
    int32_t x, y;
};

struct RGB
{
    uint8_t r, g, b;
};

class GraphElement
{
public:
    // foreground and background color
    RGB m_fg_color, m_bg_color;

    // constructor
    GraphElement(RGB t_fg_color, RGB t_bg_color) : m_fg_color(t_fg_color), m_bg_color(t_bg_color) {}

    // ONLY ONE INTERFACE WITH LCD HARDWARE!!!
    void drawPixel(int32_t t_x, int32_t t_y) { lcd_put_pixel(t_x, t_y, convert_RGB888_to_RGB565(m_fg_color)); }

    // Draw graphics element
    virtual void draw() = 0;

    // Hide graphics element
    virtual void hide()
    {
        swap_fg_bg_color();
        draw();
        swap_fg_bg_color();
    }

    uint16_t convert_RGB888_to_RGB565(RGB t_color)
    {
        // return 0x07E0; /* green color */

        return (t_color.r >> 3) << 11 | (t_color.g >> 2) << 5 | (t_color.b >> 3);
    }

private:
    // swap foreground and backgroud colors
    void swap_fg_bg_color()
    {
        RGB l_tmp = m_fg_color;
        m_fg_color = m_bg_color;
        m_bg_color = l_tmp;
    }

    // IMPLEMENT!!!
    // conversion of 24-bit RGB color into 16-bit color format
};

class Pixel : public GraphElement
{
public:
    // constructor
    Pixel(Point2D t_pos, RGB t_fg_color, RGB t_bg_color) : GraphElement(t_fg_color, t_bg_color), m_pos(t_pos) {}
    // Draw method implementation
    virtual void draw() { drawPixel(m_pos.x, m_pos.y); }
    // Position of Pixel
    Point2D m_pos;
};

class Circle : public GraphElement
{
public:
    // Center of circle
    Point2D m_center;
    // Radius of circle
    int32_t m_radius;

    Circle(Point2D t_center, int32_t t_radius, RGB t_fg, RGB t_bg) : GraphElement(t_fg, t_bg), m_center(t_center), m_radius(t_radius) {}

    void draw8pixels(int x, int y)
    {
        drawPixel(m_center.x + x, m_center.y + y);
        drawPixel(m_center.x - x, m_center.y + y);
        drawPixel(m_center.x + x, m_center.y - y);
        drawPixel(m_center.x - x, m_center.y - y);
        drawPixel(m_center.x + y, m_center.y + x);
        drawPixel(m_center.x - y, m_center.y + x);
        drawPixel(m_center.x + y, m_center.y - x);
        drawPixel(m_center.x - y, m_center.y - x);
    }

    void draw()
    {

        int x = 0, y = m_radius;
        int d = 3 - 2 * m_radius;
        draw8pixels(x, y);
        while (y >= x)
        {

            x++;

            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            }
            else
                d = d + 4 * x + 6;
            draw8pixels(x, y);
            // delay(50);
        }

    } // IMPLEMENT!!!
};

class Character : public GraphElement
{
public:
    // position of character
    Point2D m_pos;
    // character
    char m_character;

    Character(Point2D t_pos, char t_char, RGB t_fg, RGB t_bg) : GraphElement(t_fg, t_bg), m_pos(t_pos), m_character(t_char){};

    //  for (int code = 0; code < 256; code++) // ascii table
    // {
    //     for (int y = 0; y < 8; y++)
    //     {
    //         for (int x = 0; x < 8; x++)
    //         {
    //             if (font8x8[code][y] & (1 << x))
    //             {
    //                 printf("#");
    //             }
    //             else
    //             {
    //                 printf(".");
    //             }
    //             printf("\n");
    //         }
    //     }
    // }

    void lcd_put_char(int pos_x, int pos_y, char ch, uint16_t fg_color, uint16_t bg_color)
    {
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                if (font8x8[(int)ch][y] & (1 << x))
                {
                    lcd_put_pixel(x + x, y + y, fg_color);
                    lcd_put_pixel(x + x + 1, y + y, fg_color);
                    lcd_put_pixel(x + x, y + y + 1, fg_color);
                    lcd_put_pixel(x + x + 1, y + y + 1, fg_color);
                }
                else
                {
                    lcd_put_pixel(x + x, y + y, bg_color);
                    lcd_put_pixel(x + x + 1, y + y, bg_color);
                    lcd_put_pixel(x + x, y + y + 1, bg_color);
                    lcd_put_pixel(x + x + 1, y + y + 1, bg_color);
                }
            }
        }
    }

    void draw()
    {
        lcd_put_char(m_pos.x, m_pos.y, m_character, convert_RGB888_to_RGB565(m_fg_color), convert_RGB888_to_RGB565(m_bg_color));
    }; // IMPLEMENT!!!
};

class Line : public GraphElement
{
public:
    // the first and the last point of line
    Point2D m_pos1, m_pos2;

    Line(Point2D t_pos1, Point2D t_pos2, RGB t_fg, RGB t_bg) : GraphElement(t_fg, t_bg), m_pos1(t_pos1), m_pos2(t_pos2) {}

    void draw()
    {
        int x0 = m_pos1.x;
        int y0 = m_pos1.y;
        int x1 = m_pos2.x;
        int y1 = m_pos2.y;
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = x0 < x1 ? 1 : -1;
        int sy = y0 < y1 ? 1 : -1;
        int err = dx - dy;
        while (x0 != x1 || y0 != y1)
        {
            drawPixel(x0, y0);
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }; // IMPLEMENT!!!
};
