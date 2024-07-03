#ifndef FLIGHTDATA_H
#define FLIGHTDATA_H

#include <optional>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

struct FlightData {
    std::time_t time;
    std::optional<double> speed;
    double altitude;
    double longitude;
    double latitude;

    FlightData(std::time_t t, std::optional<double> s, double alt, double lon, double lat)
        : time(t), speed(s), altitude(alt), longitude(lon), latitude(lat) {}

    static std::time_t parseTime(const std::string& timeStr) {
        std::tm t = {};
        std::istringstream ss(timeStr);
        ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
        if (ss.fail()) {
            std::cerr << "Failed to parse time: " << timeStr << std::endl;
            return {};
        }
        return std::mktime(&t);
    }
};

#endif // FLIGHTDATA_H
