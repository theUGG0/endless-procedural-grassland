cmake_minimum_required(VERSION 3.10)

project(Endless_Grassland)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(CREATE_LINK "${CMAKE_BINARY_DIR}/compile_commands.json" "${CMAKE_CURRENT_LIST_DIR}/compile_commands.json" SYMBOLIC)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPROJ_ROOT_PATH=\\\"\"${CMAKE_CURRENT_LIST_DIR}\\\"\" -D_CRT_SECURE_NO_WARNINGS -DGLM_ENABLE_EXPERIMENTAL")

add_executable(Endless_Grassland
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_include_directories(Endless_Grassland PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(Endless_Grassland PRIVATE glfw OpenGL::GL GLEW::GLEW glm::glm-header-only)