# **Détection de droites à l'aide de Ransac et de la transformée de Hough**


### **Explication de la structure des fichiers et des fonctions qu'ils cotniennent**

**Fichiers** : 

- image.cpp : Contient l'ensemble des structures de pixels, les fonctions pour le traîtement des images et les fonctions pour la lecture et sauvegarde des images

- affichageImage.py : Code python pour l'affichage de l'image d'origine et de l'image binarisée avec la bibliothèque matplotlib.

- sobel.cpp : Code du filtre de Sobel qui est utilisé plus tard sur l'image d'entrée et l'accumulateur. Le code est testé sur une image test.

- affichageSobel.py : Code python pour l'affichage de l'image filtrée par Sobel avec la bibliothèque matplotlib.

- houghNaif.cpp : Implémentation de l'algorithme de la méthtode de Hough sous la forme affine $y = mx + b$. Application sur une image test. Affichage pour la détection d'une droite étape par étape puis détection de toutes les droites. Tracé des droites et sauvegarde de l'image avec les droites.

- affichageHoughNaif.py : Code python pour l'affichage de l'image créée avec tracé en rouge les droites détectées avec la bibliothèque matplotlib.

- houghPolaire.cpp : Traîtement de l'image avec un filtre de Sobel pour la détection des contours, implémentation de la méthode de Hough en polaire puis traîtement de l'accumulateur pour détecter les pics : application du filtre de Sobel et la suppression des non-maxima. Fonctions pour la visualisation du résultat.

- affichageHoughPolaire.py : Affichage de l'accumulateur et de l'image binaire avec les droites détectées tracées dessus.

- RANSAC

Liste des fonctions par fichier et leur utilité : 

- image :

  **Pour le traîtement de l'image, l'import et la sauvegarde**
    - convertirNiveauGris
    - seuillage
    - loadPPM
    - savePPM
    - saveBinaryPPM
 
   **Pour l'affichage**
    - appel à la bibliothèque matplotlib de python
 
   **Pour le test du filtre de Sobel**
    - sobel
 
- Hough naïf :

  **Pour la détection d'une seule droite avec une image test**
    - creerImage
    - hougNaif

   **Pour la détection et le tracer de plusieurs droites**
    - genererImageTest
    - houghNaif
    - nonMaximumSuppression
    - detecterDroites
    - fonction de trie
    - topKdroites
    - tracerDroite
    - tracerDroites
    - savePPM
 
  - Hough polaire :
      - loadPPM
      - toGray
      - sobel
      - sobelAcc
      - NMS
      - moyenne
      - saveAccAsPPM
      - houghPolaire
      - fonction de tri
      
   **Pour l'affichage**
    - appel à la bibliothèque matplotlib de python
    - pour l'affichage de l'image avec les droites détectées : modifier les valeurs de $\rho$ et $\theta$ dans le code avec les nouvelles coordonnées trouvées

      
**Liste des structures utilisées**
  - Image générique
  - PixelRGB
  - PixelGris
  - PixelBin
  - Droite 

### **Guide utilisation**

Chaque fichier peut-être exécuté indépendemment des autres car toutes les structures nécessaires à la compilation sont importées au début du code.

Comme les codes ont été fait sur Google Collab, pour compiler faire :

%%bash

g++ nomFichier.cpp -o nomFichier_app

%%bash

./nomFichier_app


### **Tests**



