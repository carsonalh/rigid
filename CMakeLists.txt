cmake_minimum_required (VERSION 3.8)
project (rigid LANGUAGES C)

set (${PROJECT_NAME}_C_STANDARD 99)
set (${PROJECT_NAME}_C_STANDARD_REQUIRED TRUE)

set (${PROJECT_NAME}_SOURCES
        "rigid.h"
        "src/rigid_list.c"
        "src/rigid_list_internal.c"
        "src/rigid_list_internal.h"
)

add_library (${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories (${PROJECT_NAME}
        PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}
)

set (${PROJECT_NAME}_C_FLAGS
        ${${PROJECT_NAME}_C_FLAGS}
        $<$<C_COMPILER_ID:MSVC>:/Werror>
        $<$<OR:
            $<C_COMPILER_ID:GNU>,
            $<C_COMPILER_ID:Clang>,
            $<C_COMPILER_ID:MinGW>>:-Werror
                                    -Wall>
)

# If the CMAKE_BUILD_TYPE variable is set, use that value; otherwise use
# "Debug"
set (${PROJECT_NAME}_BUILD_TYPE
        $<IF:$<BOOL:${CMAKE_BUILD_TYPE}>,${CMAKE_BUILD_TYPE},Debug>)

if (${PROJECT_NAME}_BUILD_TYPE STREQUAL Debug)
    set (${PROJECT_NAME}_C_FLAGS
            ${${PROJECT_NAME}_C_FLAGS}
            $<$<C_COMPILER_ID:MSVC>:/DEBUG>
            $<$<OR:
                $<C_COMPILER_ID:GNU>,
                $<C_COMPILER_ID:Clang>,
                $<C_COMPILER_ID:MinGW>>:-g>
    )
endif ()

enable_testing ()

add_subdirectory ("extern/Unity")

add_executable (${PROJECT_NAME}_list_test "tests/list_test.c")
target_link_libraries (${PROJECT_NAME}_list_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (list_test ${PROJECT_NAME}_list_test)

add_executable (${PROJECT_NAME}_list_internal_test "tests/list_internal_test.c")
target_link_libraries (${PROJECT_NAME}_list_internal_test
        PRIVATE
            ${PROJECT_NAME}
            "unity"
)
add_test (list_internal_test ${PROJECT_NAME}_list_internal_test)

