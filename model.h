#ifndef MODEL_H_DEFINED
#define MODEL_H_DEFINED

#include "tgaimage.h"
#include "vectors.h"
#include <vector>

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
  std::vector<int> getFaceVertexes(int id);
  std::vector<int> getFaceTextureVertices(int id);
  void print();
private:
  std::vector<Vec3f> vertices_;
  std::vector<Vec2f> texture_vertices_;
  std::vector<std::vector<int>> faces_;
  TGAImage texture_;
};


#endif
