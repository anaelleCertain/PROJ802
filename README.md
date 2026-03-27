# **Détection de droites à l'aide de Ransac et de la transformée de Hough**

Le but de ce projet est de mettre en oeuvre la méthode de la transformée de Hough afin de détecter des droites dans une image. Les différentes étapes sont les suivantes : pré-traitement de l'image pour la binariser, la lire au format ppm et sauvegarder l'image binaire. Puis implémentation de la première méthode dans l'esapce de Hough où les objets sont des droites données par leur équation $y = mx + b$. Ensuite on cherche une méthode plus robuste et moins limitée de la méthode de Hough où on passe les coordonnées en polaire où $\rho = xcos(\theta) + ysin(\theta)$.


### **Contenu**

**Fichiers** : 

- image.cpp : Contient l'ensemble des structures de pixels, les fonctions pour le traîtement des images et les fonctions pour la lecture et sauvegarde des images

- affichageImage.py : Code python pour l'affichage de l'image d'origine et de l'image binarisée avec la bibliothèque matplotlib.

- sobel.cpp : Code du filtre de Sobel qui est utilisé plus tard sur l'image d'entrée et l'accumulateur. Le code est testé sur une image test.

- affichageSobel.py : Code python pour l'affichage de l'image filtrée par Sobel avec la bibliothèque matplotlib.

- houghNaif.cpp : Implémentation de l'algorithme de la méthtode de Hough sous la forme affine $y = mx + b$. Application sur une image test. Affichage pour la détection d'une droite étape par étape puis détection de toutes les droites. Tracé des droites et sauvegarde de l'image avec les droites.

- affichageHoughNaif.py : Code python pour l'affichage de l'image créée avec tracé en rouge les droites détectées avec la bibliothèque matplotlib.

- houghPolaire.cpp : Traîtement de l'image avec un filtre de Sobel pour la détection des contours, implémentation de la méthode de Hough en polaire puis traîtement de l'accumulateur pour détecter les pics : application du filtre de Sobel et la suppression des non-maxima. Fonctions pour la visualisation du résultat.

- affichageHoughPolaire.py : Affichage de l'accumulateur et de l'image binaire avec les droites détectées tracées dessus.

- RANSAC.cpp : Code basique (pas optimisé au niveau des seuils et des sélections des points p1 et p2). Avec un main() qui peut être utilisé dans le main() de houghPolaire.

- Deux images pour la compilation : imageM1_bin.ppm et imageM1.ppm et un résultat (filtre de Sobel) sobel.ppm.

      
**Liste des structures utilisées**
  - Image générique
  - PixelRGB
  - PixelGris
  - PixelBin
  - Droite
  - Point 


### **Guide utilisation**

Chaque fichier peut être exécuté indépendemment des autres car toutes les structures nécessaires à la compilation sont importées au début du code.

Le code sobel.cpp peut aussi être appliqué à Hough naïf.

Dans le main de RANSAC.cpp on trouve le code à coller dans le main() de houghPolaire.

Comme les codes ont été fait sur Google Collab, pour compiler faire :

%%bash
g++ nomFichier.cpp -o nomFichier_app

%%bash
./nomFichier_app


Commandes pour clôner le projet dans colab : 

!git clone https://github.com/anaelleCertain/PROJ802.git

%cd PROJ802

!g++ main.cpp -o programme

!./programme













