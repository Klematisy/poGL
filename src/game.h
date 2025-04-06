#pragma once

#include "vladlib/vladlib.h"
#include "vladlib/vmath.h"

//void set_color_to_objects(const VL_Color* color);
void game_run();
void game_update();
void game_draw();
void game_init();

static float compare_to_axis(Vec2 axis_vec, Vec2 usually_vec) {
    float result = abv(axis_vec, usually_vec);
    if (usually_vec.y < axis_vec.y) {
        result += M_PI;
        printf("lol\n");
    }

    return result;
}