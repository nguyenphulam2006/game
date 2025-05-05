#include "Explosion.h"

Explosion::Explosion(SDL_Texture* texture, int x, int y)
    : texture(texture), frame(0), frameDelay(5), frameCounter(0) {
    SDL_QueryTexture(texture, NULL, NULL, &frameWidth, &frameHeight);
    totalFrames = frameWidth / frameHeight;
    frameWidth = frameHeight;

    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {x, y, frameWidth, frameHeight};
}

void Explosion::update() {
    frameCounter++;
    if (frameCounter >= frameDelay) {
        frame++;
        frameCounter = 0;
        if (frame < totalFrames) {
            srcRect.x = frame * frameWidth;
        }
    }
}

void Explosion::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

bool Explosion::isFinished() const {
    return frame >= totalFrames;
}

