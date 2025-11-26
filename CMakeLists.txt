cmake_minimum_required(VERSION 3.16)
project(ComputerGraphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- TÌM THƯ VIỆN HỆ THỐNG ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# --- GOM SOURCE (LƯU Ý: KHÔNG LẤY glad.c Ở ĐÂY) ---
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.c"
        "VecMat/*.cpp"
)

# --- TẠO THƯ VIỆN GLAD ---
add_library(glad
        ${CMAKE_SOURCE_DIR}/resources/Glad/glad/glad.c
)

target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/resources/Glad/glad
)

# --- TẠO EXECUTABLE ---
add_executable(${PROJECT_NAME}
        src/main.cpp
        ${SOURCES}
)

# --- THÊM INCLUDE PATH ---
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/includes/Features
        ${CMAKE_SOURCE_DIR}/VecMat
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_SOURCE_DIR}/resources/Glad/glad
)

# --- LINK THƯ VIỆN THEO ĐÚNG THỨ TỰ ---
target_link_libraries(${PROJECT_NAME}
        glad
        OpenGL::GL
        glfw
        glm::glm
        assimp::assimp
)

# --- COPY SHADERS & ASSETS ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        COMMENT "Creating shaders directory..."
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        COMMENT "Copying shaders..."
)

# Copy assets if directory exists
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
            COMMENT "Copying assets..."
    )
endif()

message(STATUS "=== ComputerGraphics READY & CLEAN ✅ ===")
