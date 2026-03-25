%%writefile image2.cpp
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

// Pixel couleur
template<typename T>
struct PixelRGB{
  T r;
  T g;
  T b;
};

// Pixel gris
template<typename T>
struct PixelGris{
  T val;
};

// Pixel binaire
template<typename T>
struct PixelBin{
  T value;
};

// Image
template<typename Pixel>
struct ImageGenerique {
    int width;
    int height;
    std::vector<Pixel> data;

    // Initialisation d'une nouvelle image
    ImageGenerique() {
        width = 0;
        height = 0;
    }

    ImageGenerique(int w, int h) {
        width = w;
        height = h;
        data = std::vector<Pixel>(w * h);
    }

    // Pour l'accès à un pixel
    Pixel& at(int x, int y) {
        int index = y * width + x;
        if (index < 0 || index >= data.size()) {
            std::cerr << "Error: Image::at() index out of bounds! (" << x << "," << y << "), calculated index: " << index << ", data size: " << data.size() << std::endl; std::cerr.flush();

        }
        return data[index];
    }
};

// Conversion de l'image en niveau de gris
template<typename PC, typename PG>
void convertirNiveauGris(const ImageGenerique<PC> &imgCouleur,
  ImageGenerique<PG> &imgGris){
    imgGris.width = imgCouleur.width;
    imgGris.height = imgCouleur.height;
    imgGris.data = std::vector<PG>(imgGris.width * imgGris.height);

    for(int i = 0; i<imgCouleur.width * imgCouleur.height; i++){
      int r = imgCouleur.data[i].r;
      int g = imgCouleur.data[i].g;
      int b = imgCouleur.data[i].b;

      imgGris.data[i].val = (r + g + b)/3;
    }
  }

// Seuillage (image en niveau de gris -> image binaire)
template<typename PixelG, typename PixelB>
void seuillage(const ImageGenerique<PixelG>& imgG, ImageGenerique<PixelB>& imgB, int seuil){
  imgB.width = imgG.width;
  imgB.height = imgG.height;
  imgB.data = std::vector<PixelB>(imgB.width * imgB.height);

  for(int i = 0; i<imgG.width * imgG.height; i++){
    if(imgG.data[i].val > seuil){
      imgB.data[i].value = 1;
    }
    else{
      imgB.data[i].value = 0;
    }
  }
}

// Problème de lecture d'image avec l'environnement reglé par gemini
// Helper to skip comments and whitespace
void skipComments(std::ifstream& file) {
    char c;
    while (file >> c) {
        if (c == '#') {
            std::string line;
            std::getline(file, line);
        } else {
            file.unget();
            break;
        }
    }
}

// Lecture PPM
template<typename Pixel>
void loadPPM(ImageGenerique<Pixel>& img, const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl; std::cerr.flush();
        return;
    }

    std::string magic;
    file >> magic;
    std::cerr << "[DEBUG] PPM Magic: " << magic << std::endl; std::cerr.flush();

    // Check if it's a P3 PPM file
    if (magic != "P3") {
        std::cerr << "Error: Expected P3 PPM format, but got " << magic << " in file " << filename << std::endl; std::cerr.flush();
        img.width = 0; img.height = 0;
        return;
    }

    skipComments(file);
    int w, h;
    file >> w >> h;
    std::cerr << "[DEBUG] PPM Dimensions read: " << w << "x" << h << std::endl; std::cerr.flush();

    // Validate dimensions after reading
    if (file.fail() || w <= 0 || h <= 0) {
        std::cerr << "Error: Invalid or missing dimensions in PPM file " << filename << ". Width: " << w << ", Height: " << h << std::endl; std::cerr.flush();
        img.width = 0; img.height = 0;
        return;
    }

    skipComments(file);
    int maxval;
    file >> maxval;
    std::cerr << "[DEBUG] PPM Maxval: " << maxval << std::endl; std::cerr.flush();


    skipComments(file);

    img.width = w;
    img.height = h;
    img.data = std::vector<Pixel>(w * h);

    for (int i = 0; i < w * h; i++) {
        int r_val, g_val, b_val;
        file >> r_val >> g_val >> b_val;

        if (file.fail()) {
            std::cerr << "Error: Failed to read pixel data for pixel " << i << " from PPM file." << std::endl; std::cerr.flush();
            img.width = 0; img.height = 0; img.data.clear();
            return;
        }

        img.data[i].r = r_val;
        img.data[i].g = g_val;
        img.data[i].b = b_val;
    }
}

// Sauvegarde PPM pour image en niveaux de gris (P2 format)
template<typename PG>
void savePPM(const ImageGenerique<PG>& imgGris, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing " << filename << std::endl; std::cerr.flush();
        return;
    }

    file << "P2\n";
    file << imgGris.width << " " << imgGris.height << "\n";
    file << "255\n";

    for (int i = 0; i < imgGris.width * imgGris.height; ++i) {
        file << imgGris.data[i].val << " ";
        if ((i + 1) % imgGris.width == 0) {
            file << "\n";
        }
    }
    file.close();
}

// Sauvegarde PPM pour image binaire
template<typename PB>
void saveBinaryPPM(const ImageGenerique<PB>& imgBin, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing " << filename << std::endl; std::cerr.flush();
        return;
    }

    file << "P2\n";
    file << imgBin.width << " " << imgBin.height << "\n";
    file << "255\n";

    for (int i = 0; i < imgBin.width * imgBin.height; ++i) {
        file << (imgBin.data[i].value * 255) << " ";
        if ((i + 1) % imgBin.width == 0) {
            file << "\n";
        }
    }
    file.close();
}

int main() {

    using PixelC = PixelRGB<int>;
    using PixelG = PixelGris<int>;
    using PixelB = PixelBin<int>;

    ImageGenerique<PixelC> imgC;
    loadPPM(imgC, "/content/imageM1.ppm");


    ImageGenerique<PixelG> imgG;
    convertirNiveauGris(imgC, imgG);


    ImageGenerique<PixelB> imgB;
    seuillage(imgG, imgB, 200);


    std::cout << imgB.at(0,0).value << "\n";

    // Save the binary image
    saveBinaryPPM(imgB, "/content/imageM1_bin.ppm");


    return 0;
}
