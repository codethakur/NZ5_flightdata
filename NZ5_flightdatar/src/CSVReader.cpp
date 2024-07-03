#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<FlightData> readCSV(const std::string& filePath) {
    std::vector<FlightData> data;
    std::ifstream file(filePath);
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string time, speed, altitude, longitude, latitude;

        std::getline(stream, time, ',');
        std::getline(stream, speed, ',');
        std::getline(stream, altitude, ',');
        std::getline(stream, longitude, ',');
        std::getline(stream, latitude, ',');

        FlightData entry;
        entry.time = time;
        entry.speed = speed.empty() ? std::nullopt : std::optional<double>(std::stod(speed));
        entry.altitude = altitude.empty() ? std::nullopt : std::optional<double>(std::stod(altitude));
        entry.longitude = std::stod(longitude);
        entry.latitude = std::stod(latitude);

        data.push_back(entry);
    }

    return data;
}
