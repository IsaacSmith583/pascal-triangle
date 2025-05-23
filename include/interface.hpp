#ifndef INTERFACE
#define INTERFACE

#include <SDL2/SDL.h>

class Screen{
    public:
    Screen(int w, int h);

    int width; // for some reason any member declared above here somehow breaks the program????
    int height;

    bool quitPolled;
    float mouseMotionX;
    float mouseMotionY;

    void destroyScreen();
    void render();
    void clearScreen(u_int32_t c);
    void putPixel(int x, int y, u_int32_t c);
    
    void updateInput();

    bool isKeyDown(char k);
    bool isKeyJustPressed(char k);
    bool isKeyJustReleased(char k);

    private:
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t *textureBuffer;

    bool *keyboard;
    bool *past_keyboard;

    int numKeys;
    void changeKeyState(SDL_Keycode k, bool value);
};

#endif