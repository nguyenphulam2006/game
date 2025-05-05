#include "GameMenu.h"
#include <iostream>

GameMenu::GameMenu(SDL_Renderer* renderer) : renderer(renderer), selectedIndex(0) {
    TTF_Init();
    font = TTF_OpenFont("assets/font_01.ttf", 36);

    options = {"Play", "Option", "Exit"};
}

GameMenu::~GameMenu() {
    TTF_CloseFont(font);
    TTF_Quit();
}

void GameMenu::render() {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Battle City", white);
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    SDL_Rect titleRect = {250, 100, 300, 50};
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);

   for (size_t i = 0; i < options.size(); i++){
        SDL_Color color = (i == selectedIndex) ? SDL_Color{255, 0, 0, 255} : white;
        SDL_Surface* optionSurface = TTF_RenderText_Solid(font, options[i].c_str(), color);
        SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(renderer, optionSurface);
        SDL_FreeSurface(optionSurface);

        SDL_Rect optionRect = {300, 200 + i * 60, 200, 40};
        SDL_RenderCopy(renderer, optionTexture, nullptr, &optionRect);
        SDL_DestroyTexture(optionTexture);
    }
}

void GameMenu::handleEvents(bool &running, bool &startGame) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_DOWN) {
                selectedIndex = (selectedIndex + 1) % options.size();
            } else if (e.key.keysym.sym == SDLK_UP) {
                selectedIndex = (selectedIndex - 1 + options.size()) % options.size();
            } else if (e.key.keysym.sym == SDLK_RETURN) {
                if (selectedIndex == 0) startGame = true;
                if (selectedIndex == 2) running = false;
            }
        }
    }
}


