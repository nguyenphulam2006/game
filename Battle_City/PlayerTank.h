#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Bullet.h"

class PlayerTank {
public:
    PlayerTank(SDL_Renderer* renderer, int x, int y);
    ~PlayerTank();

    void handleInput(const Uint8* keystate);
    void update();
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
    int speed;
    SDL_Texture* texture;
    std::vector<Bullet> bullets;

    SDL_Renderer* renderer;
};

#endif
