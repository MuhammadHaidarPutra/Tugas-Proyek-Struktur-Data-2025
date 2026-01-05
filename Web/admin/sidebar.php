<!-- Sidebar -->
<aside class="sidebar">
    <div class="sidebar-header">
        <h2><i class="fas fa-users-cog"></i> Admin Panel</h2>
        <p><?php echo $_SESSION['nama']; ?></p>
    </div>
    <ul class="sidebar-menu">
        <li><a href="dashboard.php" <?php echo basename($_SERVER['PHP_SELF']) === 'dashboard.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-home"></i> Dashboard</a></li>
        <li><a href="karyawan.php" <?php echo basename($_SERVER['PHP_SELF']) === 'karyawan.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-users"></i> Data Karyawan</a></li>
        <li><a href="absensi.php" <?php echo basename($_SERVER['PHP_SELF']) === 'absensi.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-calendar-check"></i> Absensi</a></li>
        <li><a href="cuti.php" <?php echo basename($_SERVER['PHP_SELF']) === 'cuti.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-umbrella-beach"></i> Manajemen Cuti</a></li>
        <li><a href="jadwal.php" <?php echo basename($_SERVER['PHP_SELF']) === 'jadwal.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-clock"></i> Jadwal Kerja</a></li>
        <li><a href="payroll.php" <?php echo basename($_SERVER['PHP_SELF']) === 'payroll.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-money-bill-wave"></i> Payroll</a></li>
        <li><a href="evaluasi.php" <?php echo basename($_SERVER['PHP_SELF']) === 'evaluasi.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-chart-line"></i> Evaluasi</a></li>
        <li><a href="laporan.php" <?php echo basename($_SERVER['PHP_SELF']) === 'laporan.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-file-alt"></i> Laporan</a></li>
        <li><a href="users.php" <?php echo basename($_SERVER['PHP_SELF']) === 'users.php' ? 'class="active"' : ''; ?>>
            <i class="fas fa-user-shield"></i> Manajemen User</a></li>
        <li><a href="../logout.php"><i class="fas fa-sign-out-alt"></i> Logout</a></li>
    </ul>
</aside>
