#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"
#include <vector>

struct Enemy {
    Vector2 pos;
    bool is_looking_right;
};

inline std::vector<Enemy> enemies;
class Enemies {
public:
    Enemies();
    static void spawn_enemies();
    static void update_enemies();
    static bool is_colliding_with_enemies(Vector2 pos);
    static void remove_colliding_enemy(Vector2 pos);
};

#endif // ENEMY_H