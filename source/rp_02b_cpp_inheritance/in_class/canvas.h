#pragma once
#include <opencv2/opencv.hpp>
#include "linalg.h"

struct Canvas {
  cv::Mat draw_image;
  float resolution=1;
  Isometry2f _w2c=Isometry2f(0,0,0); // world to canvas;
  Isometry2f _c2w=Isometry2f(0,0,0); // canvas_to_world;
  Vec2f canvas_origin=Vec2f(0,0);

  void init(int r,
            int c, 
            float res);
  inline int rows() const {return draw_image.rows;}
  inline int cols() const {return draw_image.cols;}
  inline Vec2f w2c(const Vec2f& wp) const { return _w2c*wp;}
  inline Vec2f c2w(const Vec2f& cp) const { return _c2w*cp;}
  inline bool inside(const Vec2f& wp) {
    Vec2f cp=w2c(wp);
    return cp.x()<0
      || cp.x()>=draw_image.rows
      || cp.y()<=0
      || cp.y()>=draw_image.cols;
  }
  void clear();
  void drawCircle(const Vec2f& center, const float radius, const uint8_t gray_value);
  void drawPoint(const Vec2f& pos, const uint8_t gray_value);
  void drawLine(const Vec2f& p_start, const Vec2f& p_end, const uint8_t gray_value);
  void show();
};
