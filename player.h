#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

class Player {
public:
    Player();
    static void reset_player_stats();
    static void increment_player_score();
    static void cherry_picked();
    static void diamond_picked();
    static void shoe_picked();
    static int get_total_player_score();
    static void spawn_player();
    static void kill_player();
    static void move_player_horizontally(float delta);
    static void update_player_gravity();
    static void update_player();
private:
};

#endif //PLAYER_H