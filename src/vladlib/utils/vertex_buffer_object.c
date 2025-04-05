#include "vertex_buffer_object.h"

uint32_t create_vbo(uint32_t count) {
    uint32_t id;
    glGenBuffers(count, &id);
    return id;
}

uint32_t create_dynamic_vbo(uint64_t size) {
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);

    return id;
}

void fill_vbo(uint32_t id, uint64_t size, void *array, uint32_t draw_type) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, array, draw_type);
}

void fill_dynamic_vbo(uint32_t id, uint64_t size, void *array) {
    bind_vbo(id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, array);
    unbind_vbo();
}

void bind_vbo(uint32_t id) {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void delete_vbo(uint32_t *id) {
    glDeleteBuffers(1, id);
    unbind_vbo();
}

void unbind_vbo() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}