cmake_minimum_required(VERSION 3.8)
project(DescsInMutiViews)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp VideoStream.cpp VideoStream.h BufferedVideoStream.cpp BufferedVideoStream.h SynQueue.hpp FeatureExtractor.cpp FeatureExtractor.h FeatureMatcher.cpp FeatureMatcher.h FeatureTracker.cpp FeatureTracker.h)
add_executable(DescsInMutiViews ${SOURCE_FILES})
target_link_libraries(DescsInMutiViews ${OpenCV_LIBS})