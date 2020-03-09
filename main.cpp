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
 * Transform .obj coordinates [-1,1] into discrete image coordinates [0,width|height]
 */
Vec2i CObjToImage(float x, float y){
  int x0 = (x+1.)*width/2.;
  int y0 = (y+1.)*height/2.;
  return Vec2i(x0,y0);
}

Vec2i CObjToImage(Vec3f v){
  int x0 = (v.x+1.)*width/2.;
  int y0 = (v.y+1.)*height/2.;
  return Vec2i(x0,y0);
}

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
 * Draw a triangle, filled with given color.
 */
void bayesian_triangle(Vec2i p0, Vec2i p1, Vec2i p2, Image *image, Vec3f color){
  Triangle2i tri(p0,p1,p2);
  Vec2i bbox_clamp(image->width()-1,image->height()-1);
  Vec2i bbox_min(image->width()-1,image->height()-1);
  Vec2i bbox_max(0,0);
  for(int i=0 ; i < 3 ; i++){
    for(int j=0; j < 2 ; j++){
      bbox_min[j]=std::max(0,std::min(bbox_min[j],tri[i][j]));
      bbox_max[j]=std::min(bbox_clamp[j],std::max(bbox_max[j],tri[i][j]));
    }
  }
  Vec2i v;
  for(v.x=bbox_min.x;v.x<=bbox_max.x;v.x++){
    for(v.y=bbox_min.y;v.y<=bbox_max.y;v.y++){
      Vec3f bc = tri.barycentric(v);
      if(bc.x<0||bc.y<0||bc.z<0) continue;
      image->setPixel(v.x,v.y,color);
    }
  }
}

void scanlines_triangle(Vec2i p0, Vec2i p1, Vec2i p2, Image *image, Vec3f color){
  if(p0.y==p1.y && p0.y==p2.y) return;
  if(p0.y > p1.y) std::swap(p0,p1);
  if(p0.y > p2.y) std::swap(p0,p2);
  if(p1.y > p2.y) std::swap(p1,p2);
  int height = p2.y-p0.y;
  for(int y=0;y<height;y++){
    int lowerHalf = !(y > p1.y-p0.y || p1.y==p0.y);
    int segmentHeight = (lowerHalf ? p1.y-p0.y : p2.y-p1.y);
    float alpha = (float)y/height;
    float beta = (float)(y-(lowerHalf ? 0 : p1.y-p0.y))/segmentHeight;
    Vec2i A = p0 + (p2-p0) * alpha;
    Vec2i B = (lowerHalf ? p0 + (p1-p0) * beta : p1 + (p2-p1) * beta);
    if(A.x>B.x) std::swap(A,B);
    for(int x=A.x;x<B.x;x++){
      image->setPixel(x,p0.y+y,color);
    }
  }
}

void triangle(Vec2i p0, Vec2i p1, Vec2i p2, Image *image, Vec3f color){
  //line(p0,p1,image,Vec3f(0.5,0.5,0.5));
  //line(p2,p1,image,Vec3f(0.5,0.5,0.5));
  //line(p0,p2,image,Vec3f(0.5,0.5,0.5));
  bayesian_triangle(p0,p1,p2,image,color);
}

/**
 * Rendering function.
 */
void render() {
  Image* framebuffer = new Image(width,height);
  Vec3f light_dir(0,0,-1);
  //Render the model
  for(int f=0;f<model->nfaces();f++){
    std::vector<int> face = model->getFaceVertexes(f);
    Vec3f world_coords[3];
    Vec2i screen_coords[3];
    for(int i=0;i<3;i++){
      world_coords[i]=model->getVertex(face[i]);
      screen_coords[i]=CObjToImage(world_coords[i]);
    }
    Vec3f n = cross(world_coords[2]-world_coords[0],world_coords[1]-world_coords[0]);
    n.normalize();
    float intensity = n*light_dir;
    if(intensity<=0) continue;
    triangle(screen_coords[0],screen_coords[1],screen_coords[2],framebuffer,Vec3f(1*intensity,1*intensity,1*intensity));
  }

  //Test renders
  //triangle(Vec2i(180,50),Vec2i(150,1),Vec2i(70,180),framebuffer,Vec3f(1,0.5,0.5));

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

