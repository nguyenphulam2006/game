#include "EnemyBullet.h"

EnemyBullet::EnemyBullet(int x, int y) {
    rect.x = x;
    rect.y = y;
    rect.w = 10;
    rect.h = 20;
}

void EnemyBullet::move() {
    rect.y += 5;
}

void EnemyBullet::render(SDL_Renderer* renderer, SDL_Texture* bulletTexture) {
    SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
}
