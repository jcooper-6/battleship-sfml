cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main 
    src/main.cpp
    src/board.cpp
    src/tile.cpp
    src/TextureManager.cpp
    src/random.cpp
    src/timer.cpp
    src/counter.cpp
    src/button.cpp

)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

# Copies the images folder to the output directory where the .exe lives
file(COPY "${CMAKE_SOURCE_DIR}/images" DESTINATION "${CMAKE_BINARY_DIR}/bin")
