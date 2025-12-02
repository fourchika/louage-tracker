cmake_minimum_required(VERSION 3.10)
project(LouageTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir la version de C++ explicitement
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Fichiers source
set(SOURCE_FILES
    src/main.cpp
    src/System.cpp
    src/Station.cpp
    src/Destination.cpp
    src/Louage.cpp
)

# Créer l'exécutable
add_executable(server ${SOURCE_FILES})

# Inclure les répertoires
target_include_directories(server PRIVATE include)
target_include_directories(server PRIVATE src)

# Options de compilation
if(MSVC)
    # Options pour Windows
    target_compile_options(server PRIVATE /W4 /EHsc)
else()
    # Options pour Linux (Render)
    target_compile_options(server PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wno-unused-parameter
        -O2  # Optimisation
    )
endif()

# Liaisons de bibliothèques
if(WIN32)
    # Windows: winsock2
    target_link_libraries(server ws2_32)
else()
    # Linux: pthread pour les sockets
    target_link_libraries(server pthread)
endif()

# Pour le débogage (optionnel)
option(ENABLE_DEBUG "Enable debug symbols" OFF)
if(ENABLE_DEBUG)
    if(MSVC)
        target_compile_options(server PRIVATE /DEBUG)
    else()
        target_compile_options(server PRIVATE -g)
    endif()
endif()

# Message de fin
message(STATUS "✅ Configuration CMake terminée pour ${PROJECT_NAME}")
message(STATUS "   Compilateur C++: ${CMAKE_CXX_COMPILER}")
message(STATUS "   Système: ${CMAKE_SYSTEM_NAME}")