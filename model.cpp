#include "model.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Model::Model(const char *filename) : vertices_(), faces_() {
  std::ifstream in;
  std::string line;
  std::cout << "loading" << std::endl;
  in.open(filename, std::ifstream::in);
  if(in.fail()) return;
  std::cout << "loaded" << std::endl;
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
    else if(!line.compare(0,2,"f ")){
      std::vector<int> f;
      int itrash, id;
      iss >> trash;
      while(iss >> id >> trash >> itrash >> trash >> itrash){
	id--;
	f.push_back(id);
      }
      faces_.push_back(f);
    }
  }
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

std::vector<int> Model::getFaceVertexes(int id){
  return faces_[id];
}

void Model::print(){
  for(int i = 0; i < nverts() ; i+=10){
    std::cout << getVertex(i) << std::endl;
  }
  std::cout << std::endl << "============" << std::endl;
  for(int i = 0; i< nfaces() ; i+=10){
    std::vector<int> face = getFaceVertexes(i);
    std::cout << face[0] << "/" << face[1] << "/" << face[2] << std::endl;
  }
}
