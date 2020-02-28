//
// Created by Alexis on 29/01/2020.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "vector3.h"

/**
 * Draw a line between (x0,y0) and (x1,y1). Who could've guessed.
 */
void line(int x0, int y0, int x1, int y1, std::vector<Vec3f> &image, Vec3f color){
  int xFor = true;
  //Steep check
  if(std::abs(x1-x0) < std::abs(y1-y0)){
    xFor=false;
    std::swap(x0,y0);
    std::swap(x1,y1);
  }
  //Forcing x0 <= x1
  if(x0>x1) {std::swap(x0,x1); std::swap(y0,y1);}
  //Line render
  for(int x=x0; x<=x1; x++){
    float t = (x-x0)/(float)(x1-x0);
    int y = y0 + (y1-y0)*t;
    image[xFor?x+y*1024:x*1024+y]=color;
  }
}

/**
 * Rendering function.
 */
void render() {
    const int   width    = 1024;
    const int   height   = 768;
    const float fov      = M_PI/3.;
    std::vector<Vec3f> framebuffer(width*height);

    line(0,0,1023,767,framebuffer,Vec3f(1,1,1));
    line(0,767,264,0,framebuffer,Vec3f(1,0,1));
    
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm",std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height*width; ++i) {
        Vec3f &c = framebuffer[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max>1) c = c*(1./max);
        for (size_t j = 0; j<3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}

int main() {
    render();
    return 0;
}

