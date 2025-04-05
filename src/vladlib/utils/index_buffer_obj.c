#include "index_buffer_obj.h"

uint32_t create_ibo(uint32_t count) {
    uint32_t id;
    glGenBuffers(1, &id);
    return id;
}

void bind_ibo(uint32_t id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void unbind_ibo() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void delete_ibo(uint32_t *id) {
    glDeleteBuffers(1, id);
    unbind_ibo();
}

void fill_ibo(uint32_t id, uint64_t size, void *array) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, array, GL_STATIC_DRAW);
}