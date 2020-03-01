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
#include "model.h"
#include "image.h"

const int   width    = 800;
const int   height   = 800;
Model* model=NULL;

/**
 * Draw a line between (x0,y0) and (x1,y1). Who could've guessed.
 */
void line(int x0, int y0, int x1, int y1, Image *image, Vec3f color){
  int xFor = true;
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
      int x1 = (v1.x+1.)*width/2.;
      int y1 = (v1.y+1.)*height/2.;
      line(x0,y0,x1,y1,framebuffer,Vec3f(1,0,1));
    }
  }

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

