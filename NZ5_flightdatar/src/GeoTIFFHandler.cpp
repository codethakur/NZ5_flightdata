#include <gdal_priv.h>
#include <iostream>

cv::Mat loadGeoTIFF(const std::string& filePath) {
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
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
