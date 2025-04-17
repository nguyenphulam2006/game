#include "map.h"
#include <iostream>

Map::Map(SDL_Renderer* renderer, const char* path) {
    this->renderer = renderer;
    texture = loadTexture(path);
    if (!texture) {
        std::cerr << "Không tải được map từ: " << path << std::endl;
        return;
    }
    SDL_QueryTexture(texture, NULL, NULL, &mapWidth, &mapHeight);
    destRect = { 0, 0, 800, 600 };
    camera = { 0, 0, destRect.w, destRect.h };
}

Map::~Map() {
    SDL_DestroyTexture(texture);
}

SDL_Texture* Map::loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}
void Map::render() {
    SDL_RenderCopy(renderer, texture, &camera, &destRect);
}
void Map::setCamera(int x, int y) {
    camera.x = x;
    camera.y = y;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > mapWidth - camera.w) camera.x = mapWidth - camera.w;
    if (camera.y > mapHeight - camera.h) camera.y = mapHeight - camera.h;
}
