<!-- Sidebar Karyawan -->
<aside class="sidebar">
    <div class="sidebar-header">
        <h2><i class="fas fa-user"></i> Karyawan</h2>
        <p><?php echo $_SESSION['nama']; ?></p>
    </div>
    <ul class="sidebar-menu">
        <li><a href="dashboard.php" <?php echo basename($_SERVER['PHP_SELF']) === 'dashboard.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-home"></i> Dashboard</a></li>
        <li><a href="profil.php" <?php echo basename($_SERVER['PHP_SELF']) === 'profil.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-user-circle"></i> Profil Saya</a></li>
        <li><a href="absensi.php" <?php echo basename($_SERVER['PHP_SELF']) === 'absensi.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-calendar-check"></i> Absensi Saya</a></li>
        <li><a href="cuti.php" <?php echo basename($_SERVER['PHP_SELF']) === 'cuti.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-umbrella-beach"></i> Pengajuan Cuti</a></li>
        <li><a href="jadwal.php" <?php echo basename($_SERVER['PHP_SELF']) === 'jadwal.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-clock"></i> Jadwal Saya</a></li>
        <li><a href="payroll.php" <?php echo basename($_SERVER['PHP_SELF']) === 'payroll.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-money-bill-wave"></i> Slip Gaji</a></li>
        <li><a href="evaluasi.php" <?php echo basename($_SERVER['PHP_SELF']) === 'evaluasi.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-chart-line"></i> Evaluasi Saya</a></li>
        <li><a href="../logout.php"><i class="fas fa-sign-out-alt"></i> Logout</a></li>
    </ul>
</aside>
