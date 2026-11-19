# 🚗 MAUNC 4WD Bluetooth Robot Car

![MAUNC 4WD Bluetooth Robot Car](assets/robot-4wd.jpeg)

## 📌 Tentang Proyek

**MAUNC 4WD Bluetooth Robot Car** merupakan proyek pengembangan mobil robot empat roda (4WD) berbasis **ESP32 DevKit V1** yang dikendalikan secara nirkabel menggunakan koneksi Bluetooth.

Proyek ini dirancang untuk menggabungkan beberapa konsep dasar dalam bidang robotika dan sistem tertanam (*embedded systems*), mulai dari pengendalian motor DC, komunikasi Bluetooth, pembacaan sensor ultrasonik, hingga pengendalian perangkat output berupa lampu LED dan buzzer.

Mobil robot ini dapat dikendalikan melalui aplikasi Bluetooth Controller pada smartphone. Selain kendali manual, sistem juga dilengkapi sensor ultrasonik HC-SR04 yang berfungsi mendeteksi objek di depan mobil dan mengaktifkan pengereman otomatis ketika jarak objek mencapai batas yang telah ditentukan.

Proyek ini dikembangkan secara kolaboratif oleh dua anggota sebagai bagian dari kegiatan pembelajaran, eksplorasi, dan pengembangan teknologi robotika di lingkungan **MAUNC Robotika UNSIL**.

## ✨ Fitur Utama

- **Bluetooth Control:** Mengendalikan pergerakan mobil melalui smartphone menggunakan komunikasi Bluetooth.
- **4WD Drive System:** Menggunakan empat motor DC TT untuk sistem penggerak empat roda.
- **Forward & Backward Movement:** Mendukung pergerakan maju dan mundur.
- **Directional Control:** Mendukung pergerakan belok kiri dan kanan melalui pengaturan arah putaran motor.
- **Ultrasonic Obstacle Detection:** Mendeteksi objek di depan mobil menggunakan sensor HC-SR04.
- **Automatic Braking:** Mengaktifkan pengereman ketika sensor mendeteksi objek pada jarak 30 cm atau kurang saat mobil bergerak maju.
- **Forward Safety Check:** Membatasi perintah maju apabila jarak objek terdeteksi 20 cm atau kurang.
- **LED Lighting System:** Menghidupkan dan mematikan lampu melalui tombol pada gamepad.
- **Electronic Horn:** Mengaktifkan buzzer sebagai klakson melalui tombol gamepad.
- **Rechargeable Battery:** Menggunakan baterai Li-Po 3S sebagai sumber daya utama dengan dukungan DC-DC Buck Converter.

## 🧰 Alat dan Komponen

Berikut merupakan daftar komponen yang digunakan dalam pembuatan proyek:

| No. | Komponen                         |     Jumlah |
| --- | -------------------------------- | ---------: |
| 1   | ESP32 DevKit V1                  |     1 buah |
| 2   | L298N Dual H-Bridge Motor Driver |     1 buah |
| 3   | Li-Po Battery 3S                 |     1 buah |
| 4   | DC-DC Buck Converter             |     1 buah |
| 5   | TT DC Gear Motor                 |     4 buah |
| 6   | Robot Car Wheel                  |     4 buah |
| 7   | Ultrasonic Sensor HC-SR04        |     1 buah |
| 8   | Active Buzzer                    |     1 buah |
| 9   | LED Indicator                    |     3 buah |
| 10  | 4WD Robot Car Chassis            |      1 set |
| 11  | Jumper Wires                     | Secukupnya |

## ⚙️ Konfigurasi Pin ESP32

Berikut adalah konfigurasi pin yang digunakan berdasarkan program utama proyek.

### 1. Motor Driver L298N

| Fungsi | Pin ESP32 |
| ------ | --------- |
| IN1    | GPIO 25   |
| IN2    | GPIO 27   |
| ENA    | GPIO 18   |
| IN3    | GPIO 26   |
| IN4    | GPIO 33   |
| ENB    | GPIO 19   |

### 2. Sensor Ultrasonik HC-SR04

| Fungsi | Pin ESP32 |
| ------ | --------- |
| TRIG   | GPIO 22   |
| ECHO   | GPIO 23   |

### 3. Output Tambahan

