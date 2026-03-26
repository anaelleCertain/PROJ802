%%writefile testSobel.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

// Structure pour une image générique
template<typename Pixel>
struct ImageGenerique{
  int width, height;
  std::vector<Pixel> image;

  ImageGenerique(int w, int h){
    width = w;
    height = h;
    image = std::vector<Pixel>(w * h);
  }

  Pixel& at(int x, int y){
    return image[y * width + x];
  }
};

template<typename T>
struct PixelBin{
  T value;
};

// Image Test : fonc noir avec une diagonale blanche
ImageGenerique<PixelBin<int>> creerImageTest(int w, int h){

  ImageGenerique<PixelBin<int>> img(w,h);

  // fond noir
  for(int i = 0; i < w * h; i++){
    img.image[i].value = 0;
  }

  // diagonale blanche
  for(int x = 0; x < w; x++){
    int y = x;
    if(y < h){
      img.at(x,y).value = 255;
    }
  }

  return img;
}


// filtre de Sobel
ImageGenerique<PixelBin<int>> sobel(const ImageGenerique<PixelBin<int>>& img){

  int w = img.width;
  int h = img.height;

  ImageGenerique<PixelBin<int>> img_filtree(w,h);

  // Masque de Sobel horizontal
  int Gx[3][3] = {
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}
  };

  // Masque de Sobel vertical
  int Gy[3][3] = {
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}
  };

  // Convolution autour du pixel (x,y)
  for(int y = 1; y < h - 1; y++){
    for(int x = 1; x < w - 1; x++){

      int sx = 0;
      int sy = 0;

      for(int j = -1; j <= 1; j++){
        for(int i = -1; i <= 1; i++){

          int val = img.image[(y + j) * w + (x + i)].value;

          // Application des masques
          sx += val * Gx[j+1][i+1];
          sy += val * Gy[j+1][i+1];
        }
      }

      // Calcul de la norme du gradient
      int grad_norme = std::sqrt(sx * sx + sy * sy);

      // Seuillage : contour fort -> blanc
      // sinon -> noir
      if(grad_norme > 100){
        img_filtree.at(x, y).value = 255;
      }
      else{
        img_filtree.at(x, y).value = 0;
      }
    }
  }

  return img_filtree;
}


// Sauvegarde PPM
void savePPM(const ImageGenerique<PixelBin<int>>& img, const std::string& filename){

  std::ofstream file(filename);

  file << "P3\n";
  file << img.width << " " << img.height << "\n";
  file << "255\n";

  for(int i=0;i<img.width*img.height;i++){

    int val = img.image[i].value;

    // gris → RGB
    file << val << " " << val << " " << val << " ";

    if((i+1)%img.width==0){
      file << "\n";
    }
  }

  file.close();
}


int main(){

  int w = 100, h = 100;

  auto img = creerImageTest(w,h);

  auto edges = sobel(img);

  savePPM(edges, "sobel.ppm");

  std::cout << "Image Sobel sauvegardee : sobel.ppm" << std::endl;

  return 0;
}


