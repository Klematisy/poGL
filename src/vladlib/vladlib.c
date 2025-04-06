#include "vladlib.h"

static  MAIN_LIB_WINDOW window = nullptr;

static uint32_t MAIN_2D_VBO;
static uint32_t MAIN_2D_VAO;
static uint32_t MAIN_2D_IBO;

static uint32_t BASE_2D_SHADER;

ObjectOfData main_data;

static uint32_t rect_indices[] = {
    0, 1, 2,
    2, 3, 0
};

void BindObjectOfData(const ObjectOfData *ood) {
    bind_vao(ood->vao);
    use_program(ood->shader_program);
}

static void normalize_points(float *array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        array[i] = 2 * array[i] - 1;
    }
}

static void buffers_init() {
    MAIN_2D_VBO = create_dynamic_vbo(8 * sizeof(float)); // CREATE MAIN DYNAMIC DRAW VBO

    MAIN_2D_VAO = create_vao(1);
    attrib_vao(MAIN_2D_VAO, 0, 2, sizeof(float) * 2, 0);

    MAIN_2D_IBO = create_ibo(1);
    fill_ibo(MAIN_2D_IBO, sizeof(rect_indices), rect_indices);

    BASE_2D_SHADER = create_shader_program("src/vladlib/resources/shaders/base_shader.vert", "src/vladlib/resources/shaders/base_shader.frag");

    unbind_vao();
    unbind_ibo();
    unbind_vbo();

    main_data = (ObjectOfData) {MAIN_2D_VAO, BASE_2D_SHADER};
}

static void specify_opengl_version() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

static bool glew_init(void) {
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Glew init error!\n");
        return false;
    }
    return true;
}

void* vl_init_lib(uint16_t width, uint16_t height, const char* window_name) {

    if (!glfwInit())
        return nullptr;

    specify_opengl_version();

    window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Window creating error!\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!glew_init()) {
        window = nullptr;
    }

    buffers_init();

    return window;
}

void vl_draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void vl_end_draw() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool vl_window_open() {
    return !glfwWindowShouldClose(window);
}

void vl_deinit_lib(void) {
    delete_vbo(&MAIN_2D_VBO);
    delete_vao(&MAIN_2D_VAO);
    delete_ibo(&MAIN_2D_IBO);

    delete_program(BASE_2D_SHADER);

    glfwTerminate();
}

//------------------------DRAW------------------------//

void vl_draw_rect(VL_Rect *rect) {
    float vertices[] = {
            rect->x,                rect->y,
            rect->x,                rect->y + rect->height,
            rect->x + rect->width,  rect->y + rect->height,
            rect->x + rect->width,  rect->y
    };

    normalize_points(vertices, sizeof(vertices) / sizeof(float));
    fill_dynamic_vbo(MAIN_2D_VBO, sizeof(vertices), vertices);

    BindObjectOfData(&main_data);

    add_uniform3f(BASE_2D_SHADER, "color", rect->color.r, rect->color.g, rect->color.b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    unbind_vao();
}

// INPUT FUNCTIONS
bool vl_left_pressed(void)  { return glfwGetKey(window,  GLFW_KEY_LEFT) == GLFW_PRESS; }
bool vl_right_pressed(void) { return glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS; }
bool vl_up_pressed(void)    { return glfwGetKey(window,    GLFW_KEY_UP) == GLFW_PRESS; }
bool vl_down_pressed(void)  { return glfwGetKey(window,  GLFW_KEY_DOWN) == GLFW_PRESS; }