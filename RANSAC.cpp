%%writefile ransacBasqiue.cpp
#include <iostream>
#include <vector>
#include <cmath>


// Structure pour créer l'ensemble des points 
// suffisamment proche de la droite 
struct Point {
    float x;
    float y;
};

// Distance point droite
float distancePointDroite(const Point& p, const Droite& d){

  float cos_t = std::cos(d.theta);
  float sin_t = std::sin(d.theta);

  float dist = std::abs(p.x * cos_t + p.y * sin_t - d.rho);

  return dist;
}

// On crée une droite à partir de P1 et P2
Droite droiteDepuisPoints(const Point& p1, const Point& p2){

  Droite d;

  float dx = p2.x - p1.x;
  float dy = p2.y - p1.y;

  float theta = std::atan2(dy, dx) + M_PI/2;

  d.rho = rho;
  d.theta = theta;
  d.votes = 0;

  return d;
}

// Récupération des points de l'image
std::vector<Point> extrairePoints(const ImageGenerique<PixelBin<int>>& img){

  std::vector<Point> points;

  for(int y = 0; y < img.height; y++){
    for(int x = 0; x < img.width; x++){

      if(img.at(x,y).value > 0){
        Point p;
        p.x = x;
        p.y = y;
        points.push_back(p);
      }
    }
  }

  return points;
}

// Ensemble des points proche d'une droite de Hough
std::vector<Point> pointsProchesDroite(
    const std::vector<Point>& points,
    const Droite& d,
    float epsilon){

  std::vector<Point> S;

  for(const auto& p : points){

    float dist = distancePointDroite(p,d);

    if(dist < epsilon){
      S.push_back(p);
    }
  }

  return S;
}

// Algorithme de RANSAC

Droite RANSAC(const std::vector<Point>& S,
              int K,
              float t){

  Droite meilleur_modele;
  int meilleur_score = 0;

  int n = S.size();

  if(n < 2){
    return meilleur_modele;
  }

  for(int i = 0; i < K; i++){

    int id1 = rand() % n;
    int id2 = rand() % n;

    if(id1 == id2) continue;

    Point P1 = S[id1];
    Point P2 = S[id2];

    Droite Mi = droiteDepuisPoints(P1,P2);

    int N = 0;

    for(const auto& p : S){

      float dist = distancePointDroite(p,Mi);

      if(dist < t){
        N++;
      }
    }

    if(N > meilleur_score){

      meilleur_score = N;
      meilleur_modele = Mi;
      meilleur_modele.votes = N;

    }
  }

  return meilleur_modele;
}

// Pour l'utilisation après le code houghPolaire
int main(){
  auto points = extrairePoints(img);

  float epsilon = 3.0f; // to be adjust
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
}
