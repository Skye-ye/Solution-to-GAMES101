//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include <Eigen/Core>
#include <opencv2/opencv.hpp>
class Texture {
private:
  cv::Mat image_data;

public:
  Texture(const std::string &name) {
    image_data = cv::imread(name);
    cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
    width = image_data.cols;
    height = image_data.rows;
  }

  int width, height;

  Eigen::Vector3f getColor(float u, float v) {
    auto u_img = u * width;
    auto v_img = (1 - v) * height;
    int u_clamped = std::clamp(static_cast<int>(u_img), 0, width - 1);
    int v_clamped = std::clamp(static_cast<int>(v_img), 0, height - 1);
    auto color = image_data.at<cv::Vec3b>(v_clamped, u_clamped);
    return Eigen::Vector3f(color[0], color[1], color[2]);
  }

  Eigen::Vector3f getColorBilinear(float u, float v) {
    float u_img = u * width;
    float v_img = (1 - v) * height;

    // Get integer coordinates (clamp to valid range)
    int u0 = std::clamp(static_cast<int>(std::floor(u_img)), 0, width - 1);
    int u1 = std::clamp(static_cast<int>(std::ceil(u_img)), 0, width - 1);
    int v0 = std::clamp(static_cast<int>(std::floor(v_img)), 0, height - 1);
    int v1 = std::clamp(static_cast<int>(std::ceil(v_img)), 0, height - 1);

    // Get fractional parts for interpolation
    float s = u_img - u0; // horizontal interpolation weight
    float t = v_img - v0; // vertical interpolation weight

    // Sample four corner pixels
    auto c00 = image_data.at<cv::Vec3b>(v0, u0); // top-left
    auto c10 = image_data.at<cv::Vec3b>(v0, u1); // top-right
    auto c01 = image_data.at<cv::Vec3b>(v1, u0); // bottom-left
    auto c11 = image_data.at<cv::Vec3b>(v1, u1); // bottom-right

    // Bilinear interpolation
    auto top = (1 - s) * c00 + s * c10;
    auto bottom = (1 - s) * c01 + s * c11;
    auto color = (1 - t) * top + t * bottom;

    return Eigen::Vector3f(color[0], color[1], color[2]);
  }
};
#endif // RASTERIZER_TEXTURE_H
