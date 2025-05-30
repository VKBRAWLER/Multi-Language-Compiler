cmake_minimum_required(VERSION 3.10)
project(ParseTreeVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/Node.cpp
    src/Parser.cpp
    src/Tokenizer.cpp
    src/SymbolTable.cpp
    src/TraceGenerator.cpp
)

# Create executable
add_executable(parser ${SOURCES})

# Copy json.hpp to include directory if it doesn't exist
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/include/json.hpp")
    file(DOWNLOAD
        https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
        "${PROJECT_SOURCE_DIR}/include/json.hpp"
        SHOW_PROGRESS
    )
endif() 