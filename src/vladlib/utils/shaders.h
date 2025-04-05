#include "GL/glew.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Shader {
    uint32_t type;
    uint32_t id;
} Shader;

uint32_t create_shader_program(const char* vs_file_path, const char* fs_file_path);
uint32_t use_program(uint32_t shaderProgram);
void delete_program(uint32_t id);

void add_uniform1f(uint32_t id, const char* name, float value);
void add_uniform3f(uint32_t id, const char* name, float f1, float f2, float f3);