#include "canvas.h"
#include <cstring>
/*
  cv::Mat draw_image;
  float resolution;
  Isometry2f _w2c=Isometry2f(0,0,0); // world to canvas;
  Isometry2f _iw2c=Isometry2f(0,0,0); // canvas_to_world;
  Vec2f canvas_origin=Vec2f(0,0);
*/
void Canvas::init(int r,
          int c, 
          float res) {
  draw_image=cv::Mat(r, c, CV_8UC1);
  clear();
  canvas_origin=Vec2f(r/2, c/2);
  resolution=res;
  _c2w.t=-canvas_origin*resolution;
  _c2w.R.setIdentity();
  _c2w.R=_c2w.R.scale(resolution);
  _w2c.R.setIdentity();
  _w2c.R=_w2c.R.scale(1./resolution);
  _w2c.t=canvas_origin;
}

void Canvas::clear() {
  memset(draw_image.data, 0, draw_image.rows*draw_image.cols*sizeof(uint8_t));
}

void Canvas::drawCircle(const Vec2f& center, const float radius, const uint8_t gray_value) {
  Vec2f c_center=w2c(center);
  float c_radius=radius/resolution;
  // cerr << "circle w_center: " << center << " w_radius: " << radius << endl;
  // cerr << "circle c_center: " << c_center << " c_radius: " << c_radius << endl;
  cv::circle(draw_image, cv::Point(c_center.y(), c_center.x()), c_radius, cv::Scalar(gray_value));
}

void Canvas::drawPoint(const Vec2f& pos, const uint8_t gray_value) {
  Vec2f c_pos=w2c(pos);
  if (c_pos.x() <0
      || c_pos.x()>=rows()
      || c_pos.y()<0
      || c_pos.y()>cols())
    return;
  draw_image.at<uint8_t>(c_pos.x(), c_pos.y())=gray_value;
}

void Canvas::drawLine(const Vec2f& p_start, const Vec2f& p_end, const uint8_t gray_value) {
  Vec2f c_start=w2c(p_start);
  Vec2f c_end=w2c(p_end);
  // cerr << "line w_start: " << p_start << " w_end: " << p_end << endl;
  // cerr << "line c_start: " << c_start << " c_end: " << c_end << endl;
  cv::line(draw_image,
           cv::Point(c_start.y(), c_start.x()),
           cv::Point(c_end.y(), c_end.x()),
           cv::Scalar(gray_value), 1);
}

void Canvas::show(){
  // cerr << "canvas_size: " << rows() << " " << cols() << endl;
  // cerr << "canvas_center: " << canvas_origin << endl;
  // cerr << "canvas_R: " << _w2c.R;
  // cerr << "canvas_t: " << _w2c.t;
  
  cv::imshow("canvas", draw_image);
}
