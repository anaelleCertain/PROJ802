#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include<cmath>


int main(){
    int w = 100;
    int h = 100;

    // Image test
    auto img = creerImageTest(w,h);

    // Détection des contours de l'image
    auto edges = sobel(img);

    // Sauvegarde
    savePPM(edges, "sobel.ppm");

    using PixelC = PixelRGB<int>;
    using PixelG = PixelGris<int>;
    using PixelB = PixelBin<int>;

    // Lecture de l'image imageM1.ppm
    ImageGenerique<PixelC> imgC;
    loadPPM(imgC, "/content/imageM1.ppm");

    // Conversion en niveau de gris
    ImageGenerique<PixelG> imgG;
    convertirNiveauGris(imgC, imgG);

    // Conversion en image binaire et sauvegarde de la nouvelle image binaire
    ImageGenerique<PixelB> imgB;
    seuillage(imgG, imgB, 200);
    saveBinaryPPM(imgB, "/content/imageM1_bin.ppm");

    // création de l'image test pour Hough naïf
    ImageGenerique<PixelBin<int>> img(width, height);

    genererImageTest(img);

    // Paramètres pour Hough na ïf
    float m_min = -2.0f;
    float m_max = 2.0f;
    float m_step = 0.05f;
    int nb_m = (int)((m_max - m_min) / m_step);

    float b_min = -height;
    float b_max = height;
    float b_step = 1.0f;
    int nb_b = (int)((b_max - b_min) / b_step);

    // Accumulateur pour Hough naïf
    std::vector<std::vector<int>> acc = houghNaif(img);

    // Détection des pics dans l'accumulateur
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

    // Image M1 avec les droites détectées
    savePPM(img_out, "resultat.ppm");

    auto points = extrairePoints(img);

    // Paramètres pour RANSAC
    float epsilon = 3.0f; 
    int K = 500;
    float t = 2.0f;

    for(const auto& d_hough : droites_Hough){

        auto S = pointsProchesDroite(points,d_hough,eps);
        Droite d_ransac = RANSAC(S,K,t);

        std::cout << "RANSAC : rho=" << d_ransac.rho
            << " theta=" << d_ransac.theta
            << " votes=" << d_ransac.votes
            << std::endl;
  }
    
    return 0;
}
