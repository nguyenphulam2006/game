#include "PlayerTank.h"
#include <iostream>
#include <algorithm>

PlayerTank::PlayerTank(SDL_Renderer* renderer, int x, int y): renderer(renderer)  {
   rect = {x, y, 50, 50};
    speed = 5;
    SDL_Surface* surface = IMG_Load("assets/player_tank.png");
    if (!surface) std::cout << "Failed to load ship: " << IMG_GetError() << std::endl;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

PlayerTank::~PlayerTank() {
    SDL_DestroyTexture(texture);
}

void PlayerTank::handleInput(const Uint8* keystate) {
   if (keystate[SDL_SCANCODE_LEFT]) rect.x -= speed;
    if (keystate[SDL_SCANCODE_RIGHT]) rect.x += speed;
    if (keystate[SDL_SCANCODE_SPACE]) bullets.emplace_back(rect.x + 20, rect.y, 0, -10);
}
void PlayerTank::update() {
    for (auto& b : bullets) b.move();
}
void PlayerTank::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    for (auto& b : bullets) b.render(renderer);
}

