cmake_minimum_required(VERSION 3.10.0)
project(PICADOR-JR VERSION 0.1.0 LANGUAGES C CXX)

# Collect all .cpp files from the Sources folder
file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/PICADOR-Jr-Project/Source/*.cpp"
)

# Add the main .cpp
list(APPEND SRC_FILES PICADOR-Jr-Project.cpp)

# Create the executable
add_executable(PICADOR-JR ${SRC_FILES})

# Include headers
target_include_directories(PICADOR-JR PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/PICADOR-Jr-Project/Include"
)