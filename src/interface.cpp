#include "interface.hpp"

#include <SDL2/SDL.h>
#include <iostream>

Screen::Screen(int w, int h) {
    
    width = w;
    height = h;

    quitPolled = false;
    SDL_GetKeyboardState(&numKeys);
    keyboard = new bool[numKeys]; // allocating statically breaks the program ig
    past_keyboard = new bool[numKeys];

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("pascal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    textureBuffer = new uint32_t[(w * h)];
};

void Screen::destroyScreen() {
    delete[] keyboard;
    delete[] past_keyboard;
    delete[] textureBuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};

void Screen::render() {
    SDL_UpdateTexture(texture, NULL, textureBuffer, width*sizeof(u_int32_t));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
};

void Screen::clearScreen(u_int32_t c) {
    for(int i = 0; i < width*height; i++) {
        textureBuffer[i] = c;
    }
}

void Screen::updateInput() {
    // update past keyboard
    for(int i = 0; i < numKeys; i++) {
        *(past_keyboard + i) = *(keyboard+i);
    }
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                quitPolled = true;
                break;
            case SDL_KEYDOWN:
                changeKeyState(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                changeKeyState(event.key.keysym.sym, false);
                break;
        }
    }
};

void Screen::changeKeyState(SDL_Keycode k, bool value) {
    if(k < 0 || k >= numKeys) return;
    keyboard[k] = value;
};

void Screen::putPixel(int x, int y, u_int32_t c) {
    if(x < 0 || y < 0 || x >= width || y >= height) return; // off screen

    textureBuffer[y*width + x] = c;
};

bool Screen::isKeyDown(char k) {
    return keyboard[k];
}

bool Screen::isKeyJustPressed(char k) {
    return keyboard[k] && !past_keyboard[k];
}

bool Screen::isKeyJustReleased(char k) {
    return !keyboard[k] && past_keyboard[k];
}