#pragma once

#include <vector>
#include <string>
#include "FlightData.h"

// Function to read CSV file
std::vector<FlightData> readCSV(const std::string& filePath);
