#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>

class EnemyTank {
public:
    EnemyTank(SDL_Renderer* renderer, int x, int y);
    ~EnemyTank();
    void update();
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
    SDL_Texture* texture;
};
#endif
