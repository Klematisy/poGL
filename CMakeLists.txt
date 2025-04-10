cmake_minimum_required(VERSION 3.28)
project(poGL C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/clang)

find_package(OpenGL REQUIRED)
include_directories(poGL deps/include)

add_executable(poGL src/main.c
         src/vladlib/utils/glew.c
         src/vladlib/utils/shaders.c
         src/vladlib/utils/shaders.h
         src/vladlib/utils/vertex_buffer_object.c
         src/vladlib/utils/vertex_buffer_object.h
         src/vladlib/utils/vertex_array_object.h
         src/vladlib/utils/vertex_array_object.c
         src/vladlib/utils/index_buffer_obj.h
         src/vladlib/utils/index_buffer_obj.c
         src/vladlib/vladlib.h
         src/vladlib/vladlib.c
         src/game.h
         src/game.c
         src/vladlib/vmath.c)

target_link_libraries(poGL ${CMAKE_SOURCE_DIR}/deps/library/libglfw.3.4.dylib)
target_link_libraries(poGL OpenGL::GL)