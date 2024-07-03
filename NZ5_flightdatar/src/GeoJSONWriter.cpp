#include "GeoJSONWriter.h"
#include <fstream>
#include "json.hpp" // Using nlohmann/json library

void writeGeoJSON(const std::vector<FlightData>& data, const std::string& filePath) {
    nlohmann::json geoJSON;
    geoJSON["type"] = "FeatureCollection";
    geoJSON["features"] = nlohmann::json::array();

    for (const auto& entry : data) {
        nlohmann::json feature;
        feature["type"] = "Feature";
        feature["geometry"] = {
            {"type", "Point"},
            {"coordinates", {entry.longitude, entry.latitude}}
        };
        feature["properties"] = {
            {"time", entry.time},
            {"speed", entry.speed ? *entry.speed : nullptr},
            {"altitude", entry.altitude ? *entry.altitude : nullptr}
        };
        geoJSON["features"].push_back(feature);
    }

    std::ofstream file(filePath);
    file << geoJSON.dump(4);
}
