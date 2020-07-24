cmake_minimum_required (VERSION 3.8)
project (rigid LANGUAGES C)

set (${PROJECT_NAME}_C_STANDARD 99)
set (${PROJECT_NAME}_C_STANDARD_REQUIRED TRUE)

set (${PROJECT_NAME}_SOURCES
        "rigid.h"
        "src/rigid_list.c"
        "src/rigid_list_utils.c"
        "src/rigid_list_utils.h"
)

add_library (${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories (${PROJECT_NAME}
        PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}
)

set (${PROJECT_NAME}_C_FLAGS
        PRIVATE
            ${${PROJECT_NAME}_C_FLAGS}
            $<$<C_COMPILER_ID:MSVC>:/Werror>
            $<$<C_COMPILER_ID:GNU>:-Werror
                                   -Wall>
            $<$<C_COMPILER_ID:Clang>:-Werror
                                     -Wall>
)

enable_testing ()

add_subdirectory ("extern/Unity")

add_executable (${PROJECT_NAME}_list_test "tests/list_test.c")
target_link_libraries (${PROJECT_NAME}_list_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (list_test ${PROJECT_NAME}_list_test)

add_executable (${PROJECT_NAME}_list_utils_test "tests/list_utils_test.c")
target_link_libraries (${PROJECT_NAME}_list_utils_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (list_utils_test ${PROJECT_NAME}_list_utils_test)

