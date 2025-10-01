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
    u = std::clamp(u, 0.0f, 1.0f);
    v = std::clamp(v, 0.0f, 1.0f);
    auto u_img = std::min(static_cast<int>(u * width), width - 1);
    auto v_img = std::min(static_cast<int>((1 - v) * height), height - 1);
    auto color = image_data.at<cv::Vec3b>(v_img, u_img);
    return Eigen::Vector3f(color[0], color[1], color[2]);
  }
};
#endif // RASTERIZER_TEXTURE_H
