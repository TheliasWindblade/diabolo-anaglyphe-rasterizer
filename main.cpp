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
#include "vectors.h"
#include "model.h"
#include "image.h"

const int   width    = 800;
const int   height   = 800;
Model* model=NULL;

/**
 * Draw a line between (x0,y0) and (x1,y1). Who could've guessed.
 */
void line(Vec2i p0, Vec2i p1, Image *image, Vec3f color){
  int xFor = true;
  int x0=p0.x,y0=p0.y;
  int x1=p1.x,y1=p1.y;
  //Steepness check
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
    //De-transpose if steep
    if(xFor){
      image->setPixel(x,y,color);
    } else {
      image->setPixel(y,x,color);
    }
  }
}

/**
 * Draw a triangle. With 3 lines.
 */
void triangle(Vec2i p0, Vec2i p1, Vec2i p2, Image *image, Vec3f color){
  if(p0.y > p1.y) std::swap(p0,p1);
  if(p0.y > p2.y) std::swap(p0,p2);
  if(p1.y > p2.y) std::swap(p1,p2);
  int height = p2.y-p0.y;
  for(int y=0;y<height;y++){
    int lowerHalf = !(y > p1.y-p0.y || p1.y==p0.y);
    int segmentHeight = (lowerHalf ? p1.y-p0.y : p2.y-p1.y)+1;
    float alpha = (float)y/height;
    float beta = (float)(y-(lowerHalf ? 0 : p1.y-p0.y))/segmentHeight;
    Vec2i A = p0 + (p2-p0) * alpha;
    Vec2i B = (lowerHalf ? p0 + (p1-p0) * beta : p1 + (p2-p1) * beta);
    A.y=p0.y+y;
    line(A,B,image,Vec3f(0.7,0.5,1));
    image->setPixel(A,Vec3f(0,1,0));
    image->setPixel(B,Vec3f(1,0,0));
  }
  line(p0,p1,image,Vec3f(0.6,0.6,0.6));
  line(p2,p1,image,Vec3f(0.6,0.6,0.6));
  line(p0,p2,image,Vec3f(0.6,0.6,0.6));
}

/**
 * Rendering function.
 */
void render() {
  Image* framebuffer = new Image(width,height);
  
  //Render the model.
  for(int f=0;f<model->nfaces();f++){
    std::vector<int> face = model->getFaceVertexes(f);
    for(int i=0;i<3;i++){
      Vec3f v0 = model->getVertex(face[i]);
      Vec3f v1 = model->getVertex(face[(i+1)%3]);
      int x0 = (v0.x+1.)*width/2.;
      int y0 = (v0.y+1.)*height/2.;
      Vec2i p0(x0,y0);
      int x1 = (v1.x+1.)*width/2.;
      int y1 = (v1.y+1.)*height/2.;
      Vec2i p1(x1,y1);
      line(p0,p1,framebuffer,Vec3f(1,0,1));
    }
  }

  //Test renders
  triangle(Vec2i(180,50),Vec2i(150,1),Vec2i(70,180),framebuffer,Vec3f(1,0.5,0.5));

  //Save the image.
  framebuffer->saveAsPPM();

  delete model;
  delete framebuffer;
}

int main(int argc, char* argv[]) {
  if(argc==2){
    model = new Model(argv[1]);
  } else {
    model = new Model("obj/diablo3_pose.obj");
  }
  render();
  return 0;
}

