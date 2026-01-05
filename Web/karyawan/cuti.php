<?php
require_once '../config.php';

if (!isLoggedIn() || isAdmin()) {
    redirect('../index.php');
}

$idKaryawan = $_SESSION['id_karyawan'];

// Handle pengajuan cuti
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $tanggal_mulai = $_POST['tanggal_mulai'];
    $tanggal_selesai = $_POST['tanggal_selesai'];
    $jenis_cuti = $_POST['jenis_cuti'];
    $alasan = sanitize($_POST['alasan']);
    
    $stmt = $conn->prepare("INSERT INTO cuti (id_karyawan, tanggal_mulai, tanggal_selesai, jenis_cuti, alasan) VALUES (?, ?, ?, ?, ?)");
    $stmt->bind_param("issss", $idKaryawan, $tanggal_mulai, $tanggal_selesai, $jenis_cuti, $alasan);
    
    if ($stmt->execute()) {
        $success = "Pengajuan cuti berhasil dikirim!";
    } else {
        $error = "Gagal mengajukan cuti!";
    }
}

// Get riwayat cuti
$cutiList = $conn->query("SELECT * FROM cuti WHERE id_karyawan = $idKaryawan ORDER BY created_at DESC");

// Hitung cuti tersisa
$cutiDigunakan = $conn->query("
    SELECT COALESCE(SUM(DATEDIFF(tanggal_selesai, tanggal_mulai) + 1), 0) as total 
    FROM cuti 
    WHERE id_karyawan = $idKaryawan 
    AND YEAR(tanggal_mulai) = YEAR(CURDATE())
    AND status = 'disetujui'
")->fetch_assoc()['total'];
$cutiTersisa = 12 - $cutiDigunakan;
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pengajuan Cuti - Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
    <div class="dashboard">
        <?php include 'sidebar.php'; ?>
        
        <main class="main-content">
            <?php include 'topbar.php'; ?>
            
            <div class="content">
                <?php if (isset($success)): ?>
                    <div class="alert alert-success">
                        <i class="fas fa-check-circle"></i> <?php echo $success; ?>
                    </div>
                <?php endif; ?>
                
                <?php if (isset($error)): ?>
                    <div class="alert alert-error">
                        <i class="fas fa-exclamation-circle"></i> <?php echo $error; ?>
                    </div>
                <?php endif; ?>
                
                <div class="alert alert-info">
                    <i class="fas fa-info-circle"></i>
                    Sisa cuti tahun ini: <strong><?php echo $cutiTersisa; ?> hari</strong>
                </div>
                
                <!-- Form Pengajuan -->
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-umbrella-beach"></i> Ajukan Cuti Baru</h3>
                    </div>
                    <div class="card-body">
                        <form method="POST">
                            <div class="form-group">
                                <label for="tanggal_mulai">Tanggal Mulai <span style="color: red;">*</span></label>
                                <input type="date" id="tanggal_mulai" name="tanggal_mulai" required>
                            </div>
                            
                            <div class="form-group">
                                <label for="tanggal_selesai">Tanggal Selesai <span style="color: red;">*</span></label>
                                <input type="date" id="tanggal_selesai" name="tanggal_selesai" required>
                            </div>
                            
                            <div class="form-group">
                                <label for="jenis_cuti">Jenis Cuti <span style="color: red;">*</span></label>
                                <select id="jenis_cuti" name="jenis_cuti" required>
                                    <option value="tahunan">Cuti Tahunan</option>
                                    <option value="sakit">Cuti Sakit</option>
                                    <option value="penting">Cuti Penting</option>
                                    <option value="lainnya">Lainnya</option>
                                </select>
                            </div>
                            
                            <div class="form-group">
                                <label for="alasan">Alasan <span style="color: red;">*</span></label>
                                <textarea id="alasan" name="alasan" required></textarea>
                            </div>
                            
                            <button type="submit" class="btn btn-primary">
                                <i class="fas fa-paper-plane"></i> Ajukan Cuti
                            </button>
                        </form>
                    </div>
                </div>
                
                <!-- Riwayat Cuti -->
                <div class="card mt-20">
                    <div class="card-header">
                        <h3><i class="fas fa-history"></i> Riwayat Pengajuan Cuti</h3>
                    </div>
                    <div class="card-body">
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Tanggal Mulai</th>
                                        <th>Tanggal Selesai</th>
                                        <th>Durasi</th>
                                        <th>Jenis</th>
                                        <th>Alasan</th>
                                        <th>Status</th>
                                        <th>Diajukan</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if ($cutiList->num_rows > 0): ?>
                                        <?php while ($row = $cutiList->fetch_assoc()): 
                                            $durasi = (strtotime($row['tanggal_selesai']) - strtotime($row['tanggal_mulai'])) / 86400 + 1;
                                        ?>
                                            <tr>
                                                <td><?php echo formatTanggalIndo($row['tanggal_mulai']); ?></td>
                                                <td><?php echo formatTanggalIndo($row['tanggal_selesai']); ?></td>
                                                <td><?php echo $durasi; ?> hari</td>
                                                <td><span class="badge badge-info"><?php echo ucfirst($row['jenis_cuti']); ?></span></td>
                                                <td><?php echo $row['alasan']; ?></td>
                                                <td>
                                                    <?php 
                                                    $badges = ['pending'=>'badge-warning','disetujui'=>'badge-success','ditolak'=>'badge-danger'];
                                                    ?>
                                                    <span class="badge <?php echo $badges[$row['status']]; ?>">
                                                        <?php echo ucfirst($row['status']); ?>
                                                    </span>
                                                </td>
                                                <td><?php echo date('d/m/Y', strtotime($row['created_at'])); ?></td>
                                            </tr>
                                        <?php endwhile; ?>
                                    <?php else: ?>
                                        <tr>
                                            <td colspan="7" class="text-center">Belum ada riwayat cuti</td>
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
