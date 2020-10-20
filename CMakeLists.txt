cmake_minimum_required (VERSION 3.8)
project (rigid VERSION 0.1.1 LANGUAGES C)

set (${PROJECT_NAME}_C_STANDARD 99)
set (${PROJECT_NAME}_C_STANDARD_REQUIRED TRUE)

add_library (${PROJECT_NAME} STATIC
        "rigid.h"
        "src/rigid_list.c"
        "src/rigid_list_internal.c"
        "src/rigid_list_internal.h"
        "src/rigid_sort.c"
        "src/rigid_stack.c"
)
target_include_directories (${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set (${PROJECT_NAME}_C_FLAGS
        ${${PROJECT_NAME}_C_FLAGS}
            $<$<C_COMPILER_ID:MSVC>:/Wall>
            $<$<OR:
                $<C_COMPILER_ID:GNU>,
                $<C_COMPILER_ID:Clang>,
                $<C_COMPILER_ID:MinGW>>:-pedantic-errors -Werror -Wall>
)

include (CMakePackageConfigHelpers)
write_basic_package_version_file (
        ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${${PROJECT_NAME}_VERSION}
        COMPATIBILITY SameMinorVersion
)

install (TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
)
install (EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME})
install (FILES
            ${${PROJECT_NAME}_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake
            ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
)
install (FILES rigid.h DESTINATION include)

enable_testing ()

find_package (pest 0.1.1 REQUIRED)

add_executable (${PROJECT_NAME}_list_test "tests/list_test.c")
target_link_libraries (${PROJECT_NAME}_list_test
        PRIVATE
            ${PROJECT_NAME}
            pest::pest
)
add_test (list_test ${PROJECT_NAME}_list_test)

add_executable (${PROJECT_NAME}_list_internal_test
        "tests/list_internal_test.c")
target_link_libraries (${PROJECT_NAME}_list_internal_test
        PRIVATE
            ${PROJECT_NAME}
            pest::pest
)
add_test (list_internal_test ${PROJECT_NAME}_list_internal_test)

add_executable (${PROJECT_NAME}_sort_test "tests/sort_test.c")
target_link_libraries (${PROJECT_NAME}_sort_test
        PRIVATE
            ${PROJECT_NAME}
            pest::pest
)
add_test (sort_test ${PROJECT_NAME}_sort_test)

