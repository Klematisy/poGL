#include "shaders.h"

static void* load_file(const char* file_path) {
    FILE *file = fopen(file_path, "r");

    if (file) {
        fseek(file, 0, SEEK_END);
        uint64_t size = ftell(file);
        fseek(file, 0, 0);

        char *content = (char *) malloc(size);
        fread(content, size, 1, file);

        fclose(file);

        return content;
    }

    return (void*) 0;
}

static void check_compilation_errors(const Shader *shader) {
    int result;
    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(shader->id, length, &length, message);
        fprintf(stderr, "Failed to compile ");
        switch (shader->type) {
            case GL_VERTEX_SHADER:
                fprintf(stderr, "vertex shader!\n");
                break;
            case GL_FRAGMENT_SHADER:
                fprintf(stderr, "fragment shader!\n");
                break;
            case GL_GEOMETRY_SHADER:
                fprintf(stderr, "geometry shader!\n");
                break;
            case GL_COMPUTE_SHADER:
                fprintf(stderr, "compute shader!\n");
                break;
            default:
                fprintf(stderr, "unknown shader!\n");
        }
        fprintf(stderr, "%s", message);

        free(message);
    }
}

static uint32_t create_shader(GLenum type, const char* file_path) {
    char* src = load_file(file_path);

    uint32_t shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &src, NULL);
    glCompileShader(shader_id);
    Shader shader = {type, shader_id};

    check_compilation_errors(&shader);

    free(src);
    return shader.id;
}

uint32_t create_shader_program(const char* vs_file_path, const char* fs_file_path) {
    uint32_t program = glCreateProgram();
    uint32_t vs = create_shader(GL_VERTEX_SHADER,   vs_file_path);
    uint32_t fs = create_shader(GL_FRAGMENT_SHADER, fs_file_path);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void add_uniform(uint32_t id, const char* name, float value) {
    glUniform1f(glGetUniformLocation(id,  name), value);
}

void set_uniform3f(uint32_t id, const char* name, float f1, float f2, float f3) {
    glUniform3f(glGetUniformLocation(id,  name), f1, f2, f3);
}

uint32_t use_program(uint32_t shaderProgram) {
    glUseProgram(shaderProgram);
}

void delete_program(uint32_t id) {
    glDeleteProgram(id);
}