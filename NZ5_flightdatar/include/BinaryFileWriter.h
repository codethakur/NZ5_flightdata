#pragma once

#include <vector>
#include <string>
#include "FlightData.h"

// Function to write data to a binary file
void writeBinaryFile(const std::vector<FlightData>& data, const std::string& filePath, const std::string& sortKey);
