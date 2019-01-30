cmake_minimum_required(VERSION 3.8)
project(Yoloassociate)
set(CMAKE_CXX_STANDARD 11)

# opencv
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


# Build examples
set(SOURCE_FILES main.cpp)
add_executable(Yoloassociate ${SOURCE_FILES})
target_link_libraries( Yoloassociate ${OpenCV_LIBS} )