cmake_minimum_required (VERSION 3.10)

project (Game VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#add_subdirectory(MathFunctions)
#list(APPEND EXTRA_LIBS MathFunctions)
#list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")

add_executable (Game "src/Engine/Main.cpp" "bin/includes/glad/glad.h")

target_include_directories(Game PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/bin/includes")

add_library(GLFW STATIC IMPORTED GLOBAL)
set_property(TARGET GLFW PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/bin/GLFW/lib/glfw3.lib")

add_library(GLAD "${PROJECT_SOURCE_DIR}/bin/GLAD/src/glad.c")
target_include_directories(GLAD PUBLIC "{PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/bin/includes")

target_link_libraries(Game PUBLIC GLFW)
target_link_libraries(Game PUBLIC GLAD)

target_include_directories(Game PUBLIC "{PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/bin/glm")


