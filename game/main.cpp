#include <SDL.h>
#include <SDL_image.h>
#include "character.h"
#include "map.h"
#include "Obstacle.h"
#include <vector>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    if (a.x + a.w <= b.x || a.x >= b.x + b.w ||
        a.y + a.h <= b.y || a.y >= b.y + b.h) {
        return false;
    }
    return true;
}
bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    return true;
}

void close(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(window, renderer)) return -1;

    Map map(renderer, "asset/map.png");
    Character player(renderer);
    std::vector<Obstacle*> obstacles;
    obstacles.push_back(new Obstacle(renderer, "asset/obstacle.png", 400, 400, 50, 50));
    obstacles.push_back(new Obstacle(renderer, "asset/obstacle.png", 600, 350, 50, 50));

    int score = 0;
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            player.handleEvent(e);
        }
        player.update();
        int camX = player.getX() + player.getWidth() / 2 - SCREEN_WIDTH / 2;
        int camY = player.getY() + player.getHeight() / 2 - SCREEN_HEIGHT / 2;
        map.setCamera(camX, camY);
        SDL_Rect playerRect = { player.getX(), player.getY(), player.getWidth(), player.getHeight() };
        for (auto& obs : obstacles) {
            if (checkCollision(playerRect, obs->getRect())) {
                score += 10;
                std::cout << "Va chạm! Điểm: " << score << std::endl;
            }
        }

        SDL_RenderClear(renderer);
        map.render();
        for (auto& obs : obstacles) {
            obs->render();
        }
        player.render();
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
    for (auto& obs : obstacles) {
        delete obs;
    }

    close(window, renderer);
    return 0;
}
