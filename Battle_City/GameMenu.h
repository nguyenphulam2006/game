#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>

class GameMenu {
public:
    GameMenu(SDL_Renderer* renderer);
    ~GameMenu();
    void render();
    void handleEvents(bool &running, bool &startGame);

private:
    SDL_Renderer* renderer;
    SDL_Texture* titleTexture;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    std::vector<std::string> options;
    int selectedIndex;
};

