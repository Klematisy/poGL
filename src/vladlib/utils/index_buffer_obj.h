#pragma once

#include "GL/glew.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t create_ibo(uint32_t count);
void fill_ibo(uint32_t id, uint64_t size, void *array);
void bind_ibo(uint32_t id);
void unbind_ibo();
void delete_ibo(uint32_t *id);
