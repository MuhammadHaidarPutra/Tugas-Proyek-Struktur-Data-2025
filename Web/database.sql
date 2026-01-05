-- Database untuk Sistem Manajemen Karyawan

-- Tabel Users untuk login
CREATE TABLE users (
    id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('admin', 'karyawan') NOT NULL,
    nama VARCHAR(100) NOT NULL,
    id_karyawan INT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel Karyawan
CREATE TABLE karyawan (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nama VARCHAR(100) NOT NULL,
    jabatan VARCHAR(50) NOT NULL,
    departemen VARCHAR(50) NOT NULL,
    email VARCHAR(100),
    telepon VARCHAR(20),
    tanggal_masuk DATE,
    gaji_pokok DECIMAL(15,2) DEFAULT 0,
    status ENUM('aktif', 'non-aktif') DEFAULT 'aktif',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabel Absensi
CREATE TABLE absensi (
    id INT PRIMARY KEY AUTO_INCREMENT,
    id_karyawan INT NOT NULL,
    tanggal DATE NOT NULL,
    jam_masuk TIME,
    jam_keluar TIME,
    durasi_kerja INT DEFAULT 0, -- dalam menit
    status ENUM('hadir', 'izin', 'sakit', 'alfa', 'cuti') DEFAULT 'hadir',
    keterangan TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_karyawan) REFERENCES karyawan(id) ON DELETE CASCADE
);

-- Tabel Cuti
CREATE TABLE cuti (
    id INT PRIMARY KEY AUTO_INCREMENT,
    id_karyawan INT NOT NULL,
    tanggal_mulai DATE NOT NULL,
    tanggal_selesai DATE NOT NULL,
    jenis_cuti ENUM('tahunan', 'sakit', 'penting', 'lainnya') DEFAULT 'tahunan',
    alasan TEXT,
    status ENUM('pending', 'disetujui', 'ditolak') DEFAULT 'pending',
    approved_by INT NULL,
    approved_at TIMESTAMP NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_karyawan) REFERENCES karyawan(id) ON DELETE CASCADE
);

-- Tabel Jadwal
CREATE TABLE jadwal (
    id INT PRIMARY KEY AUTO_INCREMENT,
    id_karyawan INT NOT NULL,
    tanggal DATE NOT NULL,
    shift ENUM('pagi', 'siang', 'malam') NOT NULL,
    jam_mulai TIME NOT NULL,
    jam_selesai TIME NOT NULL,
    keterangan TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_karyawan) REFERENCES karyawan(id) ON DELETE CASCADE
);

-- Tabel Payroll
CREATE TABLE payroll (
    id INT PRIMARY KEY AUTO_INCREMENT,
    id_karyawan INT NOT NULL,
    bulan INT NOT NULL,
    tahun INT NOT NULL,
    gaji_pokok DECIMAL(15,2) NOT NULL,
    tunjangan DECIMAL(15,2) DEFAULT 0,
    potongan DECIMAL(15,2) DEFAULT 0,
    total_gaji DECIMAL(15,2) NOT NULL,
    status ENUM('draft', 'dibayar') DEFAULT 'draft',
    tanggal_bayar DATE NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_karyawan) REFERENCES karyawan(id) ON DELETE CASCADE
);

-- Tabel Evaluasi
CREATE TABLE evaluasi (
    id INT PRIMARY KEY AUTO_INCREMENT,
    id_karyawan INT NOT NULL,
    periode VARCHAR(20) NOT NULL,
    nilai_kinerja INT NOT NULL CHECK(nilai_kinerja BETWEEN 1 AND 100),
    nilai_kedisiplinan INT NOT NULL CHECK(nilai_kedisiplinan BETWEEN 1 AND 100),
    nilai_kerjasama INT NOT NULL CHECK(nilai_kerjasama BETWEEN 1 AND 100),
    catatan TEXT,
    evaluator VARCHAR(100),
    tanggal_evaluasi DATE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_karyawan) REFERENCES karyawan(id) ON DELETE CASCADE
);

-- Tabel Log Aktivitas
CREATE TABLE log_aktivitas (
    id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT NOT NULL,
    aksi VARCHAR(255) NOT NULL,
    detail TEXT,
    ip_address VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Insert data admin default
INSERT INTO users (username, password, role, nama) VALUES 
('admin', MD5('admin123'), 'admin', 'Administrator');

-- Insert sample karyawan
INSERT INTO karyawan (nama, jabatan, departemen, email, telepon, tanggal_masuk, gaji_pokok) VALUES
('John Doe', 'Manager', 'IT', 'john@company.com', '08123456789', '2023-01-15', 8000000),
('Jane Smith', 'Staff', 'HR', 'jane@company.com', '08234567890', '2023-03-20', 5000000),
('Ahmad Rizki', 'Supervisor', 'Finance', 'ahmad@company.com', '08345678901', '2023-06-10', 7000000);

-- Insert user karyawan
INSERT INTO users (username, password, role, nama, id_karyawan) VALUES
('john', MD5('john123'), 'karyawan', 'John Doe', 1),
('jane', MD5('jane123'), 'karyawan', 'Jane Smith', 2),
('ahmad', MD5('ahmad123'), 'karyawan', 'Ahmad Rizki', 3);

-- Create indexes untuk performa
CREATE INDEX idx_absensi_karyawan ON absensi(id_karyawan);
CREATE INDEX idx_absensi_tanggal ON absensi(tanggal);
CREATE INDEX idx_cuti_karyawan ON cuti(id_karyawan);
CREATE INDEX idx_jadwal_karyawan ON jadwal(id_karyawan);
CREATE INDEX idx_payroll_karyawan ON payroll(id_karyawan);
CREATE INDEX idx_evaluasi_karyawan ON evaluasi(id_karyawan);
