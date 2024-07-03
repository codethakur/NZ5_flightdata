cmake_minimum_required(VERSION 3.10)
project(NZ5FlightDataProject)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(GDAL REQUIRED)
find_package(Boost REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS} ${GDAL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

# Add executable
add_executable(NZ5FlightDataProject
    src/main.cpp
    src/CSVReader.cpp
    src/GeoJSONWriter.cpp
    src/BinaryFileWriter.cpp
    src/GeoTIFFHandler.cpp
    include/json.hpp
    include/CSVReader.h
    include/GeoJSONWriter.h
    include/BinaryFileWriter.h
    include/GeoTIFFHandler.h
    include/FlightData.h
)

# Link libraries
target_link_libraries(NZ5FlightDataProject ${OpenCV_LIBS} ${GDAL_LIBRARY} ${Boost_LIBRARIES})
