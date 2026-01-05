<?php
/**
 * Konfigurasi Database dan Session
 * Sistem Manajemen Karyawan
 */

// Konfigurasi Database
define('DB_HOST', 'sql212.infinityfree.com');
define('DB_USER', 'if0_40739473');
define('DB_PASS', 'u92i2Vg2Snv');
define('DB_NAME', 'if0_40739473_db_karyawan');
// Konfigurasi Aplikasi
define('APP_NAME', 'Sistem Manajemen Karyawan');
define('APP_VERSION', '1.0.0');
define('BASE_URL', 'http://localhost/Web/');

// Timezone
date_default_timezone_set('Asia/Jakarta');

// Koneksi Database
function getConnection() {
    static $conn = null;
    
    if ($conn === null) {
        $conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
        
        if ($conn->connect_error) {
            die("Koneksi gagal: " . $conn->connect_error);
        }
        
        $conn->set_charset("utf8mb4");
    }
    
    return $conn;
}

// Start session
if (session_status() === PHP_SESSION_NONE) {
    session_start();
}

// Fungsi untuk cek login
function isLoggedIn() {
    return isset($_SESSION['user_id']);
}

// Fungsi untuk cek role admin
function isAdmin() {
    return isset($_SESSION['role']) && $_SESSION['role'] === 'admin';
}

// Fungsi untuk redirect
function redirect($url) {
    header("Location: " . $url);
    exit();
}

// Fungsi untuk sanitasi input
function sanitize($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

// Fungsi untuk logging aktivitas
function logActivity($userId, $aksi, $detail = '', $conn = null) {
    if ($conn === null) {
        $conn = getConnection();
    }
    
    $ipAddress = $_SERVER['REMOTE_ADDR'];
    $stmt = $conn->prepare("INSERT INTO log_aktivitas (user_id, aksi, detail, ip_address) VALUES (?, ?, ?, ?)");
    $stmt->bind_param("isss", $userId, $aksi, $detail, $ipAddress);
    $stmt->execute();
    $stmt->close();
}

// Fungsi untuk format tanggal Indonesia
function formatTanggalIndo($tanggal) {
    $bulan = array(
        1 => 'Januari', 'Februari', 'Maret', 'April', 'Mei', 'Juni',
        'Juli', 'Agustus', 'September', 'Oktober', 'November', 'Desember'
    );
    
    $pecah = explode('-', $tanggal);
    return $pecah[2] . ' ' . $bulan[(int)$pecah[1]] . ' ' . $pecah[0];
}

// Fungsi untuk format rupiah
function formatRupiah($angka) {
    return 'Rp ' . number_format($angka, 0, ',', '.');
}

// Fungsi untuk response JSON
function jsonResponse($success, $message, $data = null) {
    header('Content-Type: application/json');
    echo json_encode([
        'success' => $success,
        'message' => $message,
        'data' => $data
    ]);
    exit();
}

// Cek koneksi saat file di-include
$conn = getConnection();
?>
