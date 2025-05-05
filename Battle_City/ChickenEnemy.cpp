#include "ChickenEnemy.h"
#include <algorithm>
ChickenEnemy::ChickenEnemy(SDL_Renderer* renderer, int x, int y)
    : EnemyTank(renderer, x, y) {
    lastShootTime = SDL_GetTicks();
    shootCooldown = 300 + rand() % 500;
}
std::vector<EnemyBullet*>& ChickenEnemy::getBullets() {
    return bullets;
}
void ChickenEnemy::update() {
    EnemyTank::update();
    rect.x += moveDir * 5;
    if (rect.x <= 0 || rect.x + rect.w >= 800) {
        moveDir *= -1;
    }

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastShootTime >= shootCooldown) {
        shoot();
        lastShootTime = currentTime;
        shootCooldown = 300 + rand() % 500;
    }

    for (auto& bullet : bullets) {
        bullet->move();
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](EnemyBullet* bullet) {
            return bullet->getRect().y > 600;
        }), bullets.end());
}


void ChickenEnemy::render(SDL_Renderer* renderer, SDL_Texture* bulletTexture) {
    EnemyTank::render(renderer);
    for (auto& bullet : bullets) {
        bullet->render(renderer, bulletTexture);
    }
}

void ChickenEnemy::shoot() {
    bullets.push_back(new EnemyBullet(getRect().x + getRect().w / 2 - 2, getRect().y + getRect().h));
}
