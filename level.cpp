#include "level.h"
#include "raylib.h"
#include "player.h"
#include "enemy.h"
#include <fstream>
#include <iostream>

std::vector <char> Level::current_level_data;

Level::Level() {
    current_level_data = std::vector <char> ();
}

std::vector<level> Level::load_from_file(std::string path) {
    std::ifstream file(path);
    std::string line;
    std::vector<level> levels;
    while(std::getline(file, line)) {
        if (line[0] == ';') {
            continue;
        }
        int vertical_bar = 0;
        std::vector<char> level_data;
        for (int i = 0; i < line.size(); ++i) {
            if (line[i] == '|') {
                vertical_bar++;
                continue;
            }
            if ('0' <= line[i] && line[i] <= '9') {
                int end = i+1;
                while('0' <= line[end] && line[end] <= '9') {
                    end++;
                }
                std::string substr = line.substr(i, end - i);
                   int num = std::stoi(substr);
                char ch = line[end];
                for (int j = 0; j < num; j++) {
                    level_data.push_back(ch);
                }
                i = end;
                continue;
            }
            if (line[i] == '@' || line[i] == '=' || line[i] == '#' || line[i] == '-' || line[i] == '*' || line[i] == '^' || line[i] == '&' || line[i] == 'E') {
                level_data.push_back(line [i]);
            } else {
                throw std::runtime_error("The level data is corrupted.\n");
            }
        }
        level lvl = {
                static_cast<size_t>(vertical_bar + 1),
                level_data.size() / (vertical_bar + 1),
                level_data
        };
        levels.push_back(lvl);
    }
    LEVEL_COUNT = levels.size();
    player_level_scores = std::vector <int> (levels.size());
    return levels;
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
    current_level_data = std::vector <char>(rows * columns);

    for (size_t row = 0; row < rows; row++) {
        for (size_t column = 0; column < columns; column++) {
            current_level_data[row * columns + column] = LEVELS[level_index].data[row * columns + column];
        }
    }

    current_level = {rows, columns, current_level_data};
    Player::spawn_player();
    Enemies::spawn_enemies();
    derive_graphics_metrics_from_loaded_level();
    timer = MAX_LEVEL_TIME;
}

void Level::unload_level() {
     current_level_data.clear();
}

bool Level::is_inside_level(int row, int column) {
    if (row < 0 || row >= current_level.rows) return false;
    if (column < 0 || column >= current_level.columns) return false;
    return true;
}

char& Level::get_level_cell(size_t row, size_t column) {
    return current_level.data[row * current_level.columns + column];
}

void Level::set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}

bool Level::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float)column, (float)row, 1.0f, 1.0f};
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