| Komponen                | Pin ESP32 |
| ----------------------- | --------- |
| Lampu LED               | GPIO 15   |
| Active Buzzer (Klakson) | GPIO 32   |

## 🎮 Kontrol Mobil

Pengendalian mobil dilakukan melalui aplikasi Bluetooth Controller yang terhubung dengan perangkat ESP32.

Nama perangkat Bluetooth:

**`MAUNC_Robot_2`**

Berikut daftar karakter perintah yang digunakan dalam program:

| Tombol / Perintah | Karakter | Fungsi                     |
| ----------------- | -------- | -------------------------- |
| Forward           | `F`      | Mobil bergerak maju        |
| Backward          | `B`      | Mobil bergerak mundur      |
| Left              | `L`      | Mobil berbelok ke kiri     |
| Right             | `R`      | Mobil berbelok ke kanan    |
| Stop              | `S`      | Menghentikan mobil         |
| Circle            | `0`      | Mengubah kondisi lampu LED |
| X                 | `X`      | Mengaktifkan klakson       |

**Catatan:** Program juga mengenali karakter `C`, `T`, `A`, dan `P` sebagai perintah berhenti.

## 🧠 Cara Kerja Sistem

### 1. Koneksi Bluetooth

ESP32 bertindak sebagai penerima perintah dari smartphone melalui komunikasi Bluetooth Serial. Setiap karakter yang diterima akan diproses menggunakan fungsi `executeCommand()`.

### 2. Sistem Penggerak Motor

Motor DC dikendalikan menggunakan driver L298N. Pengaturan arah putaran motor dilakukan melalui kombinasi sinyal HIGH dan LOW pada pin input driver.

Kecepatan motor diatur menggunakan PWM dengan nilai:

- `MOTOR_SPEED = 200`
- `BRAKE_SPEED = 200`

### 3. Sistem Deteksi Objek

Sensor ultrasonik HC-SR04 digunakan untuk mengukur jarak antara mobil dan objek di depannya.

Pengukuran jarak dilakukan dengan memanfaatkan waktu pantulan gelombang ultrasonik. Hasil pengukuran kemudian digunakan sebagai dasar pengambilan keputusan pada sistem pengereman.

### 4. Sistem Pengereman Otomatis

Ketika mobil sedang bergerak maju dan sensor mendeteksi objek pada jarak **30 cm atau kurang**, sistem akan mengaktifkan fungsi `brakeCar()`.

Selain itu, program memiliki pemeriksaan tambahan pada fungsi `forward()`. Apabila jarak objek terdeteksi **20 cm atau kurang**, perintah maju akan ditolak dan mobil dihentikan menggunakan fungsi `stopCar()`.

Dengan demikian, sistem memiliki dua batas pemeriksaan jarak yang berbeda sesuai kondisi pengendalian.

### 5. Sistem Lampu LED

Lampu dikendalikan melalui karakter `0` dari gamepad. Setiap kali tombol ditekan, status lampu akan berubah:

- Kondisi awal: OFF.
- Tekan tombol: ON.
- Tekan kembali: OFF.

Pengaturan ini menggunakan variabel `statusLampu` dan fungsi `toggleLampu()`.

### 6. Sistem Klakson

Buzzer aktif digunakan sebagai klakson yang dikendalikan menggunakan tombol X pada gamepad.

Ketika ESP32 menerima karakter `X`, fungsi `aktifkanKlakson()` akan mengaktifkan buzzer dan memperbarui waktu aktivasi menggunakan `millis()`.

Buzzer akan dimatikan apabila tidak menerima perintah `X` lagi selama 200 milidetik. Apabila aplikasi mengirimkan karakter `X` berulang ketika tombol ditahan, buzzer dapat terus aktif selama perintah tersebut diterima.

## 💻 Software dan Library

Perangkat lunak yang digunakan dalam pengembangan proyek ini:

- **Arduino IDE** — sebagai lingkungan pemrograman dan proses upload program ke ESP32.
- **ESP32 Arduino Core** — menyediakan dukungan pemrograman ESP32.
- **BluetoothSerial.h** — digunakan untuk komunikasi Bluetooth Serial antara smartphone dan ESP32.

Library `BluetoothSerial.h` disertakan melalui dukungan ESP32 Arduino Core.

