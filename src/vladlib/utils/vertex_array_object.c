#include "vertex_array_object.h"

uint32_t create_vao(uint32_t count) {
    uint32_t id;
    glGenVertexArrays(1, &id);
    return id;
}

void bind_vao(uint32_t id) {
    glBindVertexArray(id);
}

void unbind_vao() {
    glBindVertexArray(0);
}

void delete_vao(uint32_t *id) {
    glDeleteVertexArrays(1, id);
    unbind_vao();
}

void attrib_vao(uint32_t id, uint32_t index, uint32_t size, uint32_t stride, uint32_t pointer) {
    glBindVertexArray(id);

    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*) pointer);
    glEnableVertexAttribArray(index);
}