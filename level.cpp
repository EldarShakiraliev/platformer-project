#include "level.h"
#include "raylib.h"

Level::Level() : current_level_data(nullptr) {}

Level::~Level() {
    unload_level();
}

void Level::reset_level_index() {
    level_index = 0;
}

void Level::load_level(int offset) {
    level_index += offset;

    if (level_index >= LEVEL_COUNT) {
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
        return;
    }

    size_t rows = LEVELS[level_index].rows;
    size_t columns = LEVELS[level_index].columns;
    current_level_data = new char[rows * columns];

    for (size_t row = 0; row < rows; row++) {
        for (size_t column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = LEVELS[level_index].data[row * columns + column];
        }
    }

    current_level = {rows, columns, current_level_data};
    spawn_player();
    spawn_enemies();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void Level::unload_level() {
    delete[] current_level_data;
    current_level_data = nullptr;
}

bool Level::is_inside_level(int row, int column) const {
    if (row < 0 || row >= current_level.rows) return false;
    if (column < 0 || column >= current_level.columns) return false;
    return true;
}

char& Level::get_level_cell(size_t row, size_t column) const {
    return current_level.data[row * current_level.columns + column];
}

void Level::set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

bool Level::is_colliding(Vector2 pos, char look_for) const {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char &Level::get_collider(Vector2 pos, char look_for) {

    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }
    return get_level_cell(pos.x, pos.y);
}