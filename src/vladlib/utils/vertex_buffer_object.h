#pragma once

#include "GL/glew.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t create_vbo(uint32_t count);
uint32_t create_dynamic_vbo(uint64_t size);
void fill_dynamic_vbo(uint32_t id, uint64_t size, void *array);
void fill_vbo(uint32_t id, uint64_t size, void *array, uint32_t draw_type);
void bind_vbo(uint32_t id);
void unbind_vbo();
void delete_vbo(uint32_t *id);
