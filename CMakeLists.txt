cmake_minimum_required(VERSION 3.2)

project(serialization)

set(APP_SOURCES 
	src/main.cpp
)


set(TEST_SOURCES 
	test/main.cpp
)

set(COMMON_SOURCES 
	include/serialization.hpp
	include/meta.hpp
	include/array.hpp
	src/array.cpp
	include/primitive.hpp
	src/primitive.cpp
	include/context.hpp
	src/context.cpp
	include/root.hpp
)

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest" 
    "googletest"
)

add_executable(${PROJECT_NAME} 
	${APP_SOURCES}
	${COMMON_SOURCES}
)

add_executable(unit_test 
	${TEST_SOURCES}
	${COMMON_SOURCES}
)

target_include_directories(
	unit_test PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(
    unit_test PRIVATE
		gtest gtest_main
)

target_include_directories(
	${PROJECT_NAME} PRIVATE 
        "${PROJECT_SOURCE_DIR}/include"
)

if (MSVC)
    # @see https://github.com/google/googletest/blob/master/googletest/README.md
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /W4
    )
    target_compile_options(
        unit_test PRIVATE
        /W4
    )
else ()
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
        unit_test PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
