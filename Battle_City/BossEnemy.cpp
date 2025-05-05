#include "BossEnemy.h"
std::vector<BossBullet*> gBossBullets;
SDL_Texture* gBossBulletTexture = nullptr;

BossEnemy::BossEnemy(SDL_Renderer* renderer, int x, int y, int level)
    : EnemyTank(renderer, x, y), health(5), moveDirection(1), moveSpeed(4.0f),
      shootCooldown(2000), lastShootTime(0) {
    loadTexture("assets/boss.png");
    rect.w = 80;
    rect.h = 80;
}

BossEnemy::~BossEnemy() {
    for (auto bullet : gBossBullets) {
        delete bullet;
    }
    gBossBullets.clear();
}

void BossEnemy::update() {
    Uint32 now = SDL_GetTicks();
    rect.x += static_cast<int>(moveSpeed * moveDirection);
    if (rect.x <= 0) {
        rect.x = 0;
        moveDirection = 1;
    } else if (rect.x + rect.w >= 800) {
        rect.x = 800 - rect.w;
        moveDirection = -1;
    }
    if (now - lastShootTime >= shootCooldown) {
        shootCircularBullets();
        lastShootTime = now;
    }
    for (auto bullet : gBossBullets) {
        bullet->move();
    }
}

void BossEnemy::render(SDL_Renderer* renderer) {
    if (!destroyed) {
        if (texture) {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    for (auto bullet : gBossBullets) {
        bullet->render(renderer, gBossBulletTexture);
    }
}

void BossEnemy::shootCircularBullets() {
    const int bulletCount = 12;
    float angleStep = 2 * M_PI / bulletCount;

    for (int i = 0; i < bulletCount; ++i) {
        float angle = i * angleStep;
        float dx = cos(angle);
        float dy = sin(angle);
        int startX = rect.x + rect.w / 2;
        int startY = rect.y + rect.h / 2;
        BossBullet* bullet = new BossBullet(startX, startY, dx, dy);
        gBossBullets.push_back(bullet);
    }
}

void BossEnemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        destroyed = true;
    }
}

bool BossEnemy::isDestroyed() const {
    return destroyed;
}
