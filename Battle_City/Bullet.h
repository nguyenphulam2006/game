#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    Bullet(int x, int y, int dx, int dy);
    void move();
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
    int dx, dy;
};
#endif
