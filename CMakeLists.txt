cmake_minimum_required(VERSION 4.2)

set(CMAKE_CXX_STANDARD 17)

project (CVFirstPrinciples)

add_executable(CVFirstPrinciples 
    main.cpp
    enhancements.cpp
    enhancements.h
    GaussianFilter.cpp
    GaussianFilter.h
    imgOps.cpp
    imgOps.h
    rotate.cpp
    rotate.h
    scale.cpp
    scale.h
    similarity.cpp
    similarity.h
    translate.cpp
    translate.h)

include_directories(CVFirstPrinciples "/opt/homebrew/Cellar/opencv/4.12.0_19/include/opencv4")

find_package(OpenCV REQUIRED)
target_link_libraries(CVFirstPrinciples ${OpenCV_LIBS}) 

target_compile_options(CVFirstPrinciples PRIVATE -Wall -Wextra -Wpedantic)
