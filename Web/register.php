<?php
require_once 'config.php';

if (isLoggedIn()) {
    if (isAdmin()) {
        redirect('admin/dashboard.php');
    } else {
        redirect('karyawan/dashboard.php');
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = sanitize($_POST['username']);
    $password = $_POST['password'];
    $nama = sanitize($_POST['nama']);
    $role = $_POST['role'];

    // Cek duplikat username
    $stmt = $conn->prepare("SELECT id FROM users WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $result = $stmt->get_result();
    if ($result->num_rows > 0) {
        jsonResponse(false, 'Username sudah digunakan!');
    }
    $stmt->close();

    // Insert user baru
    $stmt = $conn->prepare("INSERT INTO users (username, password, role, nama) VALUES (?, ?, ?, ?)");
    $hash = md5($password);
    $stmt->bind_param("ssss", $username, $hash, $role, $nama);
    if ($stmt->execute()) {
        jsonResponse(true, 'Akun berhasil dibuat! Silakan login.');
    } else {
        jsonResponse(false, 'Gagal membuat akun!');
    }
    $stmt->close();
}
?>
