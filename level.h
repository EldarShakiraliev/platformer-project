#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"

class Level {
public:
    Level();

    static void reset_level_index();
    static void load_level(int offset);
    static void unload_level();

    [[nodiscard]] static bool is_inside_level(int row, int column) ;
    [[nodiscard]] static bool is_colliding(Vector2 pos, char look_for) ;
    [[nodiscard]] static char& get_collider(Vector2 pos, char look_for) ;

    [[nodiscard]] static char& get_level_cell(size_t row, size_t column) ;
    static void set_level_cell(size_t row, size_t column, char chr) ;

private:
     static char* current_level_data;
};

#endif // LEVEL_H