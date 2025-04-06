#pragma once

#include "GL/glew.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t create_vao(uint32_t count);
void bind_vao(uint32_t id);
void unbind_vao();
void delete_vao(uint32_t *id);
void attrib_vao(uint32_t id, uint32_t index, uint32_t size, uint32_t stride, uint32_t pointer);