#include "model.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Model::Model(const char *filename) : vertices_(), texture_vertices_(), faces_(), texture_() {
  std::ifstream in;
  std::string line;
  std::string fn("./obj/");
  fn+=std::string(filename);
  in.open((fn+std::string(".obj")).c_str(), std::ifstream::in);
  std::cout << fn << std::endl;
  if(in.fail()) return;
  while(!in.eof()){
    std::getline(in,line);
    std::istringstream iss(line.c_str());
    char trash;
    if(!line.compare(0,2,"v ")){
      iss >> trash;
      Vec3f v;
      for(int i=0; i<3; i++) iss >> v[i];
      vertices_.push_back(v);
    }
    else if(!line.compare(0,3,"vt ")){
      iss >> trash;
      Vec2f v;
      for(int i=0; i<2; i++) iss >> v[i];
      texture_vertices_.push_back(v);
    }
    else if(!line.compare(0,2,"f ")){
      FaceData f;
      int itrash, id, idtex;
      iss >> trash;
      while(iss >> id >> trash >> idtex >> trash >> itrash){
	id--;
	f.vertices.push_back(id);
	f.texture_vertices.push_back(idtex);
      }
      faces_.push_back(f);
    }
  }
  texture_.read_tga_file((fn+std::string("_diffuse.tga")).c_str());
  std::cerr << "## vertices : " << vertices_.size() << " ## texture vertices : " << texture_vertices_.size() << " ## faces : " << faces_.size() << " ##" << std::endl;
}

int Model::nverts(){
  return vertices_.size();
}

int Model::nfaces(){
  return faces_.size();
}

Vec3f Model::getVertex(int id){
  return vertices_[id];
}

FaceData Model::getFaceData(int id){
  return faces_[id];
}

/*void Model::print(){
  for(int i = 0; i < nverts() ; i+=10){
    std::cout << getVertex(i) << std::endl;
  }
  std::cout << std::endl << "============" << std::endl;
  for(int i = 0; i< nfaces() ; i+=10){
    std::vector<int> face = getFaceVertices(i);
    std::cout << face[0] << "/" << face[1] << "/" << face[2] << std::endl;
  }
  }*/
