#include "EnemyTank.h"
#include <iostream>

EnemyTank :: EnemyTank (SDL_Renderer* renderer, int x, int y) {
    rect = {x, y, 40, 40};
    SDL_Surface* surface = IMG_Load("assets/enemy.png");
    if (!surface) std::cout << "Failed to load: " << IMG_GetError() << std::endl;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

EnemyTank::~EnemyTank() {
    SDL_DestroyTexture(texture);
}

void EnemyTank::update() {
    rect.y += 1; // gà rơi dần xuống
}

void EnemyTank::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
