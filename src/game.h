#pragma once

#include "vladlib/vladlib.h"
#include "vladlib/vmath.h"

static void set_color_to_objects(const VL_Color* color);
static void color_calculation();
static uint32_t collision(bool major_condition, bool minor_condition, VL_Rect *brick);
static void ball_collision_w_bricks();
static void bricks_transition();
static float set_ball_angle();
static void check_end_game();
static void respawn();

void game_update();

void game_draw();

static void meshes_init();
static void init_random_variables();

void game_init();

void game_run();