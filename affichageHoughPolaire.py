import matplotlib.pyplot as plt
import numpy as np
import re

def _read_next_line(f_obj):
    line = f_obj.readline().decode('ascii').strip()
    while not line or line.startswith('#'):
        line = f_obj.readline().decode('ascii').strip()
    return line

def load_ppm(filename):
    with open(filename, 'rb') as f:
        header = _read_next_line(f) 

        width_height_line = _read_next_line(f)
        width, height = map(int, width_height_line.split())

        max_val_line = _read_next_line(f)
        max_val = int(max_val_line)

        data = []
       
        if header == 'P3': 
            for line_data in f:
                line_str = line_data.decode('ascii').strip()
                if not line_str.startswith('#'):
                    data.extend(map(int, line_str.split()))
            pixels = np.array(data, dtype=np.uint8).reshape((height, width, 3))
            return pixels
        elif header == 'P2': 
            for line_data in f:
                line_str = line_data.decode('ascii').strip()
                if not line_str.startswith('#'):
                    data.extend(map(int, line_str.split()))
            pixels = np.array(data, dtype=np.uint8).reshape((height, width))
            return pixels
        else:
            raise ValueError(f"Unsupported PPM format: {header}. Only P2 and P3 are supported.")

# Charger accumulateur
acc = load_ppm("acc_nms.ppm")

# Affichage
plt.imshow(acc, cmap='hot')
plt.colorbar()
plt.title("Accumulateur Hough")
plt.xlabel("theta")
plt.ylabel("rho")
plt.show()

# Affichage de l'image avec les droites détectées
# Image de base
img = load_ppm("imageM1_bin.ppm")

height, width = img.shape[:2]

plt.imshow(img, cmap ='grey')

# Droites détectées
# Coordonnées à changer en fonction du résultat de Hough polaire
droites = [
    (-10.4264, -1.53589),
    (10.5736, 1.53589),
    (-1.42641,-1.53589),
    (1.57359, 1.53589),
    (8.57359, 1.55334)
]

# Tracé des droites
for rho, theta in droites:
    if abs(np.sin(theta)) > 1e-6:
        x_vals = np.arange(0, width)
        y_vals = (rho - x_vals * np.cos(theta)) / np.sin(theta)

        valid_indices = (y_vals >= 0) & (y_vals < height)
        plt.plot(y_vals[valid_indices] + 4*np.pi, x_vals[valid_indices], color='red', linestyle='-', linewidth=1)
    
    else:
        x_intercept = rho / np.cos(theta)
        if 0 <= x_intercept < width:
            plt.axvline(x = x_intercept, color='red', linestyle='-', linewidth=1)

plt.title("Droites détectées")
plt.xlim(0, width)
plt.ylim(height, 0)
plt.show()
