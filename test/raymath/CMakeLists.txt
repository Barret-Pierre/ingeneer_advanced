# Find all test files in test/raymath
file(GLOB TEST_SOURCES "*.cpp")

# Create an executable for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
    )

    target_include_directories(${TEST_NAME} PUBLIC
                               "${PROJECT_SOURCE_DIR}/src/raymath"
                               )

    target_link_libraries(${TEST_NAME} 
                          PUBLIC 
                          raymath
                          )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()