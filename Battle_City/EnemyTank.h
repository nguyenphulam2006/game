#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "EnemyBullet.h"
class EnemyTank {
public:
    EnemyTank(SDL_Renderer* renderer, int x, int y);
    virtual ~EnemyTank();
    virtual void update();
    virtual void render(SDL_Renderer* renderer);
    virtual std::vector<EnemyBullet*>& getBullets() { return bullets; }
    void shoot();
    SDL_Rect getRect() const { return rect; }
    bool isDestroyed() const { return destroyed; }
    void destroy() { destroyed = true; }

protected:
    void loadTexture(const char* path);
    std::vector<EnemyBullet*> bullets;
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    SDL_Renderer* renderer;
    bool destroyed = false;
};

#endif

