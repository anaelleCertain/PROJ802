%%writefile triDroites.cpp
#include<iostream>
#include<array>
#include<vector>
#include<cmath>
#include<algorithm>
#include <fstream>


// image générique
template<typename Pixel>
struct ImageGenerique{
  int width;
  int height;
  std::vector<Pixel> image;

  ImageGenerique(int w, int h){
    width = w;
    height = h;
    image = std::vector<Pixel>(w * h); 
  }

  //accés pixel
  Pixel& at(int x, int y) {
    return image[y * width + x];
  }

  // idem pour lecture seule
  const Pixel& at(int x, int y) const {
    return image[y * width + x];
  }
};

// image binaire
template<typename T>
struct PixelBin {
    T value;
};

// Image en couleurs
struct PixelRGB{
  int r;
  int g;
  int b;
};

// Structure droite pour le stockage des droites trouvées
struct Droite{
    float m;
    float b;
    int croisements;
};

// Fonction pour générer une image test dans laquelle on a mis des droites
void genererImageTest(ImageGenerique<PixelBin<int>>& img){
    for(int i = 0; i < img.width * img.height; i++){
        img.image[i].value = 0; //initialise image noire
    }

    // Génère 3 droites sur l'image
    for(int x = 0; x < img.width; x++){
        int y1 = x + 10;
        int y2 = (int)(-0.5 * x + 80);
        int y3 = (int)(0.3 * x + 20);

        // Active les points à détecter 
        if(y1 >= 0 && y1 < img.height) img.at(x,y1).value = 1;
        if(y2 >= 0 && y2 < img.height) img.at(x,y2).value = 1;
        if(y3 >= 0 && y3 < img.height) img.at(x,y3).value = 1;
    }
}

// Transformée de Hough naïf
std::vector<std::vector<int>> houghNaif(const ImageGenerique<PixelBin<int>>& img){

  int width = img.width;
  int height = img.height;

  // pente ajuster bounds
  float m_min = -2.0f;
  float m_max = 2.0f;
  float m_step = 0.05f;
  int nb_m = (m_max - m_min)/m_step;

  // ordonnée à l'origine
  int b_min = - height;
  int b_max =  height;
  float b_step = 1.0f;
  int nb_b = (int)((b_max - b_min) / b_step); // hauteur de l'image

  // Accumulateur
  std::vector<std::vector<int>> acc(nb_b, std::vector<int>(nb_m, 0));

  // remplissage de l'accumulateur
  for(int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){

      if(img.at(x,y).value == 1){

        for (int m_idx = 0; m_idx < nb_m; m_idx++){

          float m = m_min + m_idx * m_step;
          float b = y - m * x;

          int b_idx = (int)((b - b_min)/b_step);

          if(b_idx >= 0 && b_idx < nb_b){
            acc[b_idx][m_idx]++;
            
          }
        }
      }
    }
  }

  int maxVal = 0;
  for(int b = 0; b < nb_b; b++){
    for(int m = 0; m < nb_m; m++){
      if(acc[b][m] > maxVal){
        maxVal = acc[b][m];
      }
    }
  }

  // trouver max de croisements pour une seule droite à détecter
  int maxCroisements = 0;
  int best_m_idx = 0;
  int best_b_idx = 0;

  for(int b_idx = 0; b_idx < nb_b; b_idx++){
    for(int m_idx = 0; m_idx < nb_m; m_idx++){

      if(acc[b_idx][m_idx] > maxCroisements){
        maxCroisements = acc[b_idx][m_idx];
        best_m_idx = m_idx;
        best_b_idx = b_idx;

        std::cout << "Nouveau max trouvé : "
          << "m_idx=" << best_m_idx
          << " b_idx=" << best_b_idx
          << " valeur=" << maxCroisements << std::endl;
      }
    }
  }

  float best_m = m_min + best_m_idx * m_step;
  float best_b = b_min + best_b_idx;
  std::cout << "Droite actuelle : y = "<< best_m << "x + " << best_b << std::endl;

  std::cout << "Droite detectee : y = "<< best_m << "x + " << best_b << std::endl;
  std::cout << "Nombre de croisements : " << maxCroisements << std::endl;

  return acc;
}

