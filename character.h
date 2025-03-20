#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <SDL_image.h>

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleEvent(SDL_Event& e);
    void update();
    void render();

    int getX() { return x; }
    int getY() { return y; }

private:
    SDL_Renderer* renderer;
    SDL_Texture* walkTexture;
    SDL_Texture* jumpTexture;

    int x, y;
    int speedX, speedY;
    bool isJumping;
    bool isFalling;
    bool isMoving;
    bool facingRight;
    const int JUMP_FORCE = -10;
    const int GRAVITY = 1;
    const int SPEED = 4;
    const int MAP_WIDTH = 800;

    SDL_Rect currentFrame;
    int frameIndex;
    int frameTime;

    SDL_Texture* loadTexture(const char* path);
};

#endif
