import matplotlib.pyplot as plt

# Dosyadan verileri okuma
sizes = []
speedups = []
efficiencies = []

with open(r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\speedup_efficiency_results.txt", 'r') as file:
    next(file)  # İlk satırı atla (başlık satırı)
    for line in file:
        size, speedup, efficiency = line.split()
        sizes.append(int(size))
        speedups.append(float(speedup))
        efficiencies.append(float(efficiency))

# Grafiği oluşturma
plt.figure(figsize=(12, 8))  # Daha büyük boyut

# Speedup grafiği
plt.subplot(2, 1, 1)
plt.plot(sizes, speedups, marker='o', linestyle='-', color='b', markersize=3)  # Nokta boyutunu küçült
plt.title('Speedup', fontsize=14)
plt.xlabel('Block Size (AxA)', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.grid(True)  # Kare grid ekle

# Efficiency grafiği
plt.subplot(2, 1, 2)
plt.plot(sizes, efficiencies, marker='s', linestyle='--', color='r', markersize=3)  # Nokta boyutunu küçült
plt.title('Efficiency', fontsize=14)
plt.xlabel('Size', fontsize=12)
plt.ylabel('Efficiency', fontsize=12)
plt.grid(True)  # Kare grid ekle

# Grafiği gösterme
plt.tight_layout()
plt.show()
