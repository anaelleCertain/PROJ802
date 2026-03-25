%%writefile houghPolaire2.cpp
#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>
#include<algorithm>

// Structure image
template<typename Pixel>
struct ImageGenerique{
  int width;
  int height;
  std::vector<Pixel> image;

  ImageGenerique(int w = 0, int h = 0){
    width = w;
    height = h;
    image = std::vector<Pixel>(w * h);
  }

  Pixel& at(int x, int y){
    return image[y * width + x];
  }

  const Pixel& at(int x, int y) const{
    return image[y * width + x];
  }
};

// Structure pour une image en couleurs
struct PixelRGB{
  int r;
  int g;
  int b;
};

// Structure pour une image binaire
template<typename T>
struct PixelBin{
  T value;
};

// Structure d'une droite en coordonnées polaires
struct Droite{
    float rho;
    float theta;
    int votes;
};

// Lecture ppm
ImageGenerique<PixelRGB> loadPPM(const std::string& filename){

  std::ifstream file(filename);
  std::string format;
  file >> format;

  int w_read;
  int h_read;
  int maxval;
  file >> w_read >> h_read >> maxval;

  ImageGenerique<PixelRGB> img(w_read,h_read);

  for(int i = 0; i< w_read * h_read; i++){
    file >> img.image[i].r >> img.image[i].g >> img.image[i].b;
  }

  return img;
}


// Mise de l'image en niveaux de gris
ImageGenerique<PixelBin<int>> toGray(const ImageGenerique<PixelRGB>& img){

  ImageGenerique<PixelBin<int>> out(img.width,img.height);

  for(int i = 0; i < img.width * img.height; i++){
    int g = (img.image[i].r + img.image[i].g + img.image[i].b)/3;
    out.image[i].value = g;
  }

  return out;
}

// Filtre de Sobel sur l'image d'entrée 
ImageGenerique<PixelBin<int>> sobel(const ImageGenerique<PixelBin<int>>& img){

  int w = img.width;
  int h = img.height;
  ImageGenerique<PixelBin<int>> out(w,h);

  int Gx[3][3] = {
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}
  };

  int Gy[3][3] = {
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}
  };

  for(int y = 1; y < h - 1; y++){
    for(int x = 1; x < w - 1; x++){

      int sx = 0;
      int sy = 0;

      for(int j = -1; j <= 1; j++){
        for(int i = -1; i <= 1; i++){
          int val = img.at(x + i, y + j).value;
          sx += val * Gx[j+1][i+1];
          sy += val * Gy[j+1][i+1];
        }
      }

      int mag = std::sqrt(sx * sx + sy * sy);

      if(mag > 100){
        out.at(x, y).value = 255;
      }
      else{
        out.at(x, y).value = 0;
      }
    }
  }

  return out;
}


// filtre de Sobel sur l'accumulateur 
// pour détecter les fortes variations d'intensité
std::vector<std::vector<int>> sobelAcc(const std::vector<std::vector<int>>& acc){

  // Vérification de la conformité de l'accumulateur
  int h = acc.size();
  if (h == 0) {
    std::cerr << "Accumulateur vide" << std::endl;
    return std::vector<std::vector<int>>();
  }
  
  int w = acc[0].size();
  if (w == 0) {
    std::cerr << "Accumulateur vide" << std::endl;
    return std::vector<std::vector<int>>();
  }

  std::vector<std::vector<int>> out(h, std::vector<int>(w, 0));

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

          int val = acc[y + j][x + i];

          // Application des masques
          sx += val * Gx[j+1][i+1];
          sy += val * Gy[j+1][i+1];
        }
      }

      // Calcul de la norme du gradient - magnitude
      int mag = std::sqrt(sx * sx + sy * sy);
      out[y][x] = mag;
    }
  }

  return out;
}

  // Suppression des non-maxima
  std::vector<std::vector<int>> NMS(const std::vector<std::vector<int>>& acc){

  int h = acc.size();
  int w = acc[0].size();

  std::vector<std::vector<int>> out(h, std::vector<int>(w, 0));

  for(int y = 1; y < h - 1; y++){
    for(int x = 1; x < w - 1; x++){

      int val = acc[y][x];
      bool isMax = true;

      for(int j = -1; j <= 1; j++){
        for(int i = -1; i <= 1; i++){
          if(i==0 && j==0) continue;

          if(acc[y + j][x + i] >= val){
            isMax = false;
          }
        }
      }

      if(isMax){
         out[y][x] = val;
      }
    }
  }
  return out;
}


// Sauvegarde de l'accumulateur au format ppm
void saveAccAsPPM(const std::vector<std::vector<int>>& acc, const std::string& filename){

  int h = acc.size();
  int w = acc[0].size();

  int maxVal = 0;
  for(int r = 0; r < h; r++){
    for(int t = 0; t < w; t++){
      if(acc[r][t] > maxVal){
        maxVal = acc[r][t];
      }
    }
  }

  std::ofstream file(filename);
  file << "P3\n";
  file << w << " " << h << "\n";
  file << "255\n";

  for(int r = 0; r < h; r++){
    for(int t = 0; t < w; t++){

      int val = acc[r][t];

      // normalisation
      int gray = 0;
      if(maxVal > 0){
        gray = (val * 255) / maxVal;
      }
      file << gray << " " << gray << " " << gray << " ";
    }
    file << "\n";
  }
  file.close();
}

