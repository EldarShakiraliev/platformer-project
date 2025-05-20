#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include <vector>

inline const char
        WALL      = '#',
        WALL_DARK = '=',
        AIR       = '-',
        SPIKE     = '^',
        PLAYER    = '@',
        ENEMY     = '&',
        COIN      = '*',
        EXIT      = 'E',
        CHERRY    = '+',
        SHOE      = 'S',
        DIAMOND   = 'D';

struct level {
    size_t rows = 0, columns = 0;
    std::vector <char> data;
};

inline int level_index = 0;
inline int LEVEL_COUNT = 0;

class Level {
public:
    Level();

    static std::vector<level> load_from_file(std::string path);
    static void reset_level_index();
    static void load_level(int offset);
    static void unload_level();

    [[nodiscard]] static bool is_inside_level(int row, int column) ;
    [[nodiscard]] static bool is_colliding(Vector2 pos, char look_for) ;
    [[nodiscard]] static char& get_collider(Vector2 pos, char look_for) ;

    [[nodiscard]] static char& get_level_cell(size_t row, size_t column) ;
    static void set_level_cell(size_t row, size_t column, char chr) ;

    static std::vector<level> LEVELS;
    static level current_level;
private:
    static std::vector <char> current_level_data;
};

#endif // LEVEL_H