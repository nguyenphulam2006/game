#pragma once
#include <SDL.h>

class Wall {
    SDL_Texture* texture;
private:
    SDL_Rect rect;
    bool destructible;
    bool alive;

public:
    Wall(int x, int y, bool destructible = true);

    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const { return rect; }

    bool isDestructible() const { return destructible; }
    bool isAlive() const { return alive; }
    void destroy() { if (destructible) alive = false; }
};
