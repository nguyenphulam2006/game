#include "map.h"
#include <iostream>

Map::Map(SDL_Renderer* renderer, const char* path) {
    this->renderer = renderer;
    texture = loadTexture(path);

    destRect = {0, 0, 800, 600};
}

Map::~Map() {
    SDL_DestroyTexture(texture);
}

SDL_Texture* Map::loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) return nullptr;
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void Map::render() {
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}
