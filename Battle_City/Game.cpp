#include "Game.h"
#include "PlayerShip.h"
#include "ChickenEnemy.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include "Explosion.h"
#include "GameMenu.h"
#include "BossEnemy.h"
std::vector<Explosion*> explosions;
std::vector<PlayerBullet*> playerBullets;
std::vector<EnemyBullet*> enemyBullets;

enum class GameState {
    MENU,
    PLAYING,
    LOST,
    WON
};

Game::Game() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    running = true;
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");
    if (!bgSurface) {
        SDL_Log("Failed to load background: %s", IMG_GetError());
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    enemyBulletTexture = IMG_LoadTexture(renderer, "assets/bullet.png");
    if (!enemyBulletTexture) {
        SDL_Log("Failed to load enemy bullet texture: %s", IMG_GetError());
    }

    playerBulletTexture = IMG_LoadTexture(renderer, "assets/flame.png");
    if (!playerBulletTexture) {
        SDL_Log("Failed to load player bullet texture: %s", IMG_GetError());
    }

    gBossBulletTexture = IMG_LoadTexture(renderer, "assets/boss_bullet.png");
    if (!gBossBulletTexture) {
        SDL_Log("Failed to load boss bullet texture: %s", IMG_GetError());
    }

    explosionTexture = IMG_LoadTexture(renderer, "assets/explosion.png");
    if (!explosionTexture) {
        SDL_Log("Failed to load explosion texture: %s", IMG_GetError());
    }

    font = TTF_OpenFont("assets/font_01.ttf", 36);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }
    player = new PlayerShip(renderer, playerBulletTexture, 400, 500);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    bgMusic = Mix_LoadMUS("assets/Come along with me.wav");
    if (bgMusic) Mix_PlayMusic(bgMusic, -1);
    shootSound = Mix_LoadWAV("assets/shoot.wav");
    explosionSound = Mix_LoadWAV("assets/explosion.wav");
    lastSpawnTime = SDL_GetTicks();
    lastTime = SDL_GetTicks();
    score = 0;
    nextBossScore = 1000;
    enemyKillCount = 0;
    bossActive = false;
    boss = nullptr;
    gameState = GameState::MENU;
}

Game::~Game() {
    delete player;
    if (boss) delete boss;
    for (auto b : playerBullets) delete b;
    for (auto b : enemyBullets) delete b;
    for (auto e : explosions) delete e;

    SDL_DestroyTexture(enemyBulletTexture);
    SDL_DestroyTexture(playerBulletTexture);
    SDL_DestroyTexture(gBossBulletTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(explosionSound);
    Mix_FreeMusic(bgMusic);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    GameMenu menu(renderer);
    bool startGame = false;

    while (running) {
        switch (gameState) {
            case GameState::MENU:
                menu.handleEvents(running, startGame);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                menu.render();
                SDL_RenderPresent(renderer);
                if (startGame) {
                    reset();
                    gameState = GameState::PLAYING;
                    startGame = false;
                }
                break;

            case GameState::PLAYING:
                handleEvents();
                update();
                spawnEnemy();
                render();
                if (gameLost) {
                    gameState = GameState::LOST;
                } else if (gameWon) {
                    gameState = GameState::WON;
                }
                SDL_Delay(16);
                break;

            case GameState::LOST:
            case GameState::WON:
                handleEndScreen();
                SDL_Delay(16);
                break;
        }
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            gameState = GameState::MENU;
            return;
        }
    }

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    player->HandleInput(keystate);

    static Uint32 lastShotTime = 0;
    Uint32 now = SDL_GetTicks();
    if (keystate[SDL_SCANCODE_SPACE] && now - lastShotTime > 250) {
        player->Shoot(playerBullets);
        lastShotTime = now;
    }
}

