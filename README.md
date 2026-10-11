# Key Chain Screen 📟✨

Gantungan kunci pintar (Smart Key Chain) interaktif berukuran saku berbasis **ESP32-C3** dan layar **OLED SSD1306 128x64 (I2C)**. Proyek ini menampilkan animasi ekspresi mata robot dinamis serta pemutaran frame animasi video 30 FPS yang dapat diganti secara instan menggunakan sensor sentuh kapasitif **TTP223**.

---

## 🌟 Fitur Utama

- **Dual Display Mode**:
  1. **Mode RoboEyes (Mata Robot Interaktif)**:
     - Siklus emosi otomatis: *Default* (netral) ➔ *Happy* (senang) ➔ *Laugh* (tertawa) ➔ *Angry* (marah) ➔ kembali ke *Default*.
     - Dilengkapi fitur *Auto-Blinker* (kedipan mata alami) dan *Idle Mode* pada ~60 FPS.
  2. **Mode Video / Bitmap Player**:
     - Memutar animasi video monochrome 128x64 resolusi penuh sebanyak **425 frame** dengan kecepatan ~30 FPS (delay 33ms/frame).
     - Data frame disimpan langsung pada memori flash (`PROGMEM`) melalui [`include/VideoFrame.h`](include/VideoFrame.h).
- **Kontrol Sentuh Kapasitif (Touch Control)**:
  - Menggunakan sensor sentuh digital (TTP223).
  - Dilengkapi fitur **Double-Tap Detection** (threshold jeda 800 ms) dan **Debounce Filter** (60 ms) untuk berpindah mode secara mulus tanpa *accidental trigger*.
- **USB CDC on Boot**:
  - Dukungan serial monitor langsung melalui port USB bawaan ESP32-C3 (`ARDUINO_USB_CDC_ON_BOOT=1`).

---

## 🛠️ Komponen & Perangkat Keras

| Komponen | Spesifikasi / Keterangan |
| :--- | :--- |
| **Mikrokontroler** | ESP32-C3 SuperMini / ESP32-C3 DevKitM-1 |
| **Display** | 0.96" I2C Monochrome OLED Display (SSD1306, 128x64) |
| **Sensor Input** | Modul Sensor Sentuh Kapasitif TTP223 (Active HIGH) |
| **Daya** | Baterai Li-Po / 3.7V - 5V Micro USB/Type-C |

---

## 🔌 Skema Pinout (Wiring)

| Modul | Pin Modul | Pin ESP32-C3 | Keterangan |
| :--- | :--- | :--- | :--- |
| **OLED SSD1306** | `VCC` | `3V3` | Tegangan 3.3V |
| | `GND` | `GND` | Ground |
| | `SDA` | `GPIO 8` | I2C Data |
| | `SCL` | `GPIO 9` | I2C Clock |
| **TTP223 Touch** | `VCC` | `3V3` | Tegangan 3.3V |
| | `GND` | `GND` | Ground |
| | `I/O (SIG)` | `GPIO 2` | Digital Input |

> *Catatan: Jika menggunakan board ESP32-C3 varian lain, Anda dapat menyesuaikan pin I2C dan Touch pada bagian konfigurasi di [`src/main.cpp`](src/main.cpp).*

---

## 🕹️ Cara Penggunaan & Navigasi

1. **Nyalakan Perangkat**: Saat pertama kali dinyalakan (boot), perangkat akan otomatis masuk ke **Mode RoboEyes**.
2. **Double-Tap Sensor Sentuh**: Sentuh sensor TTP223 sebanyak 2 kali berturut-turut (dalam rentang waktu 800 ms) untuk beralih antara:
   - **Mode RoboEyes** ⮀ **Mode Video Frame Animation**.
3. **Loop Animasi**: Kedua mode akan terus berjalan dalam loop tanpa henti secara *non-blocking*.

---

## 📁 Struktur Direktori

```text
Key Chain Screen/
├── include/
│   ├── VideoFrame.h       # Array bitmap 425 frame animasi (PROGMEM)
│   └── README
├── lib/
│   └── README
├── src/
│   └── main.cpp           # Logika utama (RoboEyes, Video Loop, Touch Handler)
├── platformio.ini         # Konfigurasi board ESP32-C3 & library dependencies
├── .gitignore             # Mengabaikan file build .pio & cache editor
└── README.md              # Dokumentasi proyek
```

---

## 🚀 Panduan Instalasi & Upload

Proyek ini dibangun menggunakan **[PlatformIO](https://platformio.org/)**.

### 1. Clone Repositori
```bash
git clone https://github.com/Bentarraya/Key-Chain-Screen.git
cd Key-Chain-Screen
```

### 2. Buka di PlatformIO
- Buka **Visual Studio Code**.
- Pastikan ekstensi **PlatformIO IDE** sudah terinstal.
- Buka folder proyek (`File` > `Open Folder...`).

### 3. Build & Upload
1. Hubungkan board ESP32-C3 ke komputer via kabel USB.
2. Di pojok kiri bawah VS Code PlatformIO Toolbar:
   - Klik ikon **Checkmark (✓)** untuk melakukan **Build / Compile**.
   - Klik ikon **Panah Kanan (→)** untuk melakukan **Upload** firmware.
3. Buka **Serial Monitor** pada baudrate `115200` untuk melihat status inisialisasi OLED.

---

## ⚙️ Kustomisasi

### 1. Mengubah Durasi Ekspresi Mata
Pada [`src/main.cpp`](src/main.cpp), sesuaikan konstanta durasi waktu (dalam milidetik):
```cpp
#define EYE_DEFAULT_MS 3000   // Durasi mata netral (3 detik)
#define EYE_HAPPY_MS   3000   // Durasi mata senang (3 detik)
#define EYE_LAUGH_MS   2500   // Durasi mata ketawa (2.5 detik)
#define EYE_ANGRY_MS   3500   // Durasi mata marah (3.5 detik)
```

### 2. Mengganti Animasi Video
Anda dapat mengonversi animasi GIF atau klip video ke dalam array byte bitmap monochrome C++ menggunakan tools seperti **LCD Image Converter** atau script Python image-to-bitmap, kemudian perbarui file [`include/VideoFrame.h`](include/VideoFrame.h) beserta konstanta `TOTAL_FRAMES` dan `FRAME_DELAY`.

---

## 📚 Library & Dependensi

- [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes.git) - Animasi mata robot interaktif
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) - Driver layar OLED SSD1306
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) - Core grafis Adafruit

---

## 👤 Pembuat

Dibuat oleh **[Bentarraya](https://github.com/Bentarraya)**.
