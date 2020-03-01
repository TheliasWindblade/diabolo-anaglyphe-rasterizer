#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(int width, int height) : width_(width),height_(height),framebuffer_(width*height) {}
Vec3f Image::getPixel(int x, int y) { return framebuffer_[CoordsToFramebuffer(x,y)];}

int Image::setPixel(int x, int y, Vec3f color){
  if(x>=0 && x<width() && y>=0 && y<height()){
    framebuffer_[CoordsToFramebuffer(x,y)]=color;
    return 0;
  }
  return -1;
}

int Image::width() {return width_;}
int Image::height() {return height_;}

int Image::saveAsPPM(){
  std::ofstream ofs; // save the framebuffer to file
  ofs.open("./out.ppm",std::ios::binary);
  ofs << "P6\n" << width() << " " << height() << "\n255\n";
  for (int y = height() ; y > 0 ; --y) {
    for(int x = 0 ; x < width() ; x++){
      Vec3f c = getPixel(x,y);
      float max = std::max(c[0], std::max(c[1], c[2]));
      if (max>1) setPixel(x,y,c*(1./max));
      for (size_t j = 0; j<3; j++) {
	ofs << (char)(255 * std::max(0.f, std::min(1.f, getPixel(x,y)[j])));
      }
    }
  }
  ofs.close();
  return 0;
}

int Image::CoordsToFramebuffer(int x, int y){return x+y*width();}
