#include<iostream>

int main(){
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

    saveBinaryPPM(imgB, "/content/imageM1_bin.ppm");

    std::vector<std::vector<int>> acc = houghNaif(img);

    auto acc_nms = nonMaximumSuppression(acc);

    auto droites = detecterDroites(acc_nms, m_min, m_step, b_min, b_step, 3);

    tracerDroites(img_rgb, droites);

    savePPM(img_rgb, "resultat.ppm");

    auto img = loadPPM("/content/imageM1_bin.ppm");

    auto edges = sobel(gray);

    std::vector<std::pair<float,float>> droites;

    houghPolaire(edges, droites);

    savePPM(img_out, "resultat.ppm");

    auto points = extrairePoints(img);

    float epsilon = 3.0f; 
    int K = 500;
    float t = 2.0f;

    auto S = pointsProchesDroite(points,d_hough,eps);

    Droite d_ransac = RANSAC(S,K,t);


    return 0;
}
