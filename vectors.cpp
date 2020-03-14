#include "vectors.h"

Matrix::Matrix(int w, int h) : width_(w), height_(h),
			       m_(std::vector<std::vector<float>>(h,std::vector<float>(w,0.f))) {}

Matrix Matrix::identity(int DIM){
  Matrix M(DIM,DIM);
  for(int i = 0 ; i < DIM ; i++){
    for(int j = 0 ; j < DIM ; j++){
      M[i][j]=(i==j?1.f:0.f);
    }
  }
  return M;
}

Matrix Matrix::transpose(){
  Matrix m(get_height(),get_width());
  for(int i = 0; i < get_width(); i++){
    for(int j = 0; j < get_height(); j++){
      m[j][i]=m_[i][j];
    }
  }
  return m;
}

Matrix Matrix::inverse(){
  assert(get_height()==get_width());
  //Augment matrix with identity matrix
  Matrix d(get_width()*2,get_height());
  for(int y = 0; y < get_height(); y++){
    for(int x = 0 ; x < get_width(); x++){
      d[y][x] = m_[y][x];
    }
    d[y][y+get_width()] = 1;
  }
  //First pass
  for(int y = 0; y < get_height()-1 ; y++){
    for(int x = d.get_width()-1; x > 0 ; x--){
      d[y][x]/=d[y][y];
    }
    for(int x = y+1; x < get_width(); x++){
      float c = d[x][y];
      for(int j = 0 ; j < d.get_width() ; j++){
	d[x][j] -= d[y][j]*c;
      }
    }
  }
  //Normalize last row
  for(int x = d.get_width()-1 ; x >= get_height()-1; x--){
    d[get_height()-1][x] /= d[get_height()-1][get_height()-1];
  }
  //Second pass
  for(int y = get_height()-1 ; y > 0 ; y--){
    for(int x = y-1 ; x >= 0 ; x--){
      float c = d[x][y];
      for(int j = 0 ; j < d.get_width() ; j++){
	d[x][j] -= d[y][j]*c;
      }
    }
  }
  Matrix m(get_width(),get_height());
  for(int y = 0 ; y < get_height() ; y++){
    for(int x = 0 ; x < get_width() ; x++){
      m[y][x] = d[y][x+get_width()];
    }
  }
  return m;
}

std::vector<float>& Matrix::operator[](const int i){
  assert(i>=0 && i<get_height());
  return m_[i];
}

Matrix Matrix::operator*(const Matrix& a){
  assert(width_ == a.height_);
  Matrix result(a.width_,height_);
  for(int i = 0 ; i < height_ ; i++){
    for(int j = 0 ; j < a.width_ ; j++){
      result.m_[i][j] = 0.f;
      for(int k = 0 ; k < width_ ; k++){
	result.m_[i][j] += m_[i][k]*a.m_[k][j];
      }
    }
  }
  return result;
}

int Matrix::get_width() const{
  return width_;
}

int Matrix::get_height() const{
  return height_;
}

std::ostream& operator<<(std::ostream& s, const Matrix &m){
  for(int y = 0 ; y < m.get_height() ; y++){
    for(int x = 0 ; x < m.get_width() ; x++){
      s << m.m_[y][x] << "\t";
    }
    s << std::endl;
  }
  return s;
}
