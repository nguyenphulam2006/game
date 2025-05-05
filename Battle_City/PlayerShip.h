#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include <SDL.h>
#include <SDL_image.h>
#include "PlayerBullet.h"
#include <vector>
#include <SDL_mixer.h>

class PlayerShip {
public:
    PlayerShip(SDL_Renderer* renderer, SDL_Texture* bulletTex, int x, int y);
    ~PlayerShip();
    void HandleInput(const Uint8* keyState);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    void Shoot(std::vector<PlayerBullet*>& bullets);
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect GetRect() const { return rect; }

private:
    float speed;
    Uint32 lastShotTime = 0;
    Mix_Chunk* shootSound;
    SDL_Renderer* rendererRef;
    SDL_Texture* bulletTextureRef;
};

#endif
