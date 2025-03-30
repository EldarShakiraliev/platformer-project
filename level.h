#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"

class Level {
public:
    Level();
    ~Level();

    void reset_level_index();
    void load_level(int offset);
    void unload_level();

    [[nodiscard]] bool is_inside_level(int row, int column) const;
    [[nodiscard]] bool is_colliding(Vector2 pos, char look_for) const;
    char& get_collider(Vector2 pos, char look_for);

    [[nodiscard]] char& get_level_cell(size_t row, size_t column) const;
    void set_level_cell(size_t row, size_t column, char chr);

private:
    char* current_level_data;
};

#endif // LEVEL_H