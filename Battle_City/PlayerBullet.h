#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include <SDL.h>
#include <SDL_image.h>

class PlayerBullet {
public:
    PlayerBullet(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture,
                 int frameWidth, int frameHeight, int frameCount, float animationSpeed);

    ~PlayerBullet();

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() const { return rect; }
    bool IsActive() const { return active; }
private:
    SDL_Texture* texture;
    SDL_Rect rect;
    int frameWidth;
    int frameHeight;
    int frameCount;
    int currentFrame;
    float animationSpeed;
    float timeSinceLastFrameChange;
    bool active;
};

#endif
