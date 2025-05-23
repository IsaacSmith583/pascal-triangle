#include "drawing.hpp"

#include "interface.hpp"
#include <iostream>

Context::Context(Screen *s) {
    screen = s;
}

void Context::drawLineHorizontal(int x1, int y1, int x2, int y2) {
    int dx = x2-x1;
    int dy = y2-y1;

    int dir = 1;
    if(dy < 0) { // y needs to go up
        dy = -dy;
        dir = -1;
    }

    int D = 2*dy-dx; // calculate inital D
    int y = y1;
    for(int i = 0; i <= dx; i++) {
        screen->putPixel(i+x1, y, colour);

        if(D > 0) { // d1 < d0, need to increment y
            y += dir;
            D += 2*dy - 2*dx;
        }
        else{
            D += 2*dy;
        }
    }
};

void Context::drawLineHorizontal(int x1, int y1, int x2, int y2, int *border, int borderIndex, bool right) {
    int dx = x2-x1;
    int dy = y2-y1;

    int dir = 1;
    if(dy < 0) { // y needs to go up
        dy = -dy;
        dir = -1;
    }

    int D = 2*dy-dx; // calculate inital D
    int y = y1;
    for(int i = 0; i <= dx; i++) {

        int iy = (dir == -1) ? borderIndex + dy - (y1-y) : borderIndex + (y-y1);
        if(right || border[iy] == BORDER_UNINTIALISED) {
            border[iy] = i+x1;
        }

        if(D > 0) { // d1 < d0, need to increment y
            y += dir;
            D += 2*dy - 2*dx;
        }
        else{
            D += 2*dy;
        }
    }
};

void Context::drawLineVertical(int x1, int y1, int x2, int y2) {
    int dx = x2-x1;
    int dy = y2-y1;

    int dir = 1;
    if(dx < 0) { // y needs to go up
        dx = -dx;
        dir = -1;
    }

    int D = 2*dx-dy; // calculate inital D
    int x = x1;
    for(int i = 0; i <= dy; i++) {
        screen->putPixel(x, i+y1, colour);

        if(D > 0) { // d1 < d0, need to increment y
            x += dir;
            D += 2*dx - 2*dy;
        }
        else{
            D += 2*dx;
        }
    }
};

void Context::drawLineVertical(int x1, int y1, int x2, int y2, int *border, int borderIndex) {
    int dx = x2-x1;
    int dy = y2-y1;

    int dir = 1;
    if(dx < 0) { // y needs to go up
        dx = -dx;
        dir = -1;
    }

    int D = 2*dx-dy; // calculate inital D
    int x = x1;
    for(int i = 0; i <= dy; i++) {
        border[borderIndex+i] = x;

        if(D > 0) { // d1 < d0, need to increment y
            x += dir;
            D += 2*dx - 2*dy;
        }
        else{
            D += 2*dx;
        }
    }
};

void Context::drawLine(int x1, int y1, int x2, int y2) {
    if(std::abs(x2-x1) > std::abs(y2-y1)) { // if horizontal
        if(x1 > x2) { // swap vertex
            drawLineHorizontal(x2, y2, x1, y1);
        }
        else{
            drawLineHorizontal(x1, y1, x2, y2);
        }
    }
    else{
        if(y1 > y2) { // swap vertex
            drawLineVertical(x2, y2, x1, y1);
        }
        else{
            drawLineVertical(x1, y1, x2, y2);
        }
    }
}

void Context::drawLine(int x1, int y1, int x2, int y2, int *border, int borderStart, bool right) {
    if(std::abs(x2-x1) > std::abs(y2-y1)) { // if horizontal
        if(x1 > x2) { // swap vertex
            drawLineHorizontal(x2, y2, x1, y1, border, borderStart, right);
        }
        else{
            drawLineHorizontal(x1, y1, x2, y2, border, borderStart, right);
        }
    }
    else{
        if(y1 > y2) { // swap vertex
            drawLineVertical(x2, y2, x1, y1, border, borderStart);
        }
        else{
            drawLineVertical(x1, y1, x2, y2, border, borderStart);
        }
    }
}

void swapValues(int *v1, int *v2) {
    int t = *v1;
    *v1 = *v2;
    *v2 = t;
}

void swapAddresses(int **v1, int **v2) {
    int *t = *v1;
    *v1 = *v2;
    *v2 = t;
}

int colours[] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFF00, 0xFF00FFFF, 0xFFFF00FF};
int colouri = 0;

void resetColourIndex() {
    colouri = 0;
}

void Context::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, float z = 1) {
    std::cout << z << "\n";

    // randomise colour (debugging)
    colouri++;
    colouri = colouri % 6;

    if(y2 < y1 || y2 == y1 && x2 < x1) { // swap 1 and 2 by y and x if tie
        swapValues(&x1, &x2);
        swapValues(&y1, &y2);
    }
    if(y3 < y2 || y2 == y1 && x2 < x1) { // swap 2 and 3 by y and x if tie
        swapValues(&x2, &x3);
        swapValues(&y2, &y3);
    }
    if(y2 < y1 || y2 == y1 && x2 < x1) { // swap 1 and (2 or 3) by y and x if tie
        swapValues(&x1, &x2);
        swapValues(&y1, &y2);
    }

    int size = y3 - y1 + 1;
    int *border1 = new int[size];
    int *border2 = new int[size];
    if(border1 == NULL || border2 == NULL) {
        std::cout << "Draw triangle failed; no memory\n";
        return;
    }
    for(int i = 0; i < size; i++) {
        border1[i] = BORDER_UNINTIALISED;
        border2[i] = BORDER_UNINTIALISED;
    }

    bool right = x2 > x1;

    drawLine(x1, y1, x2, y2, border2, 0, right);
    drawLine(x2, y2, x3, y3, border2, std::abs(y2-y1), right);

    drawLine(x1, y1, x3, y3, border1, 0, !right);


    if(!right) {// border 1 is right side, switch pointers
        swapAddresses(&border1, &border2); // probably not the best idea though...
    }


    for(int i = 0; i < size; i++) {
        for(int j = border1[i]; j <= border2[i]; j++) {
            screen->putPixel(j, i+y1, colours[colouri]);
        }
    }

    delete[] border1;

}