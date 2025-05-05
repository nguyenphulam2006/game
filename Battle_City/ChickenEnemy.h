#ifndef CHICKEN_ENEMY_H
#define CHICKEN_ENEMY_H

#include "EnemyTank.h"
#include "EnemyBullet.h"
#include <vector>

class ChickenEnemy : public EnemyTank {
public:
    std::vector<EnemyBullet*>& getBullets() override;
    ChickenEnemy(SDL_Renderer* renderer, int x, int y);
    void update() override;
    void render(SDL_Renderer* renderer, SDL_Texture* bulletTexture);
    void shoot();

private:
    std::vector<EnemyBullet*> bullets;
    int moveDir = 1;
    Uint32 lastShootTime;
    Uint32 shootCooldown;
};

#endif
