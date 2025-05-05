#include "PlayerShip.h"
#include "PlayerBullet.h"
#include <iostream>

PlayerShip::PlayerShip(SDL_Renderer* renderer, SDL_Texture* bulletTex, int x, int y)
    : texture(nullptr),
      rendererRef(renderer),
      speed(5.0f),
      bulletTextureRef(bulletTex),
      lastShotTime(0),
      shootSound(nullptr)
{
    rect = {x, y, 50, 50};

    SDL_Surface* surface = IMG_Load("assets/player_ship.png");
    if (!surface) {
        std::cout << "Failed to load player ship: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(rendererRef, surface);
        SDL_FreeSurface(surface);
    }
}

PlayerShip::~PlayerShip() {
    SDL_DestroyTexture(texture);
    Mix_FreeChunk(shootSound);
}

void PlayerShip::Shoot(std::vector<PlayerBullet*>& bullets) {
    int totalWidth = 0;
    int totalHeight = 0;
    SDL_QueryTexture(bulletTextureRef, nullptr, nullptr, &totalWidth, &totalHeight);

    const int FRAME_COUNT = 4;
    int frameWidth = totalWidth / FRAME_COUNT;
    int frameHeight = totalHeight;
    float animationSpeed = 10.0f;

    int bulletX = rect.x + rect.w / 2 - frameWidth / 2;
    int bulletY = rect.y;

    bullets.push_back(new PlayerBullet(bulletX, bulletY, rendererRef, bulletTextureRef,
                                       frameWidth, frameHeight, FRAME_COUNT, animationSpeed));

    if (shootSound) {
       Mix_PlayChannel(-1, shootSound, 0);
    }
}

void PlayerShip::HandleInput(const Uint8* keyState) {
    if (keyState[SDL_SCANCODE_A]) {
        rect.x -= speed;
    }
    if (keyState[SDL_SCANCODE_D]) {
        rect.x += speed;
    }
}

void PlayerShip::Update(float deltaTime) {
    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > 800) rect.x = 800 - rect.w;
}

void PlayerShip::Render(SDL_Renderer* renderer) {
     if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
     }
}
