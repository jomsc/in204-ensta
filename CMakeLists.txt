cmake_minimum_required(VERSION 3.14)
project(TETRIS_RIVALS)

#Version de C++ utilisée
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Import de la SFML lib
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

if(NOT SFML_FOUND)
    message(STATUS "SFML not found on system - fetching from source")
    include(FetchContent)
    
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5
    )
    
    FetchContent_MakeAvailable(SFML)
else()
    message(STATUS "Using system-installed SFML")
endif()

#Création des différents directories créés à la compil
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)

#Setup des fichiers sources (tous les fichiers en terminaison cpp dans src)
file(GLOB_RECURSE SOURCES
    "src/*.cpp")

#Création des executables
add_executable(${PROJECT_NAME} ${SOURCES})

# For GCC/Clang, link against libstdc++fs or libc++fs
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()

# Link de SFML au code
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
        sfml-audio
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        include
        ${enet_SOURCE_DIR}/include
)

# Cross-platform copy of assets
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    VERBATIM
)




