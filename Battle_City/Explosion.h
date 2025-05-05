#pragma once
#include <SDL.h>

class Explosion {
public:
    Explosion(SDL_Texture* texture, int x, int y);
    void update();
    void render(SDL_Renderer* renderer);
    bool isFinished() const;
    bool isAlive() const { return !isFinished(); }
private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    int frame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int frameDelay;
    int frameCounter;
};

