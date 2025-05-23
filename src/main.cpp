
#include <iostream>
#include <cmath>

#include "interface.hpp"
#include "drawing.hpp"

#define FPS_SAMPLE_RATE 400

class Camera{
    public:
    Camera() {
        x = 0;
        y = 0;
        zoom = 32;
        speed = 4;
        zoomSpeed = 0.01;
    }
    float x;
    float y;
    float zoom;
    float speed;
    float zoomSpeed;

    void move(Screen s) {
        if(s.isKeyDown('w')) {
            y -= speed/zoom;
        }
        if(s.isKeyDown('a')) {
            x -= speed/zoom;
        }
        if(s.isKeyDown('s')) {
            y += speed/zoom;
        }
        if(s.isKeyDown('d')) {
            x += speed/zoom;
        }
        if(s.isKeyJustPressed('-')) {
            zoom /= 2;
        }
        if(s.isKeyJustPressed('=')) {
            zoom *= 2;
        }
    }
};

u_int32_t getColourFromVal(float val, float max, bool logarithmic) {
    // std::max((float) 0, log2(256*abs(val)/max)) * 32
    // now we use log of log
    int grad = logarithmic ? std::max((float) 0, log2(log2(255*val/max)))*255/3 : (abs(val)/max)*255;

    u_int8_t r,g,b;

    if(val >= 0) {
        r = std::min(grad*4,255);
        g = std::min(grad,255);
        b = std::min(grad,255);
    }
    else{
        r = std::min(grad,255);
        g = std::min(grad*4,255);
        b = std::min(grad,255);
    }

    u_int32_t c = 0xFF000000 | (r << 16) | (g << 8) | b;

    return c;
};

float factorial(int n) {
    float o = 1;
    for(int i = 1; i <= n; i++) {
        o*=i;
    }
    return o;
};

float nChooseR(float n, float r) {
    return std::tgammaf(n+1)/(std::tgammaf(r+1)*std::tgammaf(n-r+1));
};

float calcPascalTriangle(float realx, float realy, Camera cam, bool continuous, bool showTri, bool negatives) {
    float threshold = 1/cam.zoom;

    float trix = realx + (realy/2)*showTri;
    float triy = realy;

    if(((trix <= triy && trix >= 0) | negatives) && ((trix-floor(trix) < threshold && triy-floor(triy) < threshold) | continuous)) {
        float o = nChooseR(triy, trix);
        return abs(o) == INFINITY ? 0 : o;
    }

    return 0;
}

void drawPascalTriangle(int sx, int sy, Camera cam, Screen s, float *vals, float max, bool continuous, bool logarithmic, bool showTri, bool negatives) {

    float threshold = 1/cam.zoom;
    
    float realx = ((sx-s.width/2)/cam.zoom)+cam.x;
    float realy = ((sy-s.height/2)/cam.zoom)+cam.y;

    float trix = realx + (realy/2)*showTri;
    float triy = realy;

    if(((trix <= triy && trix >= 0) | negatives) && ((trix-floor(trix) < threshold && triy-floor(triy) < threshold) | continuous)) {
        s.putPixel(sx, sy, getColourFromVal(*(vals+sx+(sy*s.width)), max, logarithmic));
    }
    else if(realx == 0 || realy == 0) {
        s.putPixel(sx, sy, 0xFF888888);
    }
    else{
        s.putPixel(sx, sy, 0xFFEEEEFF);
    }

};

int main() {

    std::cout << "start of program\n";
    
    Screen screen(1000, 1000);

    float *vals = new float[screen.width*screen.height];

    Context ctx(&screen);

    bool continuous = false;
    bool logarithmic = false;
    bool showTri = true;
    bool negatives = false;

    float limit = 1000;
    float limitSpeed = 10;
    
    Camera camera;

    int count = 0;
    int averageFPS[FPS_SAMPLE_RATE] = {0};

    float time = 0;
    float dt = 0;
    
    while(!screen.quitPolled) {

        resetColourIndex();
        screen.updateInput();

        // get delta time
        dt = SDL_GetTicks()-time;
        time = SDL_GetTicks();
        // counting fps
        averageFPS[count] = dt;
        if(++count >= FPS_SAMPLE_RATE) {
            count = 0;

            float total = 0;
            for(int i = 0; i < FPS_SAMPLE_RATE; i++) {
                total += averageFPS[i];
                averageFPS[i] = 0;
            }
            total/=FPS_SAMPLE_RATE;
            std::cout << "FPS: " << 1000/total << "\n";
        }

        camera.move(screen);

        if(screen.isKeyJustPressed(' ')) {
            continuous = !continuous;
        }
        if(screen.isKeyJustPressed('l')) {
            logarithmic = !logarithmic;
        }
        if(screen.isKeyJustPressed(9)) { // tab
            showTri = !showTri;
        }
        if(screen.isKeyJustPressed('n')) { // tab
            negatives = !negatives;
        }
        if(screen.isKeyDown('q')) {
            limit -= limitSpeed;
        }
        if(screen.isKeyDown('e')) {
            limit += limitSpeed;
        }

        float max = 0;

        for(int y = 0; y < screen.height; y++) {
            for(int x = 0; x < screen.width; x++) {
                float realx = ((x-screen.width/2)/camera.zoom)+camera.x;
                float realy = ((y-screen.height/2)/camera.zoom)+camera.y;

                float v = calcPascalTriangle(realx, realy, camera, continuous, showTri, negatives);
                *(vals+x+(y*screen.width)) = v;

                if(abs(v) > max && abs(v) < limit) {
                    max = abs(v);
                }
            }
        }

        // render stuff
        for(int x = 0; x < screen.width; x++) {
            for(int y = 0; y < screen.height; y++) {
                drawPascalTriangle(x, y, camera, screen, vals, max, continuous, logarithmic, showTri, negatives);
            }
        }
        screen.render();
    }

    screen.destroyScreen();

    delete[] vals;

    return 0;
};