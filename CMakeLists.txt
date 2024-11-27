cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

# ----- Multithreading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# Option pour ajouter le mutithreading par défault
option(ENABLE_MULTITHREADING "Enable multithreading for rendering" ON)
# Posibité de désactiver le multithreading
if (ENABLE_MULTITHREADING)
    add_compile_definitions(ENABLE_MULTITHREADING)
else()
    add_compile_definitions(DISABLE_MULTITHREADING)
endif()

# ----- Bounding box
# Option pour ajouter le boundign box par défault
option(ENABLE_BOUNDING_BOX "Enable bounding box for rendering" ON)
# Posibité de désactiver le multithreading
if (ENABLE_BOUNDING_BOX)
    add_compile_definitions(ENABLE_BOUNDING_BOX)
else()
    add_compile_definitions(DISABLE_BOUNDING_BOX)
endif()

# ----- Test config
enable_testing()

# ----- Base config
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer main.cpp)

target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

# Ajoutez le sous-répertoire des tests
add_subdirectory(test)

target_link_libraries(raytracer 
                      PUBLIC 
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      Threads::Threads
                      )
