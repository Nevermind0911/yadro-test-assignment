cmake_minimum_required(VERSION 3.10)
project(dungeon_bot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dungeon_bot
    src/main.cpp
    src/dungeon_alg.cpp
    src/parser.cpp
    src/alice_bot.cpp
)

target_include_directories(dungeon_bot PRIVATE include)

enable_testing()

file(GLOB TEST_INPUTS "${CMAKE_SOURCE_DIR}/tests/*-input.txt")
foreach(INPUT ${TEST_INPUTS})
    get_filename_component(BASENAME ${INPUT} NAME_WE)
    string(REGEX REPLACE "-input$" "" TEST_NAME ${BASENAME})
    set(EXPECTED "${CMAKE_SOURCE_DIR}/tests/${TEST_NAME}-output.txt")

    if(EXISTS ${EXPECTED})
        add_test(
            NAME ${TEST_NAME}
            COMMAND ${CMAKE_COMMAND}
                -DEXEC=$<TARGET_FILE:dungeon_bot>
                -DINPUT=${INPUT}
                -DEXPECTED=${EXPECTED}
                -P ${CMAKE_SOURCE_DIR}/cmake/run_test.cmake
        )
        set_tests_properties(${TEST_NAME} PROPERTIES RESOURCE_LOCK result_txt)
    endif()
endforeach()
