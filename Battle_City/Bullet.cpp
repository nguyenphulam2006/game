#include "Bullet.h"

Bullet::Bullet(int x, int y, int dx, int dy) : dx(dx), dy(dy) {
    rect = {x, y, 5, 10};
}

void Bullet::move() {
    rect.x += dx;
    rect.y += dy;
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
