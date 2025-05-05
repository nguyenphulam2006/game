#include "PlayerBullet.h"
#include <iostream>
PlayerBullet::PlayerBullet(int x, int y, SDL_Renderer* renderer, SDL_Texture* tex,
                           int fw, int fh, int fc, float animSpeed)
    : texture(tex),
      frameWidth(fw),
      frameHeight(fh),
      frameCount(fc),
      currentFrame(0),
      animationSpeed(animSpeed),
      timeSinceLastFrameChange(0.0f),
      active(true)
{
    rect = {x, y, frameWidth, frameHeight};
}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Update(float deltaTime) {
    rect.y -= 5;
    timeSinceLastFrameChange += deltaTime;
    if (timeSinceLastFrameChange >= 1.0f / animationSpeed) {
        currentFrame++;
        if (currentFrame >= frameCount) {
            currentFrame = 0;
        }
        timeSinceLastFrameChange = 0.0f;
    }
    if (rect.y + rect.h < 0) {
        active = false;
    }
}

void PlayerBullet::Render(SDL_Renderer* renderer) {
    if (!active || !texture) return;
    SDL_Rect srcRect;
    srcRect.x = currentFrame * frameWidth;
    srcRect.y = 0;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;
    SDL_RenderCopy(renderer, texture, &srcRect, &rect);
}
