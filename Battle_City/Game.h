#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "PlayerTank.h"
#include "EnemyTank.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    PlayerTank* player;
    std::vector<EnemyTank> enemies;

    void handleEvents();
    void update();
    void render();
    void spawnEnemies();
};

#endif
