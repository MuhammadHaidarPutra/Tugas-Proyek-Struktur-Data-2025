<?php
require_once 'config.php';

// Cek jika sudah login, redirect ke dashboard
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
    $stmt = $conn->prepare("SELECT id, username, password, role, nama, id_karyawan FROM users WHERE username = ?");
    $stmt->bind_param("s", $username);
    $stmt->execute();
    $stmt->store_result();
    if ($stmt->num_rows === 1) {
        $stmt->bind_result($id, $username_db, $password_db, $role, $nama, $id_karyawan);
        $stmt->fetch();
        if (md5($password) === $password_db) {
            $_SESSION['user_id'] = $id;
            $_SESSION['username'] = $username_db;
            $_SESSION['role'] = $role;
            $_SESSION['nama'] = $nama;
            $_SESSION['id_karyawan'] = $id_karyawan;
            logActivity($id, 'Login', 'User berhasil login');
            if ($role === 'admin') {
                jsonResponse(true, 'Login berhasil! Selamat datang Admin.', [
                    'role' => 'admin',
                    'redirect' => 'admin/dashboard.php'
                ]);
            } else if ($role === 'karyawan') {
                jsonResponse(true, 'Login berhasil! Selamat datang Karyawan.', [
                    'role' => 'karyawan',
                    'redirect' => 'karyawan/dashboard.php'
                ]);
            } else {
                jsonResponse(false, 'Role user tidak valid!');
            }
        } else {
            jsonResponse(false, 'Login gagal! Username atau password salah.');
        }
    } else {
        jsonResponse(false, 'Login gagal! Username atau password salah.');
    }
    $stmt->close();
}
?>
