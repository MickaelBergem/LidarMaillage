cmake_minimum_required(VERSION 2.8)
project(LidarMaillage)

#CGAL package
find_package(CGAL REQUIRED COMPONENTS Qt4)
include( ${CGAL_USE_FILE} )

# set( QT_USE_QTXML    TRUE )
# set( QT_USE_QTMAIN   TRUE )
# set( QT_USE_QTmeshRIPT  TRUE )
# set( QT_USE_QTOPENGL  TRUE )
# find_package(Qt4)
# find_package(OpenGL)
# 
# 
# if( QT4_FOUND )
#     include( ${QT_USE_FILE} )
#     find_package(QGLViewer)
# endif()

# Boost
find_package( Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})
  
add_library(PLYSimpleLoader plysimpleloader.h plysimpleloader.cpp)
target_link_libraries(PLYSimpleLoader ${Boost_LIBRARY_DIRS})

add_executable(LidarMaillage main.cpp)
target_link_libraries(LidarMaillage PLYSimpleLoader)