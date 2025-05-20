#include "enemy.h"
#include "level.h"

std::vector<Enemy> Enemies::enemies;
Enemies::Enemies() {}

void Enemies::spawn_enemies() {
    Enemies::enemies.clear();
    for (size_t row = 0; row < Level::current_level.rows; ++row) {
        for (size_t column = 0; column < Level::current_level.columns; ++column) {
            char cell = Level::get_level_cell(row, column);
            if (cell == ENEMY) {
                Enemies::enemies.push_back({{static_cast<float>(column), static_cast<float>(row)}, true});
                Level::set_level_cell(row, column, AIR);
            }
        }
    }
}

void Enemies::update_enemies() {
    for (auto &enemy:  Enemies::enemies) {
        float next_x = enemy.pos.x + (enemy.is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);
        if (Level::is_colliding({next_x, enemy.pos.y}, WALL)) {
            enemy.is_looking_right = !enemy.is_looking_right;
        } else {
            enemy.pos.x = next_x;
        }
    }
}

bool Enemies::is_colliding_with_enemies(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (const auto &enemy:  Enemies::enemies) {
        Rectangle enemy_hitbox = {enemy.pos.x, enemy.pos.y, 1.0f, 1.0f};
        if (CheckCollisionRecs(entity_hitbox, enemy_hitbox)) {
            return true;
        }
    }
    return false;
}

void Enemies::remove_colliding_enemy(Vector2 pos) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    Enemies::enemies.erase(std::remove_if( Enemies::enemies.begin(),  Enemies::enemies.end(),
                                 [&](const Enemy &enemy) {
                                     Rectangle enemy_hitbox = {enemy.pos.x, enemy.pos.y, 1.0f, 1.0f};
                                     return CheckCollisionRecs(entity_hitbox, enemy_hitbox);
                                 }),
                           Enemies::enemies.end());
}
void Enemies::draw_enemies() {
    for (auto &enemy:  Enemies::enemies) {
        horizontal_shift = (screen_size.x - cell_size) / 2;

        Vector2 pos = {
                (enemy.pos.x - player_pos.x) * cell_size + horizontal_shift,
                enemy.pos.y * cell_size
        };

        draw_sprite(enemy_walk, pos, cell_size);
    }
}
