#include "EnemyTank.h"

EnemyTank::EnemyTank(SDL_Renderer* renderer, int x, int y)
    : renderer(renderer) {
    rect = { x, y, 50, 50 };
    loadTexture("assets/enemy.png");
}

EnemyTank::~EnemyTank() {
    if (texture) SDL_DestroyTexture(texture);
}

void EnemyTank::update() {}
void EnemyTank::shoot() {
    bullets.push_back(new EnemyBullet(rect.x + rect.w / 2, rect.y));
}
void EnemyTank::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        if (texture) {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void EnemyTank::loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load("assets/enemy.png");
if (!surface) {
    std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
}

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}
