cmake_minimum_required(VERSION 3.10)
project(SLAM VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.c
    src/utils.c
    src/config.c
    src/wav_reader.c
    src/validator.c
)

add_executable(slam ${SOURCES})