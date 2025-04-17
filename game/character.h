#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();
 int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return 60; }
    int getHeight() const { return 60; }
    void handleEvent(SDL_Event& e);
    void update();
    void render();

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

    const int SPEED = 5;
    const int JUMP_FORCE = -15;
    const int GRAVITY = 1;

    int frameIndex;
    int frameTime;
    SDL_Rect currentFrame;

    SDL_Texture* loadTexture(const char* path);
};

#endif
