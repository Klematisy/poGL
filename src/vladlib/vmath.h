#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef Vec2 Point;

float abv(Vec2 v1, Vec2 v2);
Vec2 rotation_matrix(Vec2 vec1, float angle_in_radians);
float length(Vec2 vector);
