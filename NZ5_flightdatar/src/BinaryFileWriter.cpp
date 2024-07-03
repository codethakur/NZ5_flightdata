#include "BinaryFileWriter.h"
#include <fstream>
#include <algorithm>
#include <future>
#include <cstring>

struct BinaryFlightData {
    uint32_t time; // UNIX timestamp
    uint16_t speed; // knots
    uint16_t altitude; // feet
    int32_t longitude; // microdegrees
    int32_t latitude; // microdegrees
};

void writeBinaryFile(const std::vector<FlightData>& data, const std::string& filePath, const std::string& sortKey) {
    auto sortedData = data;

    if (sortKey == "time") {
        std::sort(sortedData.begin(), sortedData.end(), [](const FlightData& a, const FlightData& b) {
            return a.time < b.time;
            });
    }
    else if (sortKey == "speed") {
        std::sort(sortedData.begin(), sortedData.end(), [](const FlightData& a, const FlightData& b) {
            return a.speed.value_or(0) < b.speed.value_or(0);
            });
    }
    else if (sortKey == "altitude") {
        std::sort(sortedData.begin(), sortedData.end(), [](const FlightData& a, const FlightData& b) {
            return a.altitude.value_or(0) < b.altitude.value_or(0);
            });
    }

    std::ofstream file(filePath, std::ios::binary);

    for (size_t i = 0; i < std::min(sortedData.size(), static_cast<size_t>(400)); ++i) {
        BinaryFlightData binaryData;
        binaryData.time = static_cast<uint32_t>(std::time(nullptr)); // Placeholder for actual conversion
        binaryData.speed = static_cast<uint16_t>(sortedData[i].speed.value_or(0));
        binaryData.altitude = static_cast<uint16_t>(sortedData[i].altitude.value_or(0));
        binaryData.longitude = static_cast<int32_t>(sortedData[i].longitude * 1e6);
        binaryData.latitude = static_cast<int32_t>(sortedData[i].latitude * 1e6);

        file.write(reinterpret_cast<char*>(&binaryData), sizeof(BinaryFlightData));
    }
}

int main() {
    auto data = readCSV("NZ5_flightdatar.csv");

    auto writeFile1 = std::async(std::launch::async, writeBinaryFile, data, "file1.bin", "time");
    auto writeFile2 = std::async(std::launch::async, writeBinaryFile, data, "file2.bin", "speed");
    auto writeFile3 = std::async(std::launch::async, writeBinaryFile, data, "file3.bin", "altitude");

    writeFile1.wait();
    writeFile2.wait();
    writeFile3.wait();

    return 0;
}
