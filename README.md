# Panduan Menjalankan Program (How to Run)

Repository ini berisi implementasi algoritma **Enemy AI Dungeon (Range Detection & A* Pathfinding)** menggunakan bahasa **C++**.

Laporan lengkap dan analisis jawaban tugas dapat dilihat pada file: **[JAWABAN_TUGAS_PERT_1.md](./JAWABAN_TUGAS_PERT_1.md)**.

---

### 1. Kompilasi Program (Menggunakan g++)

Buka terminal (**Git Bash**, **PowerShell**, atau **Command Prompt**) di folder proyek, lalu jalankan perintah berikut:

```bash
g++ -static -O2 main.cpp -o dungeon.exe
```

> **Catatan:** Flag `-static` memastikan program dapat dijalankan mandiri di sistem operasi Windows mana pun tanpa memerlukan dependensi file `.dll` eksternal.

---

### 2. Menjalankan File Hasil Kompilasi

- **Di Git Bash / Linux / macOS:**
  ```bash
  ./dungeon.exe
  ```
- **Di PowerShell / Command Prompt (CMD):**
  ```powershell
  .\dungeon.exe
  ```

---

### 3. Pilihan Mode Permainan

Saat program dijalankan, terdapat 2 pilihan mode:

1. **Mode 1: Simulasi Otomatis *(Sangat Direkomendasikan untuk Demo Dosen)***
   - Player bergerak otomatis mengikuti urutan rute yang sudah ditentukan di dalam dungeon.
   - Enemy menghitung jarak real-time, mendeteksi jika Player berada dalam jangkauan, mencari jalur (A*), dan mengejar langkah demi langkah.
   - Cukup tekan tombol `[Enter]` di terminal pada setiap giliran (*turn*) untuk mengamati prosesnya.

2. **Mode 2: Kontrol Manual (WASD)**
   - Pemain mengendalikan karakter Player secara interaktif menggunakan tombol:
     - `W` : Bergerak ke Atas
     - `S` : Bergerak ke Bawah
     - `A` : Bergerak ke Kiri
     - `D` : Bergerak ke Kanan
     - `Q` : Keluar dari permainan
   - Setiap kali Player melangkah, Enemy akan otomatis merespons dan mengejar Anda.

---

### 4. Keterangan Simbol Peta di Terminal

Peta dungeon ditampilkan menggunakan matriks ASCII:
- `[P]` : Posisi Player
- `[E]` : Posisi Enemy
- `###` : Tembok penghalang (tidak dapat dilewati)
- ` . ` : Jalan kosong yang dapat dilalui
- ` * ` : Rute jalur A* yang sedang direncanakan oleh Enemy
- `[!]` : Kondisi saat Player berhasil ditangkap oleh Enemy (Game Over)
