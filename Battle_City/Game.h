#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <memory>
#include <string>
#include <SDL_mixer.h>
#include "PlayerShip.h"
#include "EnemyTank.h"
#include "BossEnemy.h"
class Game {
public:
    Uint32 lastTime = 0;
    Uint32 lastClearTime = 0;

    Game();
    ~Game();
    void run();
    void spawnEnemy();
    void updateEnemies();
    void renderEnemies();
    void removeDeadEnemies();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* enemyBulletTexture;
    SDL_Texture* explosionTexture;
    SDL_Texture* playerBulletTexture;
    TTF_Font* font;
    Mix_Music* bgMusic;
    Mix_Chunk* shootSound = nullptr;
    Mix_Chunk* explosionSound = nullptr;
    bool running;
    PlayerShip* player;
    std::vector<std::unique_ptr<EnemyTank>> enemies;
    std::vector<EnemyBullet*> enemyBullets;
    int enemyKillCount = 0;
    Uint32 lastSpawnTime = 0;
    Uint32 damageStartTime = 0;
    int score;
    bool gameWon = false;
    bool gameLost = false;
    Uint32 winTime = 0;
    Uint32 loseTime = 0;
    bool playerHit = false;
    BossEnemy* boss = nullptr;
    int nextBossScore = 1000;
    bool bossActive = false;
    enum class GameState {
    MENU,
    PLAYING,
    LOST,
    WON,
};

    GameState gameState = GameState::PLAYING;

    void handleEvents();
    void update();
    void render();
    void spawnEnemies();
    void reset();
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, SDL_Color color, int x, int y);
    void handleEndScreen();
};

#endif
