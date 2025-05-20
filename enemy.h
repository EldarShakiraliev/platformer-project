#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"
#include <vector>

struct Enemy {
    Vector2 pos;
    bool is_looking_right;
};

class Enemies {
public:
    static void spawn_enemies();
    static void update_enemies();
    static bool is_colliding_with_enemies(Vector2 pos);
    static void remove_colliding_enemy(Vector2 pos);
    static std::vector<Enemy> enemies;
    static void draw_enemies();
private:
    Enemies();
};

#endif // ENEMY_H