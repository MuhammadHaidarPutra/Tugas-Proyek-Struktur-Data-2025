<?php
require_once '../config.php';

// Cek login dan role
if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Ambil statistik
$totalKaryawan = $conn->query("SELECT COUNT(*) as total FROM karyawan WHERE status='aktif'")->fetch_assoc()['total'];
$totalAbsenHariIni = $conn->query("SELECT COUNT(*) as total FROM absensi WHERE tanggal = CURDATE()")->fetch_assoc()['total'];
$cutiPending = $conn->query("SELECT COUNT(*) as total FROM cuti WHERE status='pending'")->fetch_assoc()['total'];
$totalDepartemen = $conn->query("SELECT COUNT(DISTINCT departemen) as total FROM karyawan")->fetch_assoc()['total'];

// Absensi hari ini
$absensiHariIni = $conn->query("
    SELECT a.*, k.nama, k.jabatan 
    FROM absensi a 
    JOIN karyawan k ON a.id_karyawan = k.id 
    WHERE a.tanggal = CURDATE() 
    ORDER BY a.jam_masuk DESC 
    LIMIT 10
");

// Cuti pending
$cutiPendingList = $conn->query("
    SELECT c.*, k.nama 
    FROM cuti c 
    JOIN karyawan k ON c.id_karyawan = k.id 
    WHERE c.status = 'pending' 
    ORDER BY c.created_at DESC 
    LIMIT 5
");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Admin - Sistem Manajemen Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
    <div class="dashboard">
        <!-- Sidebar -->
        <aside class="sidebar">
            <div class="sidebar-header">
                <h2><i class="fas fa-users-cog"></i> Admin Panel</h2>
                <p><?php echo $_SESSION['nama']; ?></p>
            </div>
            <ul class="sidebar-menu">
                <li><a href="dashboard.php" class="active"><i class="fas fa-home"></i> Dashboard</a></li>
                <li><a href="karyawan.php"><i class="fas fa-users"></i> Data Karyawan</a></li>
                <li><a href="absensi.php"><i class="fas fa-calendar-check"></i> Absensi</a></li>
                <li><a href="cuti.php"><i class="fas fa-umbrella-beach"></i> Manajemen Cuti</a></li>
                <li><a href="jadwal.php"><i class="fas fa-clock"></i> Jadwal Kerja</a></li>
                <li><a href="payroll.php"><i class="fas fa-money-bill-wave"></i> Payroll</a></li>
                <li><a href="evaluasi.php"><i class="fas fa-chart-line"></i> Evaluasi</a></li>
                <li><a href="laporan.php"><i class="fas fa-file-alt"></i> Laporan</a></li>
                <li><a href="users.php"><i class="fas fa-user-shield"></i> Manajemen User</a></li>
                <li><a href="../logout.php"><i class="fas fa-sign-out-alt"></i> Logout</a></li>
            </ul>
        </aside>
        
        <!-- Main Content -->
        <main class="main-content">
            <div class="topbar">
                <div class="topbar-left">
                    <h1>Dashboard</h1>
                </div>
                <div class="topbar-right">
                    <div class="user-info">
                        <div class="user-avatar"><?php echo strtoupper(substr($_SESSION['nama'], 0, 1)); ?></div>
                        <div class="user-details">
                            <strong><?php echo $_SESSION['nama']; ?></strong>
                            <small>Administrator</small>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="content">
                <!-- Stats -->
                <div class="stats-grid">
                    <div class="stat-card">
                        <div class="stat-icon blue">
                            <i class="fas fa-users"></i>
                        </div>
                        <div class="stat-info">
                            <h3><?php echo $totalKaryawan; ?></h3>
                            <p>Total Karyawan</p>
                        </div>
                    </div>
                    
                    <div class="stat-card">
                        <div class="stat-icon green">
                            <i class="fas fa-user-check"></i>
                        </div>
                        <div class="stat-info">
                            <h3><?php echo $totalAbsenHariIni; ?></h3>
                            <p>Hadir Hari Ini</p>
                        </div>
                    </div>
                    
                    <div class="stat-card">
                        <div class="stat-icon orange">
                            <i class="fas fa-clock"></i>
                        </div>
                        <div class="stat-info">
                            <h3><?php echo $cutiPending; ?></h3>
                            <p>Cuti Menunggu</p>
                        </div>
                    </div>
                    
                    <div class="stat-card">
                        <div class="stat-icon red">
                            <i class="fas fa-building"></i>
                        </div>
                        <div class="stat-info">
                            <h3><?php echo $totalDepartemen; ?></h3>
                            <p>Departemen</p>
                        </div>
                    </div>
                </div>
                
                <!-- Absensi Hari Ini -->
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-calendar-check"></i> Absensi Hari Ini</h3>
                        <a href="absensi.php" class="btn btn-sm btn-primary">Lihat Semua</a>
                    </div>
                    <div class="card-body">
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Jam Masuk</th>
                                        <th>Jam Keluar</th>
                                        <th>Status</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if ($absensiHariIni->num_rows > 0): ?>
                                        <?php while ($row = $absensiHariIni->fetch_assoc()): ?>
                                            <tr>
                                                <td><?php echo $row['nama']; ?></td>
                                                <td><?php echo $row['jabatan']; ?></td>
                                                <td><?php echo $row['jam_masuk'] ?: '-'; ?></td>
                                                <td><?php echo $row['jam_keluar'] ?: '-'; ?></td>
                                                <td>
                                                    <?php 
                                                    $badgeClass = [
                                                        'hadir' => 'badge-success',
                                                        'izin' => 'badge-info',
                                                        'sakit' => 'badge-warning',
                                                        'alfa' => 'badge-danger',
                                                        'cuti' => 'badge-secondary'
                                                    ];
                                                    ?>
                                                    <span class="badge <?php echo $badgeClass[$row['status']]; ?>">
                                                        <?php echo ucfirst($row['status']); ?>
                                                    </span>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    <?php else: ?>
                                        <tr>
                                            <td colspan="5" class="text-center">Belum ada data absensi hari ini</td>
                                        </tr>
                                    <?php endif; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
                
                <!-- Pengajuan Cuti Pending -->
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-hourglass-half"></i> Pengajuan Cuti Menunggu Persetujuan</h3>
                        <a href="cuti.php" class="btn btn-sm btn-primary">Lihat Semua</a>
                    </div>
                    <div class="card-body">
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama Karyawan</th>
                                        <th>Tanggal Mulai</th>
                                        <th>Tanggal Selesai</th>
                                        <th>Jenis Cuti</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if ($cutiPendingList->num_rows > 0): ?>
                                        <?php while ($row = $cutiPendingList->fetch_assoc()): ?>
                                            <tr>
                                                <td><?php echo $row['nama']; ?></td>
                                                <td><?php echo formatTanggalIndo($row['tanggal_mulai']); ?></td>
                                                <td><?php echo formatTanggalIndo($row['tanggal_selesai']); ?></td>
                                                <td><span class="badge badge-info"><?php echo ucfirst($row['jenis_cuti']); ?></span></td>
                                                <td>
                                                    <a href="cuti.php?id=<?php echo $row['id']; ?>" class="btn btn-sm btn-primary">Review</a>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    <?php else: ?>
                                        <tr>
                                            <td colspan="5" class="text-center">Tidak ada pengajuan cuti pending</td>
                                        </tr>
                                    <?php endif; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </main>
    </div>
</body>
</html>
