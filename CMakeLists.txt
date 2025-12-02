cmake_minimum_required(VERSION 3.10)
project(LouageTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ajouter tous les fichiers source
add_executable(server 
    src/main.cpp
    src/System.cpp
    src/Station.cpp
    src/Destination.cpp
    src/Louage.cpp
)

# Inclure les headers
target_include_directories(server PRIVATE include)
target_include_directories(server PRIVATE src)

# Pour Linux (Render)
if(UNIX)
    target_link_libraries(server pthread)
endif()

# Optimisations
target_compile_options(server PRIVATE -O2 -Wall)