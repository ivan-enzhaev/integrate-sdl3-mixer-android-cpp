cmake_minimum_required(VERSION 3.21)
project(integrate-sdl3-mixer-android-cpp)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define imported target for SDL3
add_library(SDL3 SHARED IMPORTED)
set_target_properties(SDL3 PROPERTIES
    IMPORTED_LOCATION "C:/libs/sdl3-3.4.14-android/arm64-v8a/lib/libSDL3.so"
)

# Define imported target for SDL3_mixer
add_library(SDL3_mixer SHARED IMPORTED)
set_target_properties(SDL3_mixer PROPERTIES
    IMPORTED_LOCATION "C:/libs/sdl3-mixer-3.2.4-android/arm64-v8a/lib/libSDL3_mixer.so"
)

add_library(app SHARED src/main.cpp)

# Include directories for all libraries
target_include_directories(app PRIVATE 
    "C:/libs/sdl3-3.4.14-android/arm64-v8a/include"
    "C:/libs/sdl3-mixer-3.2.4-android/arm64-v8a/include"
)

# Link libraries to app (order matters for dynamic linking resolution)
target_link_libraries(app PRIVATE 
    SDL3
    SDL3_mixer
)
