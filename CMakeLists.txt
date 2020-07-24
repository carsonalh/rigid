cmake_minimum_required (VERSION 3.8)
project (rigid LANGUAGES C)

set (${PROJECT_NAME}_C_STANDARD 99)
set (${PROJECT_NAME}_C_STANDARD_REQUIRED TRUE)

set (${PROJECT_NAME}_SOURCES
        "rigid.h"
        "src/rigid_array_list.c"
        "src/rigid_array_list_utils.c"
        "src/rigid_array_list_utils.h"
)

add_library (${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories (${PROJECT_NAME}
        PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}
)

enable_testing ()

add_subdirectory ("extern/Unity")

add_executable (${PROJECT_NAME}_array_list_test "tests/array_list_test.c")
target_link_libraries (${PROJECT_NAME}_array_list_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (array_list_test ${PROJECT_NAME}_array_list_test)

add_executable (${PROJECT_NAME}_array_list_utils_test "tests/array_list_utils_test.c")
target_link_libraries (${PROJECT_NAME}_array_list_utils_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (array_list_utils_test ${PROJECT_NAME}_array_list_utils_test)

