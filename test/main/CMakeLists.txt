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
         8025077358804935305)

# Test Sphere
add_test(NAME spheres_on_plane
         COMMAND main_test
         /app/scenes/two-spheres-on-plane.json
         4691788139138401968)

# Test Sphere mesh
add_test(NAME sphere_mesh_on_plane
         COMMAND main_test
         /app/scenes/iso-sphere-on-plane.json
         8730072878431680756)

# Test Sphere galaxy
add_test(NAME sphere_galaxy_on_plane
         COMMAND main_test
         /app/scenes/sphere-galaxy-on-plane.json
         5650196946722507571)

# Test Monkey
add_test(NAME monkey_on_plane
         COMMAND main_test
         /app/scenes/monkey-on-plane.json
         15669477520277811191)

# Test All
add_test(NAME all
         COMMAND main_test
         /app/scenes/all.json
         16021457539211696994)

# Test Limite
add_test(NAME limite
         COMMAND main_test
         /app/scenes/without-light-scene.json
         11213600954709486593)

# Test Error
add_test(NAME Error
         COMMAND main_test
         /app/scenes/two-spheres-on-plane.json
         4691788139138401968)
