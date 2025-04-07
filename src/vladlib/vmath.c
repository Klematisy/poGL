#include "vmath.h"

static float m_sin(float angle) {
    if (fmodf(angle, M_PI) == 0.0f || angle == 0.0f) {
        return 0;
    } else {
        return sinf(angle);
    }
}

static float m_cos(float angle) {
    if (fmodf(angle, M_PI_2) == 0.0f || fmodf(angle, 3 * M_PI_2) == 0.0f) {
        return 0.0f;
    } else {
        return cosf(angle);
    }
}

Vec2 rotation_matrix(Vec2 vec1, float angle_in_radians) {
    Vec2 vec2;
    vec2.x = vec1.x * m_cos(angle_in_radians) - vec1.y * m_sin(angle_in_radians);
    vec2.y = vec1.x * m_sin(angle_in_radians) + vec1.y * m_cos(angle_in_radians);
    return vec2;
}

float abv(Vec2 v1, Vec2 v2) {
//    ab = la * lb * cos ab
    float la = length(v1);
    float lb = length(v2);
    float ab = v1.x * v2.x + v1.y * v2.y;
    return acosf(ab / (la * lb));
}

float length(Vec2 vector) {
    return sqrtf(vector.x * vector.x + vector.y * vector.y);
}