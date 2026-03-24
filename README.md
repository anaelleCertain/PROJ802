# **Détection de droites à l'aide de Ransac et de la transformée de Hough**


**Explication de la structure des fichiers et des fonctions qu'ils cotniennent**

**4 fichiers** : 

- image
- Hough naïf
- Hough polaire
- RANSAC

Liste des onctions par fichier et leur utilité : 

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

      
**Liste des structures utilisées**
  - Image générique
  - PixelRGB
  - PixelGris
  - PixelBin
  - Droite

**Guide utilisation**

Chaque fichier peut-être exécuté indépendemment des autres car toutes les structures nécessaires à la compilation sont importées au début du code.


**Tests**



