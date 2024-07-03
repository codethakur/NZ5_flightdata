#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <gdal_priv.h>
#include <ogr_spatialref.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include <sstream>

// Include your header file for FlightData
#include "FlightData.h"

// Function to load flight data from a CSV file
std::vector<FlightData> loadFlightData(const std::string& filename) {
    std::vector<FlightData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        std::istringstream s(line);
        std::string timeStr, speedStr, altStr, longStr, latStr;

        std::getline(s, timeStr, ',');
        std::getline(s, speedStr, ',');
        std::getline(s, altStr, ',');
        std::getline(s, longStr, ',');
        std::getline(s, latStr, ',');

        std::time_t time = FlightData::parseTime(timeStr);
        std::optional<double> speed;
        if (!speedStr.empty()) speed = std::stod(speedStr);
        double altitude = std::stod(altStr);
        double longitude = std::stod(longStr);
        double latitude = std::stod(latStr);

        data.emplace_back(time, speed, altitude, longitude, latitude);
    }
    file.close();
    return data;
}

// Function to load GeoTIFF image using GDAL
cv::Mat loadGeoTIFF(const std::string& filePath) {
    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(filePath.c_str(), GA_ReadOnly));
    if (!dataset) {
        std::cerr << "Failed to load GeoTIFF file!" << std::endl;
        return cv::Mat();
    }

    GDALRasterBand* band = dataset->GetRasterBand(1);
    int width = band->GetXSize();
    int height = band->GetYSize();

    cv::Mat image(height, width, CV_8UC1);
    band->RasterIO(GF_Read, 0, 0, width, height, image.data, width, height, GDT_Byte, 0, 0);

    GDALClose(dataset);
    return image;
}

// Function to convert geographic coordinates to image pixel coordinates
cv::Point2f geoToPixel(double lon, double lat, GDALDataset* dataset) {
    double geoTransform[6];
    dataset->GetGeoTransform(geoTransform);

    double x = geoTransform[0] + lon * geoTransform[1] + lat * geoTransform[2];
    double y = geoTransform[3] + lon * geoTransform[4] + lat * geoTransform[5];

    return cv::Point2f(static_cast<float>(x), static_cast<float>(y));
}

int main() {
    // Load flight data
    std::string flightDataFile = "NZ5_flightdatar.csv";
    std::vector<FlightData> flightData = loadFlightData(flightDataFile);

    if (flightData.empty()) {
        std::cerr << "No flight data loaded!" << std::endl;
        return -1;
    }

    // Load the GeoTIFF image
    std::string geoTiffFile = "world.tif";
    cv::Mat image = loadGeoTIFF(geoTiffFile);
    if (image.empty()) {
        std::cerr << "Failed to load GeoTIFF image!" << std::endl;
        return -1;
    }

    // Load GDAL dataset for coordinate transformation
    GDALAllRegister();
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(geoTiffFile.c_str(), GA_ReadOnly));
    if (!dataset) {
        std::cerr << "Failed to open GDAL dataset!" << std::endl;
        return -1;
    }

    // Draw trajectory on the image
    for (size_t i = 1; i < flightData.size(); ++i) {
        cv::Point2f p1 = geoToPixel(flightData[i - 1].longitude, flightData[i - 1].latitude, dataset);
        cv::Point2f p2 = geoToPixel(flightData[i].longitude, flightData[i].latitude, dataset);

        cv::line(image, p1, p2, cv::Scalar(0, 255, 255), 2); // Draw a yellow line
    }

    // Save the image with the drawn trajectory
    cv::imwrite("flight_trajectory.png", image);
    std::cout << "Flight trajectory saved as flight_trajectory.png" << std::endl;

    // Cleanup GDAL dataset
    GDALClose(dataset);

    return 0;
}
