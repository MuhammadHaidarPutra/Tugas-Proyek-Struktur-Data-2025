<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Get statistics
$totalKaryawan = $conn->query("SELECT COUNT(*) as total FROM karyawan WHERE status='aktif'")->fetch_assoc()['total'];

// Absensi statistics bulan ini
$bulanIni = date('m');
$tahunIni = date('Y');

$absensiStats = $conn->query("
    SELECT 
        status,
        COUNT(*) as total
    FROM absensi 
    WHERE MONTH(tanggal) = $bulanIni AND YEAR(tanggal) = $tahunIni
    GROUP BY status
");

$statsAbsensi = [];
while ($row = $absensiStats->fetch_assoc()) {
    $statsAbsensi[$row['status']] = $row['total'];
}

// Departemen statistics
$deptStats = $conn->query("
    SELECT departemen, COUNT(*) as total 
    FROM karyawan 
    WHERE status='aktif' 
    GROUP BY departemen
");

// Top 10 kehadiran
$topKehadiran = $conn->query("
    SELECT k.nama, k.jabatan, COUNT(*) as total_hadir
    FROM absensi a
    JOIN karyawan k ON a.id_karyawan = k.id
    WHERE MONTH(a.tanggal) = $bulanIni 
    AND YEAR(a.tanggal) = $tahunIni 
    AND a.status = 'hadir'
    GROUP BY a.id_karyawan
    ORDER BY total_hadir DESC
    LIMIT 10
");

// Payroll statistics
$totalPayroll = $conn->query("
    SELECT SUM(total_gaji) as total 
    FROM payroll 
    WHERE bulan = $bulanIni AND tahun = $tahunIni
")->fetch_assoc()['total'];

// Evaluasi average
$avgEvaluasi = $conn->query("
    SELECT 
        AVG(nilai_kinerja) as avg_kinerja,
        AVG(nilai_kedisiplinan) as avg_kedisiplinan,
        AVG(nilai_kerjasama) as avg_kerjasama
    FROM evaluasi
    WHERE YEAR(tanggal_evaluasi) = $tahunIni
")->fetch_assoc();
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Laporan - Admin</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <div class="dashboard">
        <?php include 'sidebar.php'; ?>
        
        <main class="main-content">
            <?php include 'topbar.php'; ?>
            
            <div class="content">
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-file-alt"></i> Laporan & Statistik</h3>
                        <button class="btn btn-primary" onclick="window.print()">
                            <i class="fas fa-print"></i> Print Laporan
                        </button>
                    </div>
                    <div class="card-body">
                        <h4>Periode: <?php echo date('F Y'); ?></h4>
                        
                        <!-- Statistik Absensi -->
                        <div class="mt-20">
                            <h5><i class="fas fa-calendar-check"></i> Statistik Absensi Bulan Ini</h5>
                            <div class="stats-grid">
                                <div class="stat-card">
                                    <div class="stat-icon green">
                                        <i class="fas fa-user-check"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo $statsAbsensi['hadir'] ?? 0; ?></h3>
                                        <p>Hadir</p>
                                    </div>
                                </div>
                                
                                <div class="stat-card">
                                    <div class="stat-icon blue">
                                        <i class="fas fa-info-circle"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo $statsAbsensi['izin'] ?? 0; ?></h3>
                                        <p>Izin</p>
                                    </div>
                                </div>
                                
                                <div class="stat-card">
                                    <div class="stat-icon orange">
                                        <i class="fas fa-hospital"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo $statsAbsensi['sakit'] ?? 0; ?></h3>
                                        <p>Sakit</p>
                                    </div>
                                </div>
                                
                                <div class="stat-card">
                                    <div class="stat-icon red">
                                        <i class="fas fa-times-circle"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo $statsAbsensi['alfa'] ?? 0; ?></h3>
                                        <p>Alfa</p>
                                    </div>
                                </div>
                            </div>
                        </div>
                        
                        <!-- Departemen -->
                        <div class="mt-20">
                            <h5><i class="fas fa-building"></i> Statistik per Departemen</h5>
                            <div class="table-responsive">
                                <table>
                                    <thead>
                                        <tr>
                                            <th>Departemen</th>
                                            <th>Jumlah Karyawan</th>
                                            <th>Persentase</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php 
                                        $deptStats->data_seek(0);
                                        while ($dept = $deptStats->fetch_assoc()): 
                                            $persen = ($dept['total'] / $totalKaryawan) * 100;
                                        ?>
                                            <tr>
                                                <td><strong><?php echo $dept['departemen']; ?></strong></td>
                                                <td><?php echo $dept['total']; ?> orang</td>
                                                <td>
                                                    <div style="display: flex; align-items: center; gap: 10px;">
                                                        <div style="flex: 1; background: #e5e7eb; height: 20px; border-radius: 10px; overflow: hidden;">
                                                            <div style="width: <?php echo $persen; ?>%; background: #2563eb; height: 100%;"></div>
                                                        </div>
                                                        <span><?php echo number_format($persen, 1); ?>%</span>
                                                    </div>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        
                        <!-- Top Kehadiran -->
                        <div class="mt-20">
                            <h5><i class="fas fa-trophy"></i> Top 10 Kehadiran Bulan Ini</h5>
                            <div class="table-responsive">
                                <table>
                                    <thead>
                                        <tr>
                                            <th>Ranking</th>
                                            <th>Nama</th>
                                            <th>Jabatan</th>
                                            <th>Total Hadir</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php 
                                        $rank = 1;
                                        while ($top = $topKehadiran->fetch_assoc()): 
                                        ?>
                                            <tr>
                                                <td>
                                                    <?php if ($rank <= 3): ?>
                                                        <span class="badge <?php echo $rank===1 ? 'badge-warning' : ($rank===2 ? 'badge-secondary' : 'badge-info'); ?>">
                                                            #<?php echo $rank; ?>
                                                        </span>
                                                    <?php else: ?>
                                                        #<?php echo $rank; ?>
                                                    <?php endif; ?>
                                                </td>
                                                <td><?php echo $top['nama']; ?></td>
                                                <td><?php echo $top['jabatan']; ?></td>
                                                <td><strong><?php echo $top['total_hadir']; ?> hari</strong></td>
                                            </tr>
                                        <?php 
                                            $rank++;
                                        endwhile; 
                                        ?>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        
                        <!-- Payroll -->
                        <div class="mt-20">
                            <h5><i class="fas fa-money-bill-wave"></i> Total Payroll Bulan Ini</h5>
                            <div class="alert alert-success">
                                <i class="fas fa-info-circle"></i>
                                Total pengeluaran payroll: <strong><?php echo formatRupiah($totalPayroll ?? 0); ?></strong>
                            </div>
                        </div>
                        
                        <!-- Evaluasi -->
                        <?php if ($avgEvaluasi['avg_kinerja']): ?>
                        <div class="mt-20">
                            <h5><i class="fas fa-chart-line"></i> Rata-rata Evaluasi Tahun Ini</h5>
                            <div class="stats-grid">
                                <div class="stat-card">
                                    <div class="stat-icon blue">
                                        <i class="fas fa-chart-bar"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo number_format($avgEvaluasi['avg_kinerja'], 1); ?></h3>
                                        <p>Kinerja</p>
                                    </div>
                                </div>
                                
                                <div class="stat-card">
                                    <div class="stat-icon green">
                                        <i class="fas fa-clock"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo number_format($avgEvaluasi['avg_kedisiplinan'], 1); ?></h3>
                                        <p>Kedisiplinan</p>
                                    </div>
                                </div>
                                
                                <div class="stat-card">
                                    <div class="stat-icon orange">
                                        <i class="fas fa-users"></i>
                                    </div>
                                    <div class="stat-info">
                                        <h3><?php echo number_format($avgEvaluasi['avg_kerjasama'], 1); ?></h3>
                                        <p>Kerjasama</p>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <?php endif; ?>
                    </div>
                </div>
            </div>
        </main>
    </div>
</body>
</html>
