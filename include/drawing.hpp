#ifndef DRAWING
#define DRAWING

#include "interface.hpp"

#define BORDER_UNINTIALISED -1

class Context{
    public:

    Context(Screen *s);

    u_int32_t colour;

    void drawLine(int x1, int y1, int x2, int y2);
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, float z);

    Screen *screen;

    private:
    void drawLineHorizontal(int x1, int y1, int x2, int y2);
    void drawLineVertical(int x1, int y1, int x2, int y2);

    void drawLineHorizontal(int x1, int y1, int x2, int y2, int *border, int borderStart, bool right);
    void drawLineVertical(int x1, int y1, int x2, int y2, int *border, int borderStart);
    void drawLine(int x1, int y1, int x2, int y2, int *border, int borderStart, bool right);

};

void resetColourIndex();

#endif