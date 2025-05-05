#ifndef BOSSBULLET_H
#define BOSSBULLET_H

#include <SDL.h>

class BossBullet {
public:
    BossBullet(int x, int y, float dx, float dy);

    void move();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    SDL_Rect getRect() const;
    bool isOffScreen() const;

private:
    SDL_Rect rect;
    float dx;
    float dy;
    float speed;
};

#endif
