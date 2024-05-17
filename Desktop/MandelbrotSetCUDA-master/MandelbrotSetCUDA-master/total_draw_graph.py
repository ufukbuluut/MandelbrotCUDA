import matplotlib.pyplot as plt

# Dosya yolları ve renklerin listesi
file_paths = [
    (r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\1speedup_efficiency_results.txt", 'b'),
    (r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\2speedup_efficiency_results.txt", 'y'),
    (r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\3speedup_efficiency_results.txt", 'r'),
    (r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\4speedup_efficiency_results.txt", 'c'),
    (r"C:\Users\ufukb\Desktop\MandelbrotSetCUDA-master\MandelbrotSetCUDA-master\5speedup_efficiency_results.txt", 'm')
]
hw = [
    (800, 600),
    (1024, 768),
    (1280, 720),
    (1600, 900),
    (1920, 1080)
]
# Grafiği oluşturma
plt.figure(figsize=(12, 8))  # Daha büyük boyut

# Speedup grafiği oluşturma
plt.subplot(2, 1, 1)

i=0
for file_path, color in file_paths:

    sizes = []
    speedups = []

    # Dosyadan verileri okuma
    with open(file_path, 'r') as file:
        next(file)  # İlk satırı atla (başlık satırı)
        for line in file:
            size, speedup, _ = line.split()
            sizes.append(int(size))
            speedups.append(float(speedup))
    print(hw[i])
    plt.plot(sizes, speedups, marker='o', linestyle='-', color=color, label=f'Speedup: {hw[i]}{file_path.split("master")[-1]}')  # Her dosya için farklı renk ve etiket
    i=i+1
plt.title('Speedup', fontsize=14)
plt.xlabel('Block Size (AxA)', fontsize=12)
plt.ylabel('Speedup', fontsize=12)
plt.grid(True)  # Grid ekle
plt.legend()  # Açıklamaları ekle

# Efficiency grafiği oluşturma
plt.subplot(2, 1, 2)
j=0
for file_path, color in file_paths:
    sizes = []
    efficiencies = []

    # Dosyadan verileri okuma
    with open(file_path, 'r') as file:
        next(file)  # İlk satırı atla (başlık satırı)
        for line in file:
            size, _, efficiency = line.split()
            sizes.append(int(size))
            efficiencies.append(float(efficiency))

    plt.plot(sizes, efficiencies, marker='s', linestyle='--', color=color, label=f'Efficiency: {hw[j]}{file_path.split("master")[-1]}')  # Her dosya için farklı renk ve etiket
    j=j+1
plt.title('Efficiency', fontsize=14)
plt.xlabel('Size', fontsize=12)
plt.ylabel('Efficiency', fontsize=12)
plt.grid(True)  # Grid ekle
plt.legend()  # Açıklamaları ekle

# Grafiği gösterme
plt.tight_layout()
plt.show()
