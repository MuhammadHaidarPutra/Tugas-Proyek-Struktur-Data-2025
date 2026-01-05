# SISTEM MANAJEMEN KARYAWAN - WEB VERSION

Website manajemen karyawan berbasis web dengan HTML, CSS, PHP, JavaScript dan MySQL. Sistem ini merupakan versi web dari aplikasi TP SDATA yang dibuat dengan C++.

## 📋 FITUR UTAMA

### Admin Panel
- **Dashboard**: Statistik dan overview sistem
- **Manajemen Karyawan**: CRUD data karyawan (tambah, edit, hapus)
- **Manajemen Absensi**: Input dan monitoring absensi karyawan
- **Manajemen Cuti**: Approve/reject pengajuan cuti
- **Jadwal Kerja**: Atur shift dan jadwal karyawan
- **Payroll**: Generate dan kelola penggajian
- **Evaluasi**: Penilaian kinerja karyawan
- **Laporan**: Statistik dan laporan lengkap
- **Manajemen User**: Kelola akun pengguna

### Karyawan Panel
- **Dashboard**: Overview personal
- **Profil**: Lihat dan edit profil
- **Absensi**: Lihat riwayat absensi
- **Pengajuan Cuti**: Ajukan dan pantau status cuti
- **Jadwal**: Lihat jadwal kerja
- **Slip Gaji**: Akses slip gaji
- **Evaluasi**: Lihat hasil evaluasi

## 🛠️ TEKNOLOGI

- **Frontend**: HTML5, CSS3, JavaScript (Vanilla)
- **Backend**: PHP 7.4+
- **Database**: MySQL 5.7+
- **Icons**: Font Awesome 6.0
- **Design**: Responsive & Modern UI

## 📁 STRUKTUR FILE

```
Web/
├── admin/              # Halaman admin
│   ├── dashboard.php
│   ├── karyawan.php
│   ├── absensi.php
│   ├── cuti.php
│   ├── jadwal.php
│   ├── payroll.php
│   ├── evaluasi.php
│   ├── laporan.php
│   ├── users.php
│   ├── sidebar.php
│   └── topbar.php
│
├── karyawan/           # Halaman karyawan
│   ├── dashboard.php
│   ├── profil.php
│   ├── cuti.php
│   ├── sidebar.php
│   └── topbar.php
│
├── assets/             # File statis
│   ├── css/
│   │   └── style.css
│   └── js/
│       ├── login.js
│       ├── karyawan.js
│       ├── absensi.js
│       └── evaluasi.js
│
├── config.php          # Konfigurasi database
├── index.php           # Halaman login
├── login.php           # Proses login
├── logout.php          # Proses logout
├── database.sql        # Schema database
└── README.md           # Dokumentasi
```

## 🚀 INSTALASI

### 1. Requirements
- PHP 7.4 atau lebih tinggi
- MySQL 5.7 atau lebih tinggi
- Web Server (Apache/Nginx)
- phpMyAdmin (opsional)

### 2. Langkah Instalasi

**A. Setup Database**
```sql
1. Buat database baru:
   CREATE DATABASE db_karyawan;

2. Import file database.sql:
   - Buka phpMyAdmin
   - Pilih database db_karyawan
   - Import file database.sql
```

**B. Konfigurasi**
```php
1. Edit file config.php:
   - DB_HOST: 'localhost'
   - DB_USER: 'root'
   - DB_PASS: '' (kosongkan jika tidak ada password)
   - DB_NAME: 'db_karyawan'
   - BASE_URL: sesuaikan dengan URL Anda
```

**C. Upload File**
```
1. Copy semua file ke folder web server:
   - XAMPP: C:/xampp/htdocs/Web/
   - WAMP: C:/wamp/www/Web/
   - Linux: /var/www/html/Web/

2. Set permission (Linux):
   chmod -R 755 /var/www/html/Web/
```

### 3. Akses Aplikasi

Buka browser dan akses:
```
http://localhost/Web/
```

## 🔐 AKUN DEFAULT

### Admin
- Username: `admin`
- Password: `admin123`

### Karyawan
- Username: `john` / Password: `john123`
- Username: `jane` / Password: `jane123`
- Username: `ahmad` / Password: `ahmad123`

## 📊 DATABASE SCHEMA

### Tabel Utama

**users**: Akun login user
- id, username, password, role, nama, id_karyawan

**karyawan**: Data karyawan
- id, nama, jabatan, departemen, email, telepon, tanggal_masuk, gaji_pokok, status

