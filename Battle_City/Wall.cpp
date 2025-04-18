#include "Wall.h"

Wall::Wall(int x, int y) {
    rect = { x, y, 40, 40 };
}

void Wall::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // nâu
    SDL_RenderFillRect(renderer, &rect);
}
