#include "game.h"
#include <stdlib.h>
#include <time.h>

#include <math.h>

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
bool enable_control  = true;
static clock_t start;
uint32_t color_gap = 0;
float acceleration = 1.0f;
float velocity_length = 0.0f;
float epsilone = 0.01f;
/* MAIN GAME VARIABLES */

static int get_random(int min, int max) {
    return min + rand() % (max - min + 1);
}

static void set_color_to_objects(const VL_Color *obj_color) {
    ball.rectangle.color = *obj_color;
    up_brick.color       = *obj_color;
    down_brick.color     = *obj_color;
    left_brick.color     = *obj_color;
    right_brick.color    = *obj_color;
}

static void color_calculation() {
    //--------------COLOR--------------//
    clock_t end = clock();
    float elapsed = (float)(end - start) / CLOCKS_PER_SEC / 2;

    VL_Color color = {(sinf(elapsed * M_PI + color_gap) + 1) / 4, (sinf(elapsed * 4) + 1) / 4, (float)(cosf(elapsed * M_PI + color_gap) + 1) / 4};
    VL_Color background_color = {1.0f - color.r, 1.0f - color.g, 1.0f - color.b};

    background.color = background_color;
    set_color_to_objects(&(VL_Color){background_color.r * 0.6f, background_color.g * 0.6f, background_color.b * 0.6f});
    //--------------COLOR--------------//
}

static uint32_t collision(bool major_condition, bool minor_condition, VL_Rect *brick) {
    Point ball_point  = {ball.rectangle.x + ball.rectangle.width  / 2,
                         ball.rectangle.y + ball.rectangle.height / 2};
    float speed = 300;
    if (major_condition && minor_condition) {
        ball.points++;
        Point brick_point = {brick->x + brick->width  / 2,
                             brick->y + brick->height / 2};
        Vec2 vector = {ball_point.x - brick_point.x, ball_point.y - brick_point.y};

        float lambda = velocity_length / length(vector);

        ball.x_speed = vector.x * lambda / speed;
        ball.y_speed = vector.y * lambda / speed;

//        printf("We were here!\n");
        return 1;
    }
    return 0;
}

static void two_bricks_concurrently(uint32_t bricks) {
    if (bricks > 1) {
        ball.x_speed *= -1.0f;
    }
}

static void ball_collision_w_bricks() {
    uint32_t bricks_count = 0;
    bool left_brick_coll_x  = ball.rectangle.x < (left_brick.x + left_brick.width) &&
                              left_brick.x < ball.rectangle.x;
    bool left_brick_coll_y  =
        (ball.rectangle.y < left_brick.y + left_brick.height) &&
        (ball.rectangle.y + ball.rectangle.height > left_brick.y);
    bricks_count += collision(left_brick_coll_x, left_brick_coll_y, &left_brick);

    bool right_brick_coll_x = right_brick.x < (ball.rectangle.x + ball.rectangle.width) &&
            ball.rectangle.x + ball.rectangle.width < right_brick.x + right_brick.width;
    bool right_brick_coll_y =
        (ball.rectangle.y < right_brick.y + right_brick.height) &&
        (ball.rectangle.y + ball.rectangle.height > right_brick.y);
    bricks_count += collision(right_brick_coll_x, right_brick_coll_y, &right_brick);

    bool up_coll_x =
            (ball.rectangle.x < up_brick.x + up_brick.width) &&
            (ball.rectangle.x + ball.rectangle.width > up_brick.x);
    bool up_coll_y = up_brick.y < (ball.rectangle.y + ball.rectangle.height) &&
                     up_brick.y + up_brick.height > ball.rectangle.y + ball.rectangle.height;
    bricks_count += collision(up_coll_y, up_coll_x, &up_brick);

    bool down_coll_x =
            (ball.rectangle.x < down_brick.x + down_brick.width) &&
            (ball.rectangle.x + ball.rectangle.width > down_brick.x);
    bool down_coll_y =
            ball.rectangle.y < (down_brick.y + down_brick.height) &&
            down_brick.y < ball.rectangle.y;
    bricks_count += collision(down_coll_y, down_coll_x, &down_brick);

    two_bricks_concurrently(bricks_count);
}

