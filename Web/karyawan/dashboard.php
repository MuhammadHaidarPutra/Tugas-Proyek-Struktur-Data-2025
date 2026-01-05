<?php
require_once '../config.php';

// Cek login dan role karyawan
if (!isLoggedIn() || isAdmin()) {
    redirect('../index.php');
}

$idKaryawan = $_SESSION['id_karyawan'];

// Ambil data karyawan
$stmt = $conn->prepare("SELECT * FROM karyawan WHERE id = ?");
$stmt->bind_param("i", $idKaryawan);
$stmt->execute();
$dataKaryawan = $stmt->get_result()->fetch_assoc();

// Absensi bulan ini
$totalHadirBulanIni = $conn->query("
    SELECT COUNT(*) as total 
    FROM absensi 
    WHERE id_karyawan = $idKaryawan 
    AND MONTH(tanggal) = MONTH(CURDATE()) 
    AND YEAR(tanggal) = YEAR(CURDATE())
    AND status = 'hadir'
")->fetch_assoc()['total'];

// Cuti tersisa (misalnya 12 hari per tahun)
$cutiDigunakan = $conn->query("
    SELECT COALESCE(SUM(DATEDIFF(tanggal_selesai, tanggal_mulai) + 1), 0) as total 
    FROM cuti 
    WHERE id_karyawan = $idKaryawan 
    AND YEAR(tanggal_mulai) = YEAR(CURDATE())
    AND status = 'disetujui'
")->fetch_assoc()['total'];
$cutiTersisa = 12 - $cutiDigunakan;

// Jadwal hari ini
$jadwalHariIni = $conn->query("
    SELECT * FROM jadwal 
    WHERE id_karyawan = $idKaryawan 
    AND tanggal = CURDATE()
")->fetch_assoc();

// Riwayat absensi terbaru
$riwayatAbsensi = $conn->query("
    SELECT * FROM absensi 
    WHERE id_karyawan = $idKaryawan 
    ORDER BY tanggal DESC 
    LIMIT 10
");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Karyawan - Sistem Manajemen Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
    <div class="dashboard">
        <?php include 'sidebar.php'; ?>
        <div class="sidebar-backdrop" onclick="document.querySelector('.sidebar').classList.remove('show');this.style.display='none';"></div>
        <!-- Main Content -->
        <main class="main-content">
            <div class="topbar">
                <div class="topbar-left">
                    <button class="btn btn-icon" style="display:none;margin-right:10px;" id="sidebarToggle" onclick="toggleSidebar()"><i class="fas fa-bars"></i></button>
                    <h1>Dashboard</h1>
                </div>
                <div class="topbar-right">
                    <div class="user-info">
                        <div class="user-avatar"><?php echo strtoupper(substr($_SESSION['nama'], 0, 1)); ?></div>
                        <div class="user-details">
                            <strong><?php echo $_SESSION['nama']; ?></strong>
                            <small><?php echo $dataKaryawan['jabatan']; ?></small>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="content">
                <!-- Welcome -->
                <div class="alert alert-info">
                    <i class="fas fa-info-circle"></i>
                    Selamat datang, <strong><?php echo $_SESSION['nama']; ?></strong>! 
                    Hari ini adalah <?php echo formatTanggalIndo(date('Y-m-d')); ?>
                </div>

                <!-- Stats: Only 3 icons, no clock-in/attendance -->
                <div class="stats-grid stats-3icons">
                    <div class="stat-card">
                        <div class="stat-card-inner">
                            <div class="modern-stat-icon blue">
                                <i class="fas fa-calendar-check"></i>
                            </div>
                            <div class="stat-label">Absensi Bulan Ini</div>
                            <div class="stat-value"><?php echo $totalHadirBulanIni; ?></div>
                            <div class="stat-unit">hari</div>
                        </div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-card-inner">
                            <div class="modern-stat-icon green">
                                <i class="fas fa-umbrella-beach"></i>
                            </div>
                            <div class="stat-label">Cuti Tersisa</div>
                            <div class="stat-value"><?php echo $cutiTersisa; ?></div>
                            <div class="stat-unit">hari</div>
                        </div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-card-inner">
                            <div class="modern-stat-icon orange">
                                <i class="fas fa-calendar-day"></i>
                            </div>
                            <div class="stat-label">Jadwal Hari Ini</div>
                            <div class="stat-value" style="min-height:32px;">
                                <?php echo $jadwalHariIni ? 'Ada Jadwal' : 'Tidak ada jadwal hari ini.'; ?>
                            </div>
                        </div>
                    </div>
                </div>
                <!-- End Stats -->
            </div>
        </main>
    </div>
<style>
.stats-grid.stats-3icons {
    display: flex;
    gap: 32px;
    justify-content: center;
    align-items: stretch;
    margin: 32px 0 0 0;
}
.stat-card {
    background: linear-gradient(135deg, #f8fafc 60%, #e0e7ef 100%);
    border-radius: 22px;
    box-shadow: 0 6px 32px rgba(37,99,235,0.10), 0 1.5px 6px rgba(0,0,0,0.04);
    padding: 36px 38px 28px 38px;
    min-width: 250px;
    text-align: center;
    transition: box-shadow 0.2s, transform 0.2s;
    border: 1.5px solid #e5e7eb;
    margin-bottom: 8px;
    position: relative;
    overflow: hidden;
    z-index: 1;
    display: flex;
    flex-direction: column;
    justify-content: center;
}
.stat-card:hover {
    box-shadow: 0 8px 36px rgba(37,99,235,0.18), 0 2px 8px rgba(0,0,0,0.06);
    transform: translateY(-4px) scale(1.03);
}
.modern-stat-icon {
    display: flex;
    align-items: center;
    justify-content: center;
    margin: 0 auto 22px auto;
    width: 78px;
    height: 78px;
    border-radius: 50%;
    background: linear-gradient(135deg, #e0e7ff 0%, #f1f5f9 100%);
    box-shadow: 0 2px 16px rgba(37,99,235,0.10);
    font-size: 2.7rem;
    transition: background 0.3s, box-shadow 0.3s;
    border: 2.5px solid #fff;
}
.modern-stat-icon.blue {
    background: linear-gradient(135deg, #2563eb 0%, #60a5fa 100%);
    color: #fff;
}
.modern-stat-icon.green {
    background: linear-gradient(135deg, #10b981 0%, #6ee7b7 100%);
    color: #fff;
}
.modern-stat-icon.orange {
    background: linear-gradient(135deg, #f59e0b 0%, #fbbf24 100%);
    color: #fff;
}
@media (max-width: 900px) {
    .stats-grid.stats-3icons {
        flex-direction: column;
        gap: 18px;
        align-items: center;
    }
    .stat-card {
        min-width: 0;
        width: 100%;
    }
}
.stat-card-inner {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    height: 100%;
    gap: 0.2em;
}
.stat-label {
    font-size: 1.08rem;
    font-weight: 700; /* lebih tebal */
    color: #22223b;
    margin-bottom: 0.1em;
    margin-top: 0.1em;
    letter-spacing: 0.2px;
    text-align: center;
    line-height: 1.2;
}
.stat-value {
    font-size: 1.35rem;
    font-weight: 700;
    margin-top: 0.15em;
    margin-bottom: 0.05em;
    color: #1e293b;
    letter-spacing: 0.5px;
    line-height: 1.2;
    text-shadow: 0 1px 0 #fff, 0 2px 8px rgba(37,99,235,0.07);
    text-align: center;
}
.stat-unit {
    font-size: 1rem;
    font-weight: 400; /* lebih tipis */
    color: #475569;
    margin-top: 0.05em;
    margin-left: 0;
    text-align: center;
    line-height: 1.2;
}
</style>
<script>
function toggleSidebar() {
    var sidebar = document.querySelector('.sidebar');
    var backdrop = document.querySelector('.sidebar-backdrop');
    sidebar.classList.toggle('show');
    if (sidebar.classList.contains('show')) {
        backdrop.style.display = 'block';
    } else {
        backdrop.style.display = 'none';
    }
}
// Show sidebar toggle button on mobile
window.addEventListener('DOMContentLoaded', function() {
    if(window.innerWidth <= 768) {
        document.getElementById('sidebarToggle').style.display = 'inline-block';
    }
});
window.addEventListener('resize', function() {
    if(window.innerWidth <= 768) {
        document.getElementById('sidebarToggle').style.display = 'inline-block';
    } else {
        document.getElementById('sidebarToggle').style.display = 'none';
        document.querySelector('.sidebar').classList.remove('show');
        document.querySelector('.sidebar-backdrop').style.display = 'none';
    }
});
</script>
</body>
</html>
