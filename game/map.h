#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <SDL_image.h>

class Map {
public:
    Map(SDL_Renderer* renderer, const char* path);
    ~Map();
    void render();

    void setCamera(int x, int y);

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Rect destRect;
    SDL_Rect camera;
    int mapWidth, mapHeight;
    SDL_Texture* loadTexture(const char* path);
};

#endif