static void bricks_transition() {
    float x_speed = 0.0f;
    float y_speed = 0.0f;

    if (vl_left_pressed() && down_brick.x > 0) {
        x_speed = -0.009f;
    }

    if (vl_right_pressed() && down_brick.x + down_brick.width < 1.0f) {
        x_speed = 0.009f;
    }

    if (vl_up_pressed() && right_brick.y + right_brick.height < 1.0f) {
        y_speed = 0.009f;
    }

    if (vl_down_pressed() && right_brick.y > 0) {
        y_speed = -0.009f;
    }

    if (!enable_control) {
        acceleration *= 0.95f;
    }

    up_brick.x    += x_speed * acceleration;
    down_brick.x  += x_speed * acceleration;
    right_brick.y += y_speed * acceleration;
    left_brick.y  += y_speed * acceleration;

}

static float set_ball_angle() {
    int start_angle = get_random(1, 359);
//    start_angle = 315;
    Vec2 vector = {1, 0};
    vector = rotation_matrix(vector, (float)(start_angle * M_PI / 180.0f));

    ball.x_speed = vector.x / 300;
    ball.y_speed = vector.y / 300;
    ball.points = 0;

    return length(vector);
}

static void check_end_game() {
    if (ball.rectangle.x > 1.0f
     || ball.rectangle.x + ball.rectangle.width < 0.0f
     || ball.rectangle.y > 1.0f
     || ball.rectangle.y + ball.rectangle.height < 0.0f) {
        enable_control = false;
    }
}

static void respawn() {
    check_end_game();
    Point point1 = {0.5f, 0.5f};
    Point point2 = {ball.rectangle.x, ball.rectangle.y};
    Vec2 vector  = {point2.x - point1.x, point2.y - point1.y};
    if (length(vector) >= 1.0f) {
        ball.rectangle.x = 0.5f - ball.rectangle.width  / 2;
        ball.rectangle.y = 0.5f - ball.rectangle.height / 2;
        enable_control = true;

        acceleration = 1.0f;
        set_ball_angle();
    }
}

void game_update() {
    color_calculation();
    bricks_transition();

    ball.rectangle.x += ball.x_speed;
    ball.rectangle.y += ball.y_speed;

    ball_collision_w_bricks();
    respawn();
}

void game_draw() {
    vl_draw_rect(&background);

    vl_draw_rect(&ball.rectangle);

    vl_draw_rect(&up_brick);
    vl_draw_rect(&down_brick);
    vl_draw_rect(&left_brick);
    vl_draw_rect(&right_brick);
}

static void meshes_init() {
    background = (VL_Rect) {0.0f, 0.0f, 1.0f, 1.0f, (VL_Color) {0.1f, 0.4f, 0.6f}};
    VL_Color *bc = &background.color;
    VL_Color objects_color = (VL_Color) {1.0f - bc->r, 1.0f - bc->g, 1.0f - bc->b};

    float gbbab  = 0.02f; // gab between gap and board
    float length = 0.25f; //

    ball.rectangle = (VL_Rect) {0.5f - 0.04f / 2, 0.5f - 0.04f / 2, 0.04f, 0.04f, objects_color};;

    down_brick = (VL_Rect) {0.5f - length / 2, 0.0f, length, gbbab, objects_color};

    up_brick   = down_brick;
    up_brick.y = 1.0f - gbbab;

    right_brick = (VL_Rect) {1.0f - gbbab, 0.5f - length / 2, gbbab, length, objects_color};

    left_brick   = right_brick;
    left_brick.x = 0.0f;
}

static void init_random_variables() {
    srand(time(NULL));

    start = clock();
    color_gap += get_random(0, 10);

    velocity_length = set_ball_angle();
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