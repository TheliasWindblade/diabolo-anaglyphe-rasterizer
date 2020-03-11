#include "image.h"
#include <iostream>
#include <fstream>

Image::Image(int width, int height) : width_(width),height_(height){
  framebuffer_ = new Vec3f[width*height];
  for(int i = 0 ; i < width*height ; i++){
    framebuffer_[i] = Vec3f(0.f,0.f,0.f);
  }
}

Vec3f Image::getPixel(int x, int y) { return framebuffer_[CoordsToFramebuffer(x,y)];}
Vec3f Image::getPixel(Vec2i vec) { return getPixel(vec.x,vec.y);}

int Image::setPixel(int x, int y, Vec3f color){
  if(x>=0 && x<width() && y>=0 && y<height()){
    framebuffer_[CoordsToFramebuffer(x,y)]=color;
    return 0;
  }
  return -1;
}

int Image::setPixel(Vec2i vec, Vec3f color){return setPixel(vec.x,vec.y,color);}

int Image::width() {return width_;}
int Image::height() {return height_;}

int Image::saveAsPPM(const char* filename){
  flip_vert();
  std::ofstream ofs; // save the framebuffer to file
  ofs.open(filename,std::ios::binary);
  ofs << "P6\n" << width() << " " << height() << "\n255\n";
  for (int y = 0 ; y < height() ; y++) {
    for(int x = 0 ; x < width() ; x++){
      Vec3f c = getPixel(x,y);
      float max = std::max(c[0], std::max(c[1], c[2]));
      if (max>1) setPixel(x,y,c*(1./max));
      c = getPixel(x,y);
      for (size_t j = 0; j<3; j++) {
	ofs << (char)(255 * std::max(0.f, std::min(1.f, c[j])));
      }
    }
  }
  ofs.close();
  return 0;
}

void Image::flip_vert(){
  for(int y = 0 ; y < height()/2 ; y++){
    for(int x = 0 ; x < width() ; x++){
      std::swap(framebuffer_[CoordsToFramebuffer(x,y)],framebuffer_[CoordsToFramebuffer(x,height()-y-1)]);
    }
  }
}

int Image::CoordsToFramebuffer(int x, int y){return x+y*width();}

