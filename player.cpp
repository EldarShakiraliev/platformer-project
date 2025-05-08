#include "player.h"
#include "level.h"
#include "enemy.h"

Player::Player() {
    player_y_velocity = 0;
    is_player_on_ground = false;
    is_looking_forward = true;
    is_moving = false;
    player_lives = MAX_PLAYER_LIVES;
    level_index = 0;
    timer = 0;
    time_to_coin_counter = 0;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

void Player::cherry_picked() {
    PlaySound(cherry_sound);
    if (player_lives < 3) {
        player_lives++;
    }
}

void Player::shoe_picked() {
    PlaySound(shoe_sound);
    is_jump_boost_active = true;
    jump_boost_timer = JUMP_BOOST_DURATION;
}

void Player::diamond_picked() {
    PlaySound(diamond_sound);
    player_level_scores[level_index] = player_level_scores[level_index] + 5;
}

int Player::get_total_player_score() {
    int sum = 0;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }
    return sum;
}

void Player::spawn_player() {
    player_y_velocity = 0;
    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = Level::get_level_cell(row, column);
            if (cell == PLAYER) {
                player_pos.x = column;
                player_pos.y = row;
                Level::set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill_player() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void Player::move_player_horizontally(float delta) {
    float next_x = player_pos.x + delta;
    if (!Level::is_colliding({next_x, player_pos.y}, WALL)) {
        player_pos.x = next_x;
    }
    else {
        player_pos.x = roundf(player_pos.x);
        return;
    }
    is_looking_forward = delta > 0;
    if (delta != 0) is_moving = true;
}

void Player::update_player_gravity() {
    if (Level::is_colliding({player_pos.x, player_pos.y - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    is_player_on_ground = Level::is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if (is_player_on_ground) {
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }
}

void Player::update_player() {
    update_player_gravity();

    if (Level::is_colliding(player_pos, COIN)) {
        Level::get_collider(player_pos, COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (Level::is_colliding(player_pos, CHERRY)) {
        Level::get_collider(player_pos, CHERRY) = AIR;
        cherry_picked();
    }

    if (Level::is_colliding(player_pos, DIAMOND)) {
        Level::get_collider(player_pos, DIAMOND) = AIR;
        diamond_picked();
    }

    if (Level::is_colliding(player_pos, SHOE)) {
        Level::get_collider(player_pos, SHOE) = AIR;
        shoe_picked();
    }

    if (is_jump_boost_active) {
        jump_boost_timer--;
        if (jump_boost_timer <= 0) {
            is_jump_boost_active = false;
        }
    }

    if (Level::is_colliding(player_pos, EXIT)) {
        if (timer > 0) {
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        } else {
            Level::load_level(1);
            PlaySound(exit_sound);
        }
    } else {
        if (timer >= 0) timer--;
    }

    if (Level::is_colliding(player_pos, SPIKE) || player_pos.y > current_level.rows) {
        kill_player();
    }

    if (Enemies::is_colliding_with_enemies(player_pos)) {
        if (player_y_velocity > 0) {
            Enemies::remove_colliding_enemy(player_pos);
            PlaySound(kill_enemy_sound);
            increment_player_score();
            player_y_velocity = -BOUNCE_OFF_ENEMY;
        } else {
            kill_player();
        }
    }
}