#include "character.h"
#include <iostream>

Character::Character(SDL_Renderer* renderer) {
    this->renderer = renderer;
    walkTexture = loadTexture("asset/walk.png");
    jumpTexture = loadTexture("asset/jump.png");

    x = 200;
    y = 390;
    speedX = 0;
    speedY = 0;
    isJumping = false;
    isFalling = false;
    isMoving = false;
    facingRight = true;
    frameIndex = 0;
    frameTime = 0;
    currentFrame = {0, 0, 30, 30};
}

Character::~Character() {
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyTexture(jumpTexture);
}

SDL_Texture* Character::loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Character::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                speedX = -SPEED;
                isMoving = true;
                facingRight = false;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                speedX = SPEED;
                isMoving = true;
                facingRight = true;
                break;
            case SDLK_SPACE:
            case SDLK_w:
            case SDLK_UP:
                if (!isJumping) {
                    isJumping = true;
                    speedY = JUMP_FORCE;
                }
                break;
        }
    } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a ||
            e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
            speedX = 0;
            isMoving = false;
        }
    }
}

void Character::update() {
    x += speedX;
    y += speedY;

    if (isJumping) {
        speedY += GRAVITY;
        if (y >= 390) {
            y = 390;
            speedY = 0;
            isJumping = false;
        }
    }

    frameTime++;
    if (frameTime >= 10) {
        frameIndex = (frameIndex +1 ) % 2;
        currentFrame.x = frameIndex * 30;
        frameTime = 0;
    }
}
void Character::render() {
    SDL_Rect dest = { x, y, 60, 60 };
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_Texture* currentTexture = isJumping ? jumpTexture : walkTexture;
    SDL_RenderCopyEx(renderer, currentTexture, &currentFrame, &dest, 0, nullptr, flip);
}
