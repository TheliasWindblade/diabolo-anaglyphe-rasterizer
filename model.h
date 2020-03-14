#ifndef MODEL_H_DEFINED
#define MODEL_H_DEFINED

#include "tgaimage.h"
#include "vectors.h"
#include <vector>

struct FaceData {
  std::vector<int> vertices;
  std::vector<int> texture_vertices;
};

/**
 * Class defining a 3D-model, read from a .obj file.
 */

class Model {

public:
  Model(const char *filename);
  int nverts();
  int nfaces();
  Vec3f getVertex(int id);
  Vec2f getTextureVertex(int id);
  TGAColor getDiffuseAt(Vec2f uvf);
  FaceData getFaceData(int id);
  Vec2i getTextureSize();
private:
  std::vector<Vec3f> vertices_;
  std::vector<Vec2f> texture_vertices_;
  std::vector<FaceData> faces_;
  TGAImage texture_;
};


#endif
