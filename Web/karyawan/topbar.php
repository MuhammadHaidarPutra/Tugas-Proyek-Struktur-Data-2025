<?php
// Get data karyawan
$dataKaryawan = $conn->query("SELECT * FROM karyawan WHERE id = {$_SESSION['id_karyawan']}")->fetch_assoc();
?>
<div class="topbar">
    <div class="topbar-left">
        <h1><?php 
            $page = basename($_SERVER['PHP_SELF'], '.php');
            $titles = [
                'dashboard' => 'Dashboard',
                'profil' => 'Profil Saya',
                'absensi' => 'Absensi Saya',
                'cuti' => 'Pengajuan Cuti',
                'jadwal' => 'Jadwal Saya',
                'payroll' => 'Slip Gaji',
                'evaluasi' => 'Evaluasi Saya'
            ];
            echo $titles[$page] ?? 'Karyawan Panel';
        ?></h1>
    </div>
    <div class="topbar-right">
        <div class="user-info">
            <div class="user-avatar"><?php echo strtoupper(substr($_SESSION['nama'], 0, 1)); ?></div>
            <div class="user-details">
                <strong><?php echo $_SESSION['nama']; ?></strong>
                <small><?php echo $dataKaryawan['jabatan'] ?? 'Karyawan'; ?></small>
            </div>
        </div>
    </div>
</div>
