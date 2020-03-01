#ifndef IMAGE_H_DEFINED
#define IMAGE_H_DEFINED

#include "vector3.h"
#include <vector>

/**
 * Class defining an image to render stuff on.
 */

class Image{
 public:
  Image(int width, int height);
  Vec3f getPixel(int x, int y);
  int setPixel(int x, int y, Vec3f color);
  int saveAsPPM();
  int width();
  int height();
 private:
  int CoordsToFramebuffer(int x, int y);
  int width_,height_;
  std::vector<Vec3f> framebuffer_;
};

#endif 