// Non-maximum suppression : garde uniquement les maxima locaux
std::vector<std::vector<int>> nonMaximumSuppression(const std::vector<std::vector<int>>& acc){
  int height = acc.size();
  int width =  acc[0].size();

  std::vector<std::vector<int>> result(height, std::vector<int>(width, 0));

  int voisin = 1;

  for(int b = voisin; b < height - voisin; b++){
    for(int m = voisin; m < width - voisin; m++){

      int current = acc[b][m];
      bool isMax = true;

      for(int db = -voisin; db <= voisin; db++){
        for(int dm = -voisin; dm <= voisin; dm++){

          if(db == 0 && dm == 0){
            continue;
          }

          int check_b = b + db;
          int check_m = m + dm;

          if (check_b >= 0 && check_b < height && check_m >= 0 && check_m < width) {
              if(acc[check_b][check_m] > current){
                isMax = false;
                break; // No need to check further neighbors for this (b, m)
              }
          }
        }
        if (!isMax) break;
      }

      if(isMax){
          result[b][m] = acc[b][m];
      }
    }
  }
  return result;
}


// Detection de droites
std::vector<Droite> detecterDroites(const std::vector<std::vector<int>>& acc,
                                float m_min, float m_step,
                                float b_min, float b_step,
                                int K){
  int height = acc.size();
  int width = acc[0].size();

  std::vector<Droite> droites_Hough;

  double somme = 0;
  int taille_acc = height * width;

  for(int b = 0; b < height; b++){
    for(int m = 0; m < width; m++){
      somme += acc[b][m];
    }
  }

  double moyenne = somme / taille_acc;
  double seuil_importance = moyenne + 2 * std::sqrt(moyenne);

  // Garder uniquement les droites improtantes
  for(int b = 0; b < height; b++){
    for(int m = 0; m < width; m++){
      if(acc[b][m] >= seuil_importance){

        Droite d;
        d.m = m_min + m * m_step;
        d.b = b_min + b * b_step;
        d.croisements = acc[b][m];

        droites_Hough.push_back(d);
      }
    }
  }

  // Tri
  std::sort(droites_Hough.begin(), droites_Hough.end(), [](const Droite& a, const Droite& b) {
      return a.croisements > b.croisements;
  });

  // Garder les K qui comptent le plus d'intersection
  int num_to_return = std::min((int)droites_Hough.size(), K);
  std::vector<Droite> top_K_droites(droites_Hough.begin(), droites_Hough.begin() + num_to_return);

  return top_K_droites;
}


// Tracer une droite
void tracerDroite(ImageGenerique<PixelRGB>& img, float m, float b){

    for(int x = 0; x < img.width; x++){
        int y = (int)(m * x + b); // Repasser à une équation de droite

        if(y >= 0 && y < img.height){
            img.at(x, y) = {255, 0, 0}; // Mettre ne rouge les droites détéctées
        }
    }
}

// Tracer plusieurs droites
void tracerDroites(ImageGenerique<PixelRGB>& img,
                   const std::vector<Droite>& droites){

    for(const auto& d : droites){
        tracerDroite(img, d.m, d.b);
    }
}
// Sauvegarde
void savePPM(const ImageGenerique<PixelRGB>& img, const std::string& filename){

    std::ofstream file(filename);

    file << "P3\n";
    file << img.width << " " << img.height << "\n";
    file << "255\n";

    for(int y = 0; y < img.height; y++){
        for(int x = 0; x < img.width; x++){
            const auto& px = img.at(x, y);
            file << px.r << " " << px.g << " " << px.b << " ";
        }
        file << "\n";
    }

    file.close();
}

int main(){

    int width = 100;
    int height = 100;

    ImageGenerique<PixelBin<int>> img(width, height);

    genererImageTest(img);

    float m_min = -2.0f;
    float m_max = 2.0f;
    float m_step = 0.05f;
    int nb_m = (int)((m_max - m_min) / m_step);

    float b_min = -height;
    float b_max = height;
    float b_step = 1.0f;
    int nb_b = (int)((b_max - b_min) / b_step);

    std::vector<std::vector<int>> acc = houghNaif(img);

    auto acc_nms = nonMaximumSuppression(acc);

    auto droites = detecterDroites(acc_nms, m_min, m_step, b_min, b_step, 3);

    for(const auto& d : droites){
        std::cout << "y = " << d.m << "x + " << d.b
                  << " votes = " << d.croisements << std::endl;
    }

    // image RGB pour affichage
    ImageGenerique<PixelRGB> img_rgb(width, height);

    // copie en niveaux de gris
    for(int y = 0; y < height; y++){
      for(int x = 0; x < width; x++){
        int val = img.at(x,y).value ? 255 : 0;
        img_rgb.at(x,y) = {val, val, val};
    }
  }

// tracer les droites en rouge
tracerDroites(img_rgb, droites);

// sauvegarde
savePPM(img_rgb, "resultat.ppm");

    return 0;
}

