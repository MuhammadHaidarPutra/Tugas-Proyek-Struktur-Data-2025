<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Handle Actions
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'];
    
    if ($action === 'approve') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("UPDATE cuti SET status='disetujui', approved_by=?, approved_at=NOW() WHERE id=?");
        $stmt->bind_param("ii", $_SESSION['user_id'], $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Approve Cuti', "ID Cuti: $id");
            jsonResponse(true, 'Cuti berhasil disetujui!');
        } else {
            jsonResponse(false, 'Gagal menyetujui cuti!');
        }
    }
    
    if ($action === 'reject') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("UPDATE cuti SET status='ditolak', approved_by=?, approved_at=NOW() WHERE id=?");
        $stmt->bind_param("ii", $_SESSION['user_id'], $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Reject Cuti', "ID Cuti: $id");
            jsonResponse(true, 'Cuti ditolak!');
        } else {
            jsonResponse(false, 'Gagal menolak cuti!');
        }
    }
}

$cutiList = $conn->query("
    SELECT c.*, k.nama, k.jabatan 
    FROM cuti c 
    JOIN karyawan k ON c.id_karyawan = k.id 
    ORDER BY c.created_at DESC
");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Manajemen Cuti - Admin</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
    <div class="dashboard">
        <?php include 'sidebar.php'; ?>
        
        <main class="main-content">
            <?php include 'topbar.php'; ?>
            
            <div class="content">
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-umbrella-beach"></i> Manajemen Cuti</h3>
                    </div>
                    <div class="card-body">
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Tanggal Mulai</th>
                                        <th>Tanggal Selesai</th>
                                        <th>Durasi</th>
                                        <th>Jenis</th>
                                        <th>Alasan</th>
                                        <th>Status</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php while ($row = $cutiList->fetch_assoc()): 
                                        $durasi = (strtotime($row['tanggal_selesai']) - strtotime($row['tanggal_mulai'])) / 86400 + 1;
                                    ?>
                                        <tr>
                                            <td><?php echo $row['nama']; ?></td>
                                            <td><?php echo $row['jabatan']; ?></td>
                                            <td><?php echo formatTanggalIndo($row['tanggal_mulai']); ?></td>
                                            <td><?php echo formatTanggalIndo($row['tanggal_selesai']); ?></td>
                                            <td><?php echo $durasi; ?> hari</td>
                                            <td><span class="badge badge-info"><?php echo ucfirst($row['jenis_cuti']); ?></span></td>
                                            <td><?php echo $row['alasan'] ?: '-'; ?></td>
                                            <td>
                                                <?php 
                                                $badges = ['pending'=>'badge-warning','disetujui'=>'badge-success','ditolak'=>'badge-danger'];
                                                ?>
                                                <span class="badge <?php echo $badges[$row['status']]; ?>">
                                                    <?php echo ucfirst($row['status']); ?>
                                                </span>
                                            </td>
                                            <td>
                                                <?php if ($row['status'] === 'pending'): ?>
                                                    <div class="action-buttons">
                                                        <button class="btn btn-sm btn-success btn-icon" 
                                                                onclick="approveCuti(<?php echo $row['id']; ?>)" 
                                                                title="Setujui">
                                                            <i class="fas fa-check"></i>
                                                        </button>
                                                        <button class="btn btn-sm btn-danger btn-icon" 
                                                                onclick="rejectCuti(<?php echo $row['id']; ?>)" 
                                                                title="Tolak">
                                                            <i class="fas fa-times"></i>
                                                        </button>
                                                    </div>
                                                <?php else: ?>
                                                    <span class="badge badge-secondary">Sudah diproses</span>
                                                <?php endif; ?>
                                            </td>
                                        </tr>
                                    <?php endwhile; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </main>
    </div>
    
    <script>
        async function approveCuti(id) {
            if (!confirm('Setujui pengajuan cuti ini?')) return;
            
            const formData = new FormData();
            formData.append('action', 'approve');
            formData.append('id', id);
            
            try {
                const response = await fetch('cuti.php', {
                    method: 'POST',
                    body: formData
                });
                
                const data = await response.json();
                
                if (data.success) {
                    showAlert('success', data.message);
                    setTimeout(() => location.reload(), 1500);
                } else {
                    showAlert('error', data.message);
                }
            } catch (error) {
                showAlert('error', 'Terjadi kesalahan sistem!');
            }
        }
        
        async function rejectCuti(id) {
            if (!confirm('Tolak pengajuan cuti ini?')) return;
            
            const formData = new FormData();
            formData.append('action', 'reject');
            formData.append('id', id);
            
            try {
                const response = await fetch('cuti.php', {
                    method: 'POST',
                    body: formData
                });
                
                const data = await response.json();
                
                if (data.success) {
                    showAlert('success', data.message);
                    setTimeout(() => location.reload(), 1500);
                } else {
                    showAlert('error', data.message);
                }
            } catch (error) {
                showAlert('error', 'Terjadi kesalahan sistem!');
            }
        }
        
        function showAlert(type, message) {
            const alertContainer = document.getElementById('alert-container');
            const alertClass = type === 'success' ? 'alert-success' : 'alert-error';
            const icon = type === 'success' ? 'fa-check-circle' : 'fa-exclamation-circle';
            
            alertContainer.innerHTML = `
                <div class="alert ${alertClass}">
                    <i class="fas ${icon}"></i> ${message}
                </div>
            `;
            
            setTimeout(() => alertContainer.innerHTML = '', 5000);
        }
    </script>
</body>
</html>