## 🚀 Cara Menjalankan Program

1. Siapkan seluruh komponen sesuai daftar alat dan bahan.
2. Rangkai ESP32, motor driver L298N, sensor ultrasonik, lampu, dan buzzer sesuai konfigurasi pin.
3. Hubungkan seluruh sistem catu daya dengan memperhatikan kebutuhan tegangan masing-masing komponen.
4. Instal Arduino IDE dan dukungan board ESP32.
5. Buka file program utama dengan ekstensi `.ino`.
6. Pilih board ESP32 yang sesuai, misalnya **ESP32 Dev Module**.
7. Pilih port komunikasi yang terhubung dengan ESP32.
8. Upload program ke ESP32.
9. Aktifkan Bluetooth pada smartphone.
10. Hubungkan smartphone dengan perangkat **MAUNC_Robot_2**.
11. Buka aplikasi Bluetooth Controller dan mulai kendalikan mobil robot.

## 🔌 Catatan Perakitan dan Keselamatan Komponen

- Pastikan tegangan keluaran DC-DC Buck Converter sesuai dengan kebutuhan komponen yang digunakan.
- Periksa spesifikasi tegangan TT DC Gear Motor sebelum menghubungkannya ke driver motor.
- Pastikan seluruh rangkaian menggunakan GND bersama (*common ground*) sesuai kebutuhan sistem.
- Pin ECHO pada HC-SR04 dapat menghasilkan sinyal 5V. Gunakan pembagi tegangan atau level shifter sebelum menghubungkannya ke GPIO23 ESP32.
- Pastikan koneksi ENA dan ENB pada L298N mendukung pengaturan PWM dari ESP32. Pada modul dengan jumper enable, jumper perlu dilepas ketika pin tersebut dikendalikan melalui PWM eksternal.
- Gunakan resistor pembatas arus untuk LED.
- Pastikan buzzer tidak menarik arus melebihi kemampuan GPIO ESP32. Gunakan transistor sebagai driver apabila diperlukan.
- Lakukan pengujian awal dengan kecepatan rendah dan periksa suhu driver motor ketika fitur pengereman aktif.

## 📁 Struktur Repository

Struktur folder yang disarankan:

```text
MAUNC-4WD-Bluetooth-Robot-Car/
│
├── README.md
│
├── src/
│   └── MAUNC_Robot_2.ino
│
├── assets/
│   └── robot-4wd.jpg
│
└── LICENSE
```

Keterangan:

- `README.md` — dokumentasi utama proyek.
- `src/` — berisi source code program ESP32.
- `assets/` — berisi dokumentasi foto atau gambar proyek.
- `LICENSE` — informasi lisensi penggunaan dan distribusi source code.

## 👥 Tim Pengembang

Proyek ini dikembangkan secara kolaboratif oleh:

| Nama                    | Peran                                     |
| ----------------------- | ----------------------------------------- |
| **Irga Mochamad Rafly** | Pengembangan dan pemrograman sistem robot |
| **Adhika Mahesa**       | Pengembangan dan perakitan proyek robot   |

**Komunitas:** MAUNC Robotika UNSIL

## 🎯 Tujuan Pengembangan

Proyek ini bertujuan untuk mengimplementasikan pengetahuan dasar robotika dan pemrograman mikrokontroler melalui pembuatan mobil robot 4WD yang dapat dikendalikan secara nirkabel.

Melalui proyek ini, diharapkan dapat menjadi sarana pembelajaran dan dokumentasi pengembangan teknologi, khususnya dalam bidang:

- Internet of Things (IoT) dan sistem tertanam.
- Mikrokontroler ESP32.
- Komunikasi Bluetooth.
- Sistem kendali motor DC.
- Sensor ultrasonik dan deteksi objek.
- Integrasi perangkat keras dan perangkat lunak.

## 📄 Lisensi dan Pengembangan

Repository ini disediakan sebagai dokumentasi proyek pembelajaran dan pengembangan robotika di lingkungan komunitas MAUNC Robotika UNSIL.

Source code dapat dipelajari dan dikembangkan lebih lanjut dengan tetap mencantumkan atribusi kepada pengembang proyek.

---

\<p align="center">
&#x20; \*\*MAUNC Robotika UNSIL\*\*

*Learn, Build, Innovate.*

\</p>
