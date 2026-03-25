import numpy as np
import matplotlib.pyplot as plt

# fonction pour charger l'image
def load_ppm(filename):
    with open(filename, 'r') as f:
        assert f.readline().strip() == "P3"

        width, height = map(int, f.readline().split())
        maxval = int(f.readline())

        data = []
        for line in f:
            data.extend([int(x) for x in line.split()])

        img = np.array(data, dtype=np.uint8)
        img = img.reshape((height, width, 3))

        return img

# charger image
img = load_ppm("resultat.ppm")

# affichage
plt.figure(figsize=(4,4))
plt.imshow(img)
plt.title("Résultat Hough naïf")
plt.show()
