#include "BossBullet.h"

BossBullet::BossBullet(int x, int y, float dx, float dy)
    : dx(dx), dy(dy), speed(5.0f)
{
    rect.x = x;
    rect.y = y;
    rect.w = 16;
    rect.h = 16;
}

void BossBullet::move() {
    rect.x += static_cast<int>(dx * speed);
    rect.y += static_cast<int>(dy * speed);
}

void BossBullet::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

SDL_Rect BossBullet::getRect() const {
    return rect;
}

bool BossBullet::isOffScreen() const {
    return rect.y < 0 || rect.y > 600 || rect.x < 0 || rect.x > 800;
}
