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