void Game::update() {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    player->Update(deltaTime);
    for (auto it = playerBullets.begin(); it != playerBullets.end();) {
        (*it)->Update(deltaTime);
        if (!(*it)->IsActive()) {
            delete *it;
            it = playerBullets.erase(it);
        } else ++it;
    }
    updateEnemies();
    for (auto it = playerBullets.begin(); it != playerBullets.end();) {
        SDL_Rect bulletRect = (*it)->GetRect();
        bool removed = false;
        for (auto& enemy : enemies) {
            SDL_Rect enemyRect = enemy->getRect();
            if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                explosions.push_back(new Explosion(explosionTexture, enemyRect.x, enemyRect.y));
                Mix_PlayChannel(-1, explosionSound, 0);
                enemy->destroy();
                enemyKillCount++;
                score += 100;
                delete *it;
                it = playerBullets.erase(it);
                removed = true;
                break;
            }
        }
        if (!removed) ++it;
    }
    for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
        (*it)->move();
        if ((*it)->isOffScreen()) {
            delete *it;
            it = enemyBullets.erase(it);
        } else ++it;
    }
    SDL_Rect playerRect = player->GetRect();
    for (auto& enemy : enemies) {
        for (auto& b : enemy->getBullets()) {
            SDL_Rect bRect = b->getRect();
            if (SDL_HasIntersection(&playerRect, &bRect)) {
                playerHit = true;
                damageStartTime = SDL_GetTicks();
                gameLost = true;
            }
        }
    }
    if (score >= nextBossScore && !bossActive) {
        boss = new BossEnemy(renderer, 350, 100, 1);
        bossActive = true;
        nextBossScore += 1000;
    }
    if (bossActive && boss) {
        boss->update();
        auto& bBullets = boss->getBossBullets();
        for (auto it = bBullets.begin(); it != bBullets.end();) {
            (*it)->move();
            SDL_Rect bRect = (*it)->getRect();
            if ((*it)->isOffScreen()) {
                delete *it;
                it = bBullets.erase(it);
            }
            else if (SDL_HasIntersection(&playerRect, &bRect)) {
                playerHit = true;
                gameLost = true;
                damageStartTime = SDL_GetTicks();
                delete *it;
                it = bBullets.erase(it);
                break;
            }
            else ++it;
        }
        for (auto it = playerBullets.begin(); it != playerBullets.end();) {
            SDL_Rect pRect = (*it)->GetRect();
            SDL_Rect bossRect = boss->getRect();
            if (SDL_HasIntersection(&pRect, &bossRect)) {
                boss->takeDamage(1);
                explosions.push_back(new Explosion(explosionTexture, bossRect.x, bossRect.y));
                Mix_PlayChannel(-1, explosionSound, 0);
                delete *it;
                it = playerBullets.erase(it);
            } else ++it;
        }
        if (boss->isDestroyed()) {
            delete boss;
            boss = nullptr;
            bossActive = false;
            score += 500;
        }
    }

    removeDeadEnemies();
    for (auto it = explosions.begin(); it != explosions.end();) {
        (*it)->update();
        if (!(*it)->isAlive()) {
            delete *it;
            it = explosions.erase(it);
        } else ++it;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    player->Render(renderer);
    for (auto& b : playerBullets) b->Render(renderer);
    for (auto& e : explosions) if (e->isAlive()) e->render(renderer);
    renderEnemies();
    if (bossActive && boss) {
        boss->render(renderer);
        for (auto& b : boss->getBossBullets()) {
            b->render(renderer, gBossBulletTexture);
        }
    }
    SDL_Color white = {255,255,255,255};
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surf = TTF_RenderText_Blended(font, scoreText.c_str(), white);
    SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect textRect = {10,10, surf->w, surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer, textTex, nullptr, &textRect);
    SDL_DestroyTexture(textTex);

    SDL_RenderPresent(renderer);
}

void Game::handleEndScreen() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_r) {
                reset();
                gameState = GameState::PLAYING;
            } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                gameState = GameState::MENU;
                running = true;
            }
        }
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_Color white = {255, 255, 255, 255};
    std::string msg = (gameState == GameState::LOST) ? "YOU DIE" : "YOU WIN";
    SDL_Color color = (gameState == GameState::LOST) ? SDL_Color{255, 0, 0, 255}
                                                    : SDL_Color{255, 255, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, msg.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    int textWidth = surf->w;
    int textHeight = surf->h;
    SDL_Rect r = { (800 - textWidth) / 2, 200, textWidth, textHeight };  // Căn giữa theo chiều ngang và đặt ở giữa chiều dọc
    SDL_RenderCopy(renderer, tex, nullptr, &r);
    SDL_DestroyTexture(tex);
    SDL_Surface* s1 = TTF_RenderText_Blended(font, "Press R to Restart", white);
    SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
    SDL_FreeSurface(s1);
    SDL_Rect r1 = { (800 - s1->w) / 2, 300, s1->w, s1->h };  // Căn giữa
    SDL_RenderCopy(renderer, t1, nullptr, &r1);
    SDL_DestroyTexture(t1);

    SDL_Surface* s2 = TTF_RenderText_Blended(font, "Press ESC to Exit", white);
    SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
    SDL_FreeSurface(s2);
    SDL_Rect r2 = { (800 - s2->w) / 2, 370, s2->w, s2->h };  // Căn giữa
    SDL_RenderCopy(renderer, t2, nullptr, &r2);
    SDL_DestroyTexture(t2);

    SDL_RenderPresent(renderer);
}

void Game::reset() {
    delete player;
    player = new PlayerShip(renderer, playerBulletTexture, 400,500);

    for (auto& b : playerBullets) delete b;
    playerBullets.clear();
    for (auto& b : enemyBullets) delete b;
    enemyBullets.clear();
    for (auto& e : explosions) delete e;
    explosions.clear();
    enemies.clear();

    score = 0;
    enemyKillCount = 0;
    nextBossScore = 1000;

    if (boss) {
        delete boss;
        boss = nullptr;
    }
    bossActive = false;
    gameLost = false;
    gameWon = false;
}

void Game::spawnEnemy() {
    if (enemies.size() >= 4) return;
    Uint32 now = SDL_GetTicks();
    if (now - lastSpawnTime > 2000) {
        int x = rand() % 700 + 50;
        enemies.push_back(std::make_unique<ChickenEnemy>(renderer, x, 50));
        lastSpawnTime = now;
    }
}

void Game::updateEnemies() {
    for (auto& e : enemies) e->update();
}

void Game::renderEnemies() {
    for (auto& e : enemies) {
        ChickenEnemy* c = dynamic_cast<ChickenEnemy*>(e.get());
        if (c) c->render(renderer, enemyBulletTexture);
        else e->render(renderer);
    }
}

void Game::removeDeadEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<EnemyTank>& e){ return e->isDestroyed(); }), enemies.end());
}
