#pragma once

#include <vector>
#include <string>
#include "FlightData.h"

// Function to write data to a GeoJSON file
void writeGeoJSON(const std::vector<FlightData>& data, const std::string& filePath);
