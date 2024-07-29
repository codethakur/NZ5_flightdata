This project involves reading flight data from a CSV file, processing the data, and visualizing the flight trajectory on a geotiff image.



Prerequisites
C++17 or later
CMake
OpenGL
GeoTIFF library
JSON library (e.g., nlohmann/json)
Thread library

---------------------
Project Structure
---------------------
├── data
│   ├── NZ5_flightdata.csv
│   ├── world.tif
├── include
│   ├── FlightData.h
│   ├── GeoJSONWriter.h
│   ├── OutputStruct.h
│   ├── TransformCoordinates.h
│   ├── VisualizeFlight.h
├── src
│   ├── main.cpp
│   ├── FlightData.cpp
│   ├── GeoJSONWriter.cpp
│   ├── OutputStruct.cpp
│   ├── TransformCoordinates.cpp
│   ├── VisualizeFlight.cpp
├── CMakeLists.txt
├── README.md
