from PIL import Image
import matplotlib.pyplot as plt

# Affichage de l'image originale
img_color = Image.open("/content/imageM1.ppm")
plt.figure(figsize=(5,5))
plt.imshow(img_color)
plt.title("Original Color Image")
plt.axis("off")
plt.show()

# Affichage de l'image binarisée
img_bin = Image.open("/content/imageM1_bin.ppm")
plt.figure(figsize=(5,5))
plt.imshow(img_bin, cmap='gray')
plt.title("Binary Image")
plt.axis("off")
plt.show()
