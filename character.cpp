#include "character.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer) {
    this->renderer = renderer;
    walkTexture = loadTexture("walk.png");
    jumpTexture = loadTexture("jump.png");

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

Player::~Player() {
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyTexture(jumpTexture);
}

SDL_Texture* Player::loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) return nullptr;
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Player::handleEvent(SDL_Event& e) {
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
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
            case SDLK_a:
            case SDLK_d:
                speedX = 0;
                isMoving = false;
                break;
        }
    }
}

void Player::update() {
    x += speedX;
    if (x < 0) x = 0;
    if (x > MAP_WIDTH - 64) x = MAP_WIDTH - 64;
    if (isJumping) {
        y += speedY;
        speedY += GRAVITY;

        if (speedY > 5) isFalling = true;
    }

    if (y >= 390) {
        y = 390;
        isJumping = false;
        isFalling = false;
        speedY = 0;
    }

    frameTime++;
    if (frameTime >= 5) {
        frameIndex++;
        if (frameIndex >= 4) frameIndex = 0;
        frameTime = 0;
        currentFrame.x = frameIndex * 64;
    }
}

void Player::render() {
    SDL_Rect destRect = {x, y, 64, 64};
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    if (isJumping || isFalling) {
        SDL_RenderCopyEx(renderer, jumpTexture, &currentFrame, &destRect, 0, NULL, flip);
    } else {
        SDL_RenderCopyEx(renderer, walkTexture, &currentFrame, &destRect, 0, NULL, flip);
    }
}

