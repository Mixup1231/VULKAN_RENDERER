cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
set(ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw-3.4/include)
list(APPEND ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/deps/glm)

# Source files
set(ENGINE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENGINE_SRC_FILES ${ENGINE_SRC_DIR}/window.cpp)
list(APPEND ENGINE_SRC_FILES ${ENGINE_SRC_DIR}/logger.cpp)
list(APPEND ENGINE_SRC_FILES ${ENGINE_SRC_DIR}/application.cpp)

# Dependencies
set(ENGINE_DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(ENGINE_DEPS ${ENGINE_DEPS_DIR}/glfw-3.4)

project(engine VERSION 1.0)
add_library(${PROJECT_NAME} SHARED ${ENGINE_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Disable glfw test cases
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Packages
find_package(Vulkan REQUIRED)

# Subdirectories
add_subdirectory(${ENGINE_DEPS})

target_link_libraries(${PROJECT_NAME} PUBLIC glfw Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ${ENGINE_INCLUDES})
target_compile_definitions(${PROJECT_NAME} PRIVATE MARS_EXPORT)
