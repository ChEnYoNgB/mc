cmake_minimum_required(VERSION 3.20)

project(TestOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.5.1.bin.WIN64")

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_ROOT}/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_ROOT}/include"
)

add_library(glad STATIC
    third_party/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include"
)

add_executable(TestOpenGL 
    main.cpp
    src/core/map.cpp
    src/core/glass.cpp
    src/core/chunk.cpp
    src/core/world.cpp
    src/core/camera.cpp
    src/core/shader.cpp
    src/core/skybox.cpp
    src/core/terrain.cpp   
    src/core/geometry.cpp 
    src/core/shadermanager.cpp
)

target_include_directories(TestOpenGL PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm"
)

target_link_libraries(TestOpenGL PRIVATE
    glfw
    glad
    opengl32
    user32
    gdi32
    shell32
)

add_custom_command(TARGET TestOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
        "$<TARGET_FILE_DIR:TestOpenGL>/shaders"
)

add_custom_command(TARGET TestOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resource"
        "$<TARGET_FILE_DIR:TestOpenGL>/resource"
)