**absensi**: Data absensi
- id, id_karyawan, tanggal, jam_masuk, jam_keluar, durasi_kerja, status

**cuti**: Pengajuan cuti
- id, id_karyawan, tanggal_mulai, tanggal_selesai, jenis_cuti, status

**jadwal**: Jadwal kerja
- id, id_karyawan, tanggal, shift, jam_mulai, jam_selesai

**payroll**: Data gaji
- id, id_karyawan, bulan, tahun, gaji_pokok, tunjangan, potongan, total_gaji

**evaluasi**: Penilaian karyawan
- id, id_karyawan, periode, nilai_kinerja, nilai_kedisiplinan, nilai_kerjasama

**log_aktivitas**: Log sistem
- id, user_id, aksi, detail, ip_address

## 🎨 FITUR DETAIL

### 1. Manajemen Karyawan
- Tambah karyawan baru
- Edit data karyawan
- Hapus karyawan
- Search & filter
- Status aktif/non-aktif

### 2. Sistem Absensi
- Input absensi manual
- Jam masuk & keluar
- Hitung durasi kerja otomatis
- Status: Hadir, Izin, Sakit, Alfa, Cuti
- Filter by tanggal

### 3. Manajemen Cuti
- Pengajuan cuti oleh karyawan
- Approval by admin
- Hitung sisa cuti otomatis (12 hari/tahun)
- Jenis cuti: Tahunan, Sakit, Penting, Lainnya

### 4. Jadwal Kerja
- 3 shift: Pagi (07:00-15:00), Siang (15:00-23:00), Malam (23:00-07:00)
- Assign jadwal per karyawan
- View jadwal harian

### 5. Payroll
- Generate payroll otomatis untuk semua karyawan
- Perhitungan:
  * Tunjangan: 20% dari gaji pokok
  * Potongan: 5% dari gaji pokok
- Status: Draft / Dibayar
- Filter by bulan & tahun

### 6. Evaluasi Karyawan
- 3 aspek penilaian:
  * Nilai Kinerja (1-100)
  * Nilai Kedisiplinan (1-100)
  * Nilai Kerjasama (1-100)
- Hitung rata-rata otomatis
- Track evaluator & tanggal

### 7. Laporan & Statistik
- Statistik absensi bulan ini
- Statistik per departemen
- Top 10 kehadiran
- Total payroll
- Rata-rata evaluasi
- Print-friendly

### 8. Log Aktivitas
- Track semua aktivitas admin
- Simpan: user, aksi, detail, timestamp, IP address
- Security audit trail

## 🔒 KEAMANAN

1. **Password**: MD5 hashing
2. **Session Management**: PHP Session
3. **Input Sanitization**: htmlspecialchars, trim, stripslashes
4. **SQL Injection Prevention**: Prepared Statements
5. **Access Control**: Role-based (admin/karyawan)
6. **Logout**: Destroy session

## 📱 RESPONSIVE DESIGN

- Desktop: Full layout dengan sidebar
- Tablet: Adaptif layout
- Mobile: Touch-friendly interface

## 🎯 FUTURE ENHANCEMENTS

- [ ] Upload foto profil karyawan
- [ ] Export laporan ke PDF/Excel
- [ ] Real-time notifications
- [ ] Absensi dengan foto/GPS
- [ ] Multi-language support
- [ ] Advanced charts dengan Chart.js
- [ ] Email notifications
- [ ] API REST untuk mobile app
- [ ] Two-factor authentication

## 🐛 TROUBLESHOOTING

### Database Connection Error
```
Solusi:
1. Cek konfigurasi di config.php
2. Pastikan MySQL service running
3. Cek username & password database
```

### Session Not Working
```
Solusi:
1. Pastikan session_start() berjalan
2. Cek permission folder session PHP
3. Clear browser cache & cookies
```

### Page Not Found
```
Solusi:
1. Cek BASE_URL di config.php
2. Pastikan .htaccess configured (jika ada)
3. Cek file path
```

## 📞 SUPPORT

Untuk pertanyaan atau masalah:
- Email: support@example.com
- GitHub Issues: [link-repo]

## 📄 LICENSE

MIT License - Free to use & modify

## 👨‍💻 DEVELOPER

Developed by: [Your Name]
Version: 1.0.0
Last Update: December 2025

---

**Catatan**: 
- Sistem ini dikembangkan untuk tujuan pembelajaran dan dapat dimodifikasi sesuai kebutuhan
- Pastikan selalu backup database sebelum melakukan update
- Gunakan HTTPS di production untuk keamanan maksimal
