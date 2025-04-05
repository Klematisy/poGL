#include "game.h"
#include <stdlib.h>
#include <time.h>

#include <math.h>

#include "game.h"
#include "vladlib/vladlib.h"
#include "vladlib/vmath.h"

/* MAIN GAME OBJECTS */
typedef struct Ball {
    VL_Rect rectangle;

    float x_speed;
    float y_speed;

    uint32_t points;
} Ball;

static Ball ball;

static VL_Rect up_brick;
static VL_Rect down_brick;
static VL_Rect right_brick;
static VL_Rect left_brick;

static VL_Rect background;
/* MAIN GAME OBJECTS */

/* MAIN GAME VARIABLES */
bool game_is_running = true;
static clock_t start;
float color_gap = 0.0f;
float visible_part = 0.02f;
/* MAIN GAME VARIABLES */

static int get_random(int min, int max) {
    return min + rand() % (max - min + 1);
}

static void color_calculation() {
    //--------------COLOR--------------//
    clock_t end = clock();
    float elapsed = (float)(end - start) / CLOCKS_PER_SEC / 2;

    VL_Color color = {(float)(sin(elapsed * M_PI + color_gap) + 1) / 4, elapsed / 10, (float)(cos(elapsed * M_PI + color_gap) + 1) / 4};
    VL_Color background_color = {1.0f - color.r, 1.0f - color.g, 1.0f - color.b};

    background.color = background_color;
    set_color_to_objects(&(VL_Color){background_color.r * 0.6f, background_color.g * 0.6f, background_color.b * 0.6f});
    //--------------COLOR--------------//
}

static void bricks_transition(float *x_speed, float *y_speed) {

    if (vl_left_pressed() && down_brick.x > 0) {
        *x_speed = -0.009f;
    }

    if (vl_right_pressed() && down_brick.x + down_brick.width < 1.0f) {
        *x_speed = 0.009f;
    }

    if (vl_up_pressed() && right_brick.y + right_brick.height < 1.0f) {
        *y_speed = 0.009f;
    }

    if (vl_down_pressed() && right_brick.y > 0) {
        *y_speed = -0.009f;
    }

    up_brick.x    += *x_speed;
    down_brick.x  += *x_speed;
    right_brick.y += *y_speed;
    left_brick.y  += *y_speed;
}

static void ball_collision() {
    bool left_brick_coll_x  = ball.rectangle.x < (left_brick.x + left_brick.width);
    bool left_brick_coll_y  =
        (ball.rectangle.y < left_brick.y + ball.rectangle.height + left_brick.height) &&
        (ball.rectangle.y + ball.rectangle.height > left_brick.y);
    if (left_brick_coll_x && left_brick_coll_y) {
        ball.x_speed *= -1.0f;
        ball.points++;
        if (ball.points % 2 == 0) {
            ball.x_speed *= 1.2f;
            ball.y_speed *= 1.2f;
        }
    } else if (left_brick_coll_x) {
        game_is_running = false;
    }

    bool right_brick_coll_x = right_brick.x < (ball.rectangle.x + ball.rectangle.width);
    bool right_brick_coll_y =
        (ball.rectangle.y < right_brick.y + right_brick.height) &&
        (ball.rectangle.y + ball.rectangle.height > right_brick.y);
    if (right_brick_coll_x && right_brick_coll_y) {
        ball.x_speed *= -1.0f;
        ball.points++;
        if (ball.points % 2 == 0) {
            ball.x_speed *= 1.2f;
            ball.y_speed *= 1.2f;
        }
    } else if (right_brick_coll_x) {
        game_is_running = false;
    }

    bool up_coll_y = up_brick.y < (ball.rectangle.y + ball.rectangle.height);
    bool up_coll_x =
            (ball.rectangle.x < up_brick.x + up_brick.width) &&
            (ball.rectangle.x + ball.rectangle.width > up_brick.x);
    if (up_coll_x && up_coll_y) {
        ball.y_speed *= -1.0f;
        ball.points++;
        if (ball.points % 2 == 0) {
            ball.x_speed *= 1.2f;
            ball.y_speed *= 1.2f;
        }
    } else if (up_coll_y) {
        game_is_running = false;
    }

    bool down_coll_y = ball.rectangle.y < (down_brick.y + down_brick.height);
    bool down_coll_x =
            (ball.rectangle.x < down_brick.x + down_brick.width) &&
            (ball.rectangle.x + ball.rectangle.width > down_brick.x);
    if (down_coll_x && down_coll_y) {
        ball.y_speed *= -1.0f;
        ball.points++;
        if (ball.points % 2 == 0) {
            ball.x_speed *= 1.2f;
            ball.y_speed *= 1.2f;
        }
    } else if (down_coll_y) {
        game_is_running = false;
    }
}

void game_update() {
    color_calculation();

    float x_speed = 0.0f;
    float y_speed = 0.0f;
    bricks_transition(&x_speed, &y_speed);

    ball.rectangle.x += ball.x_speed;
    ball.rectangle.y += ball.y_speed;

    ball_collision();
}

void game_draw() {
    vl_draw_rect(&background);

    vl_draw_rect(&ball.rectangle);

    vl_draw_rect(&up_brick);
    vl_draw_rect(&down_brick);
    vl_draw_rect(&left_brick);
    vl_draw_rect(&right_brick);
}

void set_color_to_objects(const VL_Color *obj_color) {
    ball.rectangle.color = *obj_color;
    up_brick.color       = *obj_color;
    down_brick.color     = *obj_color;
    left_brick.color     = *obj_color;
    right_brick.color    = *obj_color;
}

void meshes_init() {
    background = (VL_Rect) {0.0f, 0.0f, 1.0f, 1.0f, (VL_Color) {0.1f, 0.4f, 0.6f}};
    VL_Color *bc = &background.color;
    VL_Color objects_color = (VL_Color) {1.0f - bc->r, 1.0f - bc->g, 1.0f - bc->b};

    float gbbab  = 0.35f; // gab between gap and board
    float length = 0.25f; //

    ball.rectangle = (VL_Rect) {0.5f - 0.04f / 2, 0.5f - 0.04f / 2, 0.04f, 0.04f, objects_color};;
//    printf("%f\n", (ball.rectangle).x);

    down_brick = (VL_Rect) {0.5f - length / 2, 0.0f - gbbab + visible_part, length, gbbab, objects_color};

    up_brick   = down_brick;
    up_brick.y = 1.0f - visible_part;

    right_brick = (VL_Rect) {1.0f - visible_part, 0.5f - length / 2, gbbab, length, objects_color};

    left_brick   = right_brick;
    left_brick.x = visible_part - gbbab;
}

static void init_random_variables() {
    srand(time(NULL));

    start = clock();
    color_gap += get_random(0, 10);

    float start_angle = get_random(1, 359);
//    float start_angle = 90;
//    printf("%f", start_angle);
    Vec2 vector = {1, 0};
    vector = rotation_matrix(vector, start_angle * M_PI / 180.0f);

    ball.x_speed = vector.x / 400;
    ball.y_speed = vector.y / 400;

    ball.points = 0;
}

void game_init() {
    meshes_init();
    init_random_variables();
}

void game_run() {
    game_init();

    /*     ****MAIN LOOP****     */
    while (vl_window_open() && game_is_running) {
        game_update();
        vl_draw();
            game_draw();
        vl_end_draw();
    }

    printf("Your score: %u!", ball.points);
}