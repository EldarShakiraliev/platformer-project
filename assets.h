#ifndef IMAGES_H
#define IMAGES_H

#include "raylib.h"
#include "globals.h"

#include <string>
#include <cassert>
#include <iostream>
#include <stdexcept>

void load_fonts() {
    menu_font = LoadFontEx("data/fonts/ARCADE_N.ttf", 256, nullptr, 128);
    if (menu_font.texture.id == 0) {
        throw std::runtime_error("Error: Failed to load font\n");
    }
}

void unload_fonts() {
    UnloadFont(menu_font);
}

Texture2D not_error_load_texture(const std::string& file_path) {
    Texture2D texture = LoadTexture(file_path.c_str());
    if (texture.id == 0) {
        throw std::runtime_error("Error: Failed to load texture\n");
    }
    return texture;
}
void load_images() {
    wall_image                   = not_error_load_texture("data/images/wall.png");
    wall_dark_image              = not_error_load_texture("data/images/wall_dark.png");
    spike_image                  = not_error_load_texture("data/images/spikes.png");
    exit_image                   = not_error_load_texture("data/images/exit.png");
    cherry_image                 = not_error_load_texture("data/images/cherry.png");
    diamond_image                = not_error_load_texture("data/images/diamond.png");
    shoe_image                   = not_error_load_texture("data/images/shoe.png");

    coin_sprite                  = load_sprite("data/images/coin/coin", ".png", 3, true, 18);
    heart_image                  = not_error_load_texture("data/images/heart.png");

    player_stand_forward_image   = not_error_load_texture("data/images/player_stand_forward.png");
    player_stand_backwards_image = not_error_load_texture("data/images/player_stand_backwards.png");
    player_jump_forward_image    = not_error_load_texture("data/images/player_jump_forward.png");
    player_jump_backwards_image  = not_error_load_texture("data/images/player_jump_backwards.png");
    player_dead_image            = not_error_load_texture("data/images/player_dead.png");
    player_walk_forward_sprite   = load_sprite("data/images/player_walk_forward/player", ".png", 3, true, 15);
    player_walk_backwards_sprite = load_sprite("data/images/player_walk_backwards/player", ".png", 3, true, 15);

    enemy_walk                   = load_sprite("data/images/enemy_walk/enemy", ".png", 2, true, 15);

    background                   = not_error_load_texture("data/images/background/background.png");
    middleground                 = not_error_load_texture("data/images/background/middleground.png");
    foreground                   = not_error_load_texture("data/images/background/foreground.png");
}

void unload_images() {
    UnloadTexture(wall_image);
    UnloadTexture(wall_dark_image);
    UnloadTexture(spike_image);
    UnloadTexture(exit_image);
    UnloadTexture(cherry_image);
    UnloadTexture(diamond_image);
    UnloadTexture(shoe_image);

    unload_sprite(coin_sprite);
    UnloadTexture(heart_image);

    UnloadTexture(player_stand_forward_image);
    UnloadTexture(player_stand_backwards_image);
    UnloadTexture(player_jump_forward_image);
    UnloadTexture(player_jump_backwards_image);
    UnloadTexture(player_dead_image);
    unload_sprite(player_walk_forward_sprite);
    unload_sprite(player_walk_backwards_sprite);

    unload_sprite(enemy_walk);

    UnloadTexture(background);
    UnloadTexture(middleground);
    UnloadTexture(foreground);
}

void draw_image(Texture2D image, Vector2 pos, float size) {
    draw_image(image, pos, size, size);
}

void draw_image(Texture2D image, Vector2 pos, float width, float height) {
    Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    Rectangle destination = { pos.x, pos.y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

sprite load_sprite(
    const std::string &file_name_prefix,
    const std::string &file_name_suffix,
    size_t frame_count,
    bool loop,
    size_t frames_to_skip
) {
    assert(frame_count < 100);

    sprite result = {
        frame_count, frames_to_skip, 0, 0, loop, 0, new Texture2D[frame_count]
    };

    for (size_t i = 0; i < frame_count; ++i) {
        std::string file_name;
        if (frame_count < 10) {
            file_name = file_name_prefix;
            file_name += std::to_string(i);
            file_name += file_name_suffix;
        } else {
            file_name = file_name_prefix;
            file_name += i < 10 ? ("0" + std::to_string(i)) : std::to_string(i);
            file_name += file_name_suffix;
        }
        result.frames[i] = LoadTexture(file_name.c_str());
    }

    return result;
}

void unload_sprite(sprite &sprite) {
    assert(sprite.frames != nullptr);

    for (size_t i = 0; i < sprite.frame_count; ++i) {
        UnloadTexture(sprite.frames[i]);
    }
    delete[] sprite.frames;
    sprite.frames = nullptr;
}

void draw_sprite(sprite &sprite, Vector2 pos, float size) {
    draw_sprite(sprite, pos, size, size);
}

void draw_sprite(sprite &sprite, Vector2 pos, float width, float height) {
    draw_image(sprite.frames[sprite.frame_index], pos, width, height);

    if (sprite.prev_game_frame == game_frame) {
        return;
    }
    if (sprite.frames_skipped < sprite.frames_to_skip) {
        ++sprite.frames_skipped;
    } else {
        sprite.frames_skipped = 0;

        ++sprite.frame_index;
        if (sprite.frame_index >= sprite.frame_count) {
            sprite.frame_index = sprite.loop ? 0 : sprite.frame_count - 1;
        }
    }
    sprite.prev_game_frame = game_frame;
}

Sound not_error_load_sound(const std::string& file_path) {
    Sound sound = LoadSound(file_path.c_str());
    if (sound.frameCount == 0) {
        throw std::runtime_error("Error: Failed to load sound\n");
    }
    return sound;
}
void load_sounds() {
    InitAudioDevice();
    coin_sound         = not_error_load_sound("data/sounds/coin.wav");
    cherry_sound       = not_error_load_sound("data/sounds/cherry_sound.wav");
    diamond_sound      = not_error_load_sound("data/sounds/diamond_sound.wav");
    shoe_sound         = not_error_load_sound("data/sounds/shoe_sound.wav");
    exit_sound         = not_error_load_sound("data/sounds/exit.wav");
    kill_enemy_sound   = not_error_load_sound("data/sounds/kill_enemy.wav");
    player_death_sound = not_error_load_sound("data/sounds/player_death.wav");
    game_over_sound    = not_error_load_sound("data/sounds/game_over.wav");
}

void unload_sounds() {
    UnloadSound(coin_sound);
    UnloadSound(cherry_sound);
    UnloadSound(diamond_sound);
    UnloadSound(shoe_sound);
    UnloadSound(exit_sound);
    UnloadSound(kill_enemy_sound);
    UnloadSound(player_death_sound);
    UnloadSound(game_over_sound);
}

#endif // IMAGES_H
