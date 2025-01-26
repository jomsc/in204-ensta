cmake_minimum_required(VERSION 3.10)
project(TETRIS_RIVALS)

#Version de C++ utilisée
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Import de la SFML lib
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

#Création des différents directories créés à la compil
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#Setup des fichiers sources (tous les fichiers en terminaison cpp dans src)
file(GLOB_RECURSE SOURCES
    "src/*.cpp")

#Création des executables
add_executable(${PROJECT_NAME} ${SOURCES})

# Link de SFML et ENet au code
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        include
        ${enet_SOURCE_DIR}/include
)


