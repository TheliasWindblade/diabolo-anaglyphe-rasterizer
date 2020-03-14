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
#include "tgaimage.h"

const int   width    = 800;
const int   height   = 800;
const float sqt = 1.73;
const float pi = 3.14;
Model* model=NULL;

/**
 * Return a TGAColor from a Vec3f describing a color in the HSV system.
 */
TGAColor HSVToRGB(TGAColor color){
  float x = color[1]*std::cos(color[0]);
  float y = color[1]*std::sin(color[0]);
  return TGAColor(x*2./3+color[2],-x*1./3+y*1/sqt+color[2],-x*1./3-y*1./sqt+color[2]);
}

/**
 * Transform .obj coordinates [-1,1] into discrete image coordinates [0,width|height]
 */
Vec3f CObjToImage(Vec3f v){
  int x0 = ((v.x+1.)*width/2.+.5);
  int y0 = ((v.y+1.)*height/2.+.5);
  float z0 = (v.z+1.)/2;
  return Vec3f(x0,y0,z0);
}

Vec2f CObjToImage(Vec2f v){
  int x0 = ((v.x+1.)*width/2.+.5);
  int y0 = ((v.y+1.)*height/2.+.5);
  return Vec2f(x0,y0);
}

Vec2f CObjToImage(Vec2f v, Vec2i clamp){
  int x0 = ((v.x+1.)*clamp.x/2.+.5);
  int y0 = ((v.y+1.)*clamp.y/2.+.5);
  return Vec2f(x0,y0);
}

/**
 * Convert 2D image coordinates[0,width|height] to 1D framebuffer coordinates [0,width*height]
 */
int CImageToZBuffer(int x, int y){
  return x*height+y;
}

int CImageToZBuffer(Vec3f v){
  return v.x*height+v.y;
}


/**
 * Draw a line between (x0,y0) and (x1,y1). Who could've guessed.
 */
void line(Vec2i p0, Vec2i p1, TGAImage *image, TGAColor color){
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
      image->set(x,y,color);
    } else {
      image->set(y,x,color);
    }
  }
}

void line(Vec3f p0, Vec3f p1, TGAImage *image, TGAColor color){
  line(Vec2i(p0.x,p0.y),Vec2i(p1.x,p1.y),image,color);
}

/**
 * Draw a triangle, filled with given color.
 */
void bayesian_triangle(TexturedTriangle triangle, TGAImage *image, float *zbuffer, TGAColor color){
  Triangle3f tri = triangle.worldVertices;
  Triangle2f tex = triangle.textureVertices;
  Vec2f bbox_clamp(image->get_width()-1.,image->get_height()-1.);
  Vec2f bbox_min(std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
  Vec2f bbox_max(std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
  for(int i=0 ; i < 3 ; i++){
    for(int j=0; j < 2 ; j++){
      bbox_min[j]=std::max(0.f,std::min(bbox_min[j],tri[i][j]));
      bbox_max[j]=std::min(bbox_clamp[j],std::max(bbox_max[j],tri[i][j]));
    }
  }
  Vec3f v;
  for(v.x=bbox_min.x;v.x<=bbox_max.x;v.x++){
    for(v.y=bbox_min.y;v.y<=bbox_max.y;v.y++){
      Vec3f bc = tri.barycentric(v);
      if(bc.x<0||bc.y<0||bc.z<0) continue;
      v.z=0;
      for(int i=0;i<3;i++) { v.z += tri[i].z*bc[i]; }
      if(zbuffer[CImageToZBuffer(v)]<v.z){
	zbuffer[CImageToZBuffer(v)]=v.z;
	Vec2f ratios = Vec2f(tex.v0.x*bc[0]+tex.v1.x*bc[1]+tex.v2.x*bc[2],tex.v0.y*bc[0]+tex.v1.y*bc[1]+tex.v2.y*bc[2]);
	image->set(v.x,v.y,model->getDiffuseAt(ratios)*(color[0]/255.));
      }
    }
  }
}

//DEPRECATED : Render a triangle using the old scanlines method.
void scanlines_triangle(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage *image, TGAColor color){
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
      image->set(x,p0.y+y,color);
    }
  }
}

//Render a triangle in wireframe.
void wireframe(Triangle3f tri,TGAImage *image, TGAColor color){
  line(tri.v0,tri.v1,image,color);
  line(tri.v1,tri.v2,image,color);
  line(tri.v0,tri.v2,image,color);
}

/**
 * Rendering function.
 */
void render() {
  TGAImage* framebuffer = new TGAImage(width,height,3);
  float zbuffer[width*height];
  for(int i=0;i<width*height;i++){
    zbuffer[i]=std::numeric_limits<float>::min();
  }
  Vec3f light_dir(0,0,-1);
  //Render the model
  for(int f=0;f<model->nfaces();f++){
    FaceData face = model->getFaceData(f);
    Vec3f screen_coords[3];
    Vec3f world_coords[3];
    Vec2f tex_coords[3];
    for(int i=0;i<3;i++){
      Vec3f v = model->getVertex(face.vertices[i]);
      screen_coords[i] = CObjToImage(v);
      world_coords[i]=v;
      tex_coords[i] = model->getTextureVertex(face.texture_vertices[i]);
    }
    Vec3f n = cross(world_coords[2]-world_coords[0],world_coords[1]-world_coords[0]);
    n.normalize();
    float intensity = n*light_dir;
    if(intensity<=0) continue;
    TGAColor color = TGAColor(255,255,255);
    //TGAColor color = HSVToRGB(TGAColor(((rand()%255)/255.)*2.*pi,0.5+(rand()%16)/32.,0.25));
    color=color*intensity;
    TexturedTriangle tri(Triangle3f(screen_coords[0],screen_coords[1],screen_coords[2]),Triangle2f(tex_coords[0],tex_coords[1],tex_coords[2]));
    bayesian_triangle(tri,framebuffer,zbuffer,color);
    //wireframe(tri,framebuffer,Vec3f(0.66,0.13,0.45));
  }

  //Print z-buffer
  TGAImage* zbuffer_r = new TGAImage(width,height,3);
  for(int y=0 ; y < height ; y ++){
    for(int x=0 ; x < width ; x ++){
      float c = zbuffer[CImageToZBuffer(x,y)]*255;
      zbuffer_r->set(x,y,TGAColor(c,c,c));
    }
  }
  
  //Save the images
  framebuffer->flip_vertically();
  zbuffer_r->flip_vertically();
  framebuffer->write_tga_file("out.tga",0);
  zbuffer_r->write_tga_file("z.tga",0);

  delete model;
  delete framebuffer;
}

int main(int argc, char* argv[]) {
  if(argc==2){
    model = new Model(argv[1]);
  } else {
    model = new Model("diablo3_pose");
  }
  render();
  return 0;
}

