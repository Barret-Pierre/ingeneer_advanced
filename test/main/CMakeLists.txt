# Ajoutez l'exécutable
add_executable(main_test MainTest.cpp)
target_link_libraries(main_test 
    PUBLIC 
    rayscene
    raymath
    rayimage
    lodepng
    )

set_target_properties(main_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
)

target_include_directories(main_test PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
)

# Test Triangle
add_test(NAME triangles_on_plane
         COMMAND main_test
         /app/scenes/two-triangles-on-plane.json
         /app/test/assets/triangles_test.png)

# Test Sphere
add_test(NAME spheres_on_plane
         COMMAND main_test
         /app/scenes/two-spheres-on-plane.json
         /app/test/assets/spheres_test.png)

# Test Sphere mesh
add_test(NAME sphere_mesh_on_plane
         COMMAND main_test
         /app/scenes/iso-sphere-on-plane.json
         /app/test/assets/sphere_mesh_test.png)

# Test Sphere galaxy
add_test(NAME sphere_galaxy_on_plane
         COMMAND main_test
         /app/scenes/sphere-galaxy-on-plane.json
         /app/test/assets/spheres_galaxy_test.png)

# Test Monkey
add_test(NAME monkey_on_plane
         COMMAND main_test
         /app/scenes/monkey-on-plane.json
         /app/test/assets/monkey_test.png)

# Test All
add_test(NAME all
         COMMAND main_test
         /app/scenes/all.json
         /app/test/assets/all_test.png)

# Test Limite
add_test(NAME limite
         COMMAND main_test
         /app/scenes/without-light-scene.json
         /app/test/assets/without_light_test.png)

# Test Limite
add_test(NAME Error
         COMMAND main_test
         /app/scenes/two-spheres-on-plane.json
         /app/test/assets/triangles_test.png)
