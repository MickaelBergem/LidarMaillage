cmake_minimum_required(VERSION 2.8)
project(LidarMaillage)

#CGAL package
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

  
add_library(PLYSimpleLoader plysimpleloader.h plysimpleloader.cpp)
add_executable(LidarMaillage main.cpp)
target_link_libraries(LidarMaillage PLYSimpleLoader)