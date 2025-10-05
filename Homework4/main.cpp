#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) {
  if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) {
    std::cout << "Left button of the mouse is clicked - position (" << x << ", "
              << y << ")" << '\n';
    control_points.emplace_back(x, y);
  }
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) {
  auto &p_0 = points[0];
  auto &p_1 = points[1];
  auto &p_2 = points[2];
  auto &p_3 = points[3];

  for (double t = 0.0; t <= 1.0; t += 0.001) {
    auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

    window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
  }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points,
                             float t) {
  if (control_points.size() == 1) {
    return control_points[0];
  }
  std::vector<cv::Point2f> next_points;
  for (size_t i = 0; i < control_points.size() - 1; ++i) {
    cv::Point2f point = (1 - t) * control_points[i] + t * control_points[i + 1];
    next_points.push_back(point);
  }
  return recursive_bezier(next_points, t);
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) {
  for (double t = 0.0; t <= 1.0; t += 0.001) {
    auto point = recursive_bezier(control_points, t);

    // 反走样实现
    int x0 = std::floor(point.x);
    int y0 = std::floor(point.y);

    // 遍历周围的 2x2 或 3x3 像素
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        int px = x0 + dx;
        int py = y0 + dy;

        // 边界检查
        if (px < 0 || px >= window.cols || py < 0 || py >= window.rows) {
          continue;
        }

        // 计算像素中心坐标
        cv::Point2f pixel_center(px + 0.5f, py + 0.5f);

        // 计算点到像素中心的距离
        float distance = cv::norm(point - pixel_center);

        // 根据距离计算权重（距离越近，颜色越亮）
        // 可以使用不同的衰减函数
        float max_distance = std::sqrt(2.0f); // 对角线距离
        float ratio = 1.0f - (distance / max_distance);
        ratio = std::max(0.0f, ratio); // 确保非负

        // 混合颜色（使用最大值避免覆盖）
        cv::Vec3b &color = window.at<cv::Vec3b>(py, px);
        color[1] = std::max(color[1], static_cast<uchar>(255 * ratio));
      }
    }
  }
}

int main() {
  cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
  cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
  cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

  cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

  int key = -1;
  while (key != 27) {
    for (auto &point : control_points) {
      cv::circle(window, point, 3, {255, 255, 255}, 3);
    }

    if (control_points.size() == 4) {
      // naive_bezier(control_points, window);
      bezier(control_points, window);

      cv::imshow("Bezier Curve", window);
      cv::imwrite("my_bezier_curve.png", window);
      key = cv::waitKey(0);

      return 0;
    }

    cv::imshow("Bezier Curve", window);
    key = cv::waitKey(20);
  }

  return 0;
}
