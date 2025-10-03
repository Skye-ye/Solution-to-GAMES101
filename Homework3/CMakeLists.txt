cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(Rasterizer
    main.cpp
    rasterizer.hpp
    rasterizer.cpp
    global.hpp 
    Triangle.hpp
    Triangle.cpp
    Texture.hpp
    Texture.cpp
    Shader.hpp
    OBJ_Loader.h
)

target_include_directories(Rasterizer PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Rasterizer ${OpenCV_LIBS} Eigen3::Eigen)