// Hough polaire
void houghPolaire(const ImageGenerique<PixelBin<int>>& img,
                  std::vector<std::pair<float,float>> & droites){

  int width = img.width;
  int height = img.height;


  if (width <= 0 || height <= 0) {
    std::cerr << "Dimensions invalides" << std::endl;
    return;
  }

  // Discrétisation
  float rho_max = std::sqrt(width*width + height*height);
  float rho_min = - rho_max;
  float rho_step = 1.0f;
  int nb_rho = (int)((rho_max - rho_min)/rho_step) + 1;

  float theta_min = -M_PI/2;
  float theta_max = M_PI/2;
  float theta_step = M_PI/180;
  int nb_theta = (int)((theta_max - theta_min)/theta_step) + 1;


  if (nb_rho <= 0 || nb_theta <= 0) {
    std::cerr << "Accumulateur vide" << std::endl;
    return;
  }

  std::vector<std::vector<int>> acc(nb_rho, std::vector<int>(nb_theta,0));

  // Remplissage de l'accumulateur
  for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){

      if(img.at(y,x).value > 0){

        for(int t = 0; t < nb_theta; t++){

          // Transformée inverse
          float theta = theta_min + t * theta_step;

          // Equation de la transformée de Hough
          float rho = x * cos(theta) + y * sin(theta);

          int r = (int)((rho - rho_min)/rho_step);

          if(r >= 0 && r < nb_rho){
            acc[r][t]++;
          }
        }
      }
    }
  }

int maxVal = 0;

for(int r = 0; r < nb_rho; r++){
  for(int t = 0; t < nb_theta; t++){
    if(acc[r][t] > maxVal){
      maxVal = acc[r][t];
    }
  }
}

std::cout << "MAX ACC = " << maxVal << std::endl;

saveAccAsPPM(acc,"acc.ppm");

auto acc_sobel = sobelAcc(acc);
saveAccAsPPM(acc_sobel,"acc_sobel.ppm");

auto acc_nms = NMS(acc_sobel);
saveAccAsPPM(acc_nms,"acc_nms.ppm");

float seuil = 0.4f * maxVal;

std::vector<Droite> droites_Hough;
for(int r = 0; r < nb_rho; r++){
  for(int t = 0; t < nb_theta; t++){
    if(acc_nms[r][t] > seuil){

      Droite d;
      d.rho = rho_min + r * rho_step;
      d.theta = theta_min + t * theta_step ;
      d.votes = acc_nms[r][t];

      droites_Hough.push_back(d);
    }
  }
}

// Tri
std::sort(droites_Hough.begin(), droites_Hough.end(),
  [](auto&a,auto&b){return a.votes>b.votes;});

std::cout << "Droites detectees :" << std::endl;

for(int i = 0; i < std::min(5,(int)droites_Hough.size()); i++){
  std::cout << "rho=" << droites_Hough[i].rho
            << " theta=" << droites_Hough[i].theta
            << " votes=" << droites_Hough[i].votes << std::endl;
  }
}

// Fonction pour tracer les droites détectées sur l'image
void tracerDroite(ImageGenerique<PixelRGB>& img, float rho, float theta){

  int w = img.width;
  int h = img.height;

  float cos_t = std::cos(theta);
  float sin_t = std::sin(theta);

  for(int x = 0; x < w; x++){
    if(std::abs(sin_t) > 1e-6){
      int y = (int)((-(cos_t/sin_t) * x) + rho/sin_t);

      if(y >= 0 && y < h){
        img.at(x,y).r = 255;
        img.at(x,y).g = 0;
        img.at(x,y).b = 0;
      }
    }
  }
}

// Fonction pour sauvegarder l'image en couleurs
void savePPM(const ImageGenerique<PixelRGB>& img, const std::string& filename){

  std::ofstream file(filename);

  file << "P3\n";
  file << img.width << " " << img.height << "\n";
  file << "255\n";

  for(int i = 0; i < img.width * img.height; i++){
    file << img.image[i].r << " "
         << img.image[i].g << " "
         << img.image[i].b << " ";
  }
}

int main(){

  auto img = loadPPM("/content/imageM1_bin.ppm");
  
  // Transformation de l'image
  auto gray = toGray(img);
  auto edges = sobel(gray);

  std::vector<std::pair<float,float>> droites;

  houghPolaire(edges, droites);

  // copie image originale
  ImageGenerique<PixelRGB> img_out = img;

  // tracer les 5 meilleures
  for(int i = 0; i < std::min(5, (int)droites.size()); i++){
    tracerDroite(img_out, droites[i].first, droites[i].second);
  }

  savePPM(img_out, "resultat.ppm");

  return 0;
}
