#include "Game.h"
#include <iostream>

Game::Game() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    running = true;

    player = new PlayerTank(renderer, 400, 500);
    spawnEnemies();
}

Game::~Game() {
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
    }
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->handleInput(keystate);
}

void Game::update() {
    player->update();
    for (auto& c : enemies) c.update();
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    player->render(renderer);
    for (auto& c : enemies) c.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::spawnEnemies() {
    for (int i = 0; i < 5; i++) {
        enemies.emplace_back(renderer, 100 + i * 120, 50);
    }
}
