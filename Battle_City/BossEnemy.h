#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "EnemyTank.h"
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include "BossBullet.h"
extern std::vector<BossBullet*> gBossBullets;
extern SDL_Texture* gBossBulletTexture;

class BossEnemy : public EnemyTank {
public:
    BossEnemy(SDL_Renderer* renderer, int x, int y, int level);
    ~BossEnemy();
    void update();
    void render(SDL_Renderer* renderer);
    std::vector<BossBullet*>& getBossBullets() { return gBossBullets; }
    void takeDamage(int damage);
    bool isDestroyed() const;

private:
    void shootCircularBullets();
    int health;
    int moveDirection;
    float moveSpeed;
    Uint32 shootCooldown;
    Uint32 lastShootTime;
};

#endif
