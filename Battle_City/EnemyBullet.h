#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include <SDL.h>

class EnemyBullet {
public:
    EnemyBullet(int x, int y);
    void move();
    void render(SDL_Renderer* renderer, SDL_Texture* bulletTexture);
    SDL_Rect getRect() const { return rect; }
    bool isOffScreen() const { return rect.y > 600; }
private:
    SDL_Rect rect;
};

#endif
