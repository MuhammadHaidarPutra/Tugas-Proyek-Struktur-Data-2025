<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Handle CRUD
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'];
    
    if ($action === 'create') {
        $id_karyawan = $_POST['id_karyawan'];
        $tanggal = $_POST['tanggal'];
        $jam_masuk = $_POST['jam_masuk'];
        $jam_keluar = $_POST['jam_keluar'] ?: null;
        $status = $_POST['status'];
        $keterangan = sanitize($_POST['keterangan']);
        
        // Hitung durasi kerja
        $durasi_kerja = 0;
        if ($jam_masuk && $jam_keluar) {
            $masuk = strtotime($jam_masuk);
            $keluar = strtotime($jam_keluar);
            $durasi_kerja = ($keluar - $masuk) / 60; // dalam menit
        }
        
        $stmt = $conn->prepare("INSERT INTO absensi (id_karyawan, tanggal, jam_masuk, jam_keluar, durasi_kerja, status, keterangan) VALUES (?, ?, ?, ?, ?, ?, ?)");
        $stmt->bind_param("isssdss", $id_karyawan, $tanggal, $jam_masuk, $jam_keluar, $durasi_kerja, $status, $keterangan);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Tambah Absensi', "ID Karyawan: $id_karyawan, Tanggal: $tanggal");
            jsonResponse(true, 'Absensi berhasil ditambahkan!');
        } else {
            jsonResponse(false, 'Gagal menambahkan absensi!');
        }
    }
    
    if ($action === 'update') {
        $id = $_POST['id'];
        $jam_masuk = $_POST['jam_masuk'];
        $jam_keluar = $_POST['jam_keluar'] ?: null;
        $status = $_POST['status'];
        $keterangan = sanitize($_POST['keterangan']);
        
        $durasi_kerja = 0;
        if ($jam_masuk && $jam_keluar) {
            $masuk = strtotime($jam_masuk);
            $keluar = strtotime($jam_keluar);
            $durasi_kerja = ($keluar - $masuk) / 60;
        }
        
        $stmt = $conn->prepare("UPDATE absensi SET jam_masuk=?, jam_keluar=?, durasi_kerja=?, status=?, keterangan=? WHERE id=?");
        $stmt->bind_param("ssdssi", $jam_masuk, $jam_keluar, $durasi_kerja, $status, $keterangan, $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Update Absensi', "ID Absensi: $id");
            jsonResponse(true, 'Absensi berhasil diupdate!');
        } else {
            jsonResponse(false, 'Gagal mengupdate absensi!');
        }
    }
    
    if ($action === 'delete') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("DELETE FROM absensi WHERE id=?");
        $stmt->bind_param("i", $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Hapus Absensi', "ID Absensi: $id");
            jsonResponse(true, 'Absensi berhasil dihapus!');
        } else {
            jsonResponse(false, 'Gagal menghapus absensi!');
        }
    }
}

// Get data
$tanggal = isset($_GET['tanggal']) ? $_GET['tanggal'] : date('Y-m-d');
$absensiList = $conn->query("
    SELECT a.*, k.nama, k.jabatan, k.departemen 
    FROM absensi a 
    JOIN karyawan k ON a.id_karyawan = k.id 
    WHERE a.tanggal = '$tanggal'
    ORDER BY a.jam_masuk DESC
");

$karyawanList = $conn->query("SELECT id, nama FROM karyawan WHERE status='aktif' ORDER BY nama");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Absensi - Admin</title>
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
                        <h3><i class="fas fa-calendar-check"></i> Data Absensi</h3>
                        <button class="btn btn-primary" onclick="showAddModal()">
                            <i class="fas fa-plus"></i> Tambah Absensi
                        </button>
                    </div>
                    <div class="card-body">
                        <div class="mb-20 d-flex gap-10">
                            <input type="date" id="filterTanggal" value="<?php echo $tanggal; ?>" 
                                   onchange="filterByDate()" style="padding: 10px; border: 2px solid #e5e7eb; border-radius: 8px;">
                            <button class="btn btn-info" onclick="location.href='absensi.php?tanggal=<?php echo date('Y-m-d'); ?>'">
                                <i class="fas fa-calendar-day"></i> Hari Ini
                            </button>
                        </div>
                        
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Departemen</th>
                                        <th>Jam Masuk</th>
                                        <th>Jam Keluar</th>
                                        <th>Durasi (Menit)</th>
                                        <th>Status</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if ($absensiList->num_rows > 0): ?>
                                        <?php while ($row = $absensiList->fetch_assoc()): ?>
                                            <tr>
                                                <td><?php echo $row['nama']; ?></td>
                                                <td><?php echo $row['jabatan']; ?></td>
                                                <td><?php echo $row['departemen']; ?></td>
                                                <td><?php echo $row['jam_masuk'] ?: '-'; ?></td>
                                                <td><?php echo $row['jam_keluar'] ?: '-'; ?></td>
                                                <td><?php echo $row['durasi_kerja'] ?: 0; ?></td>
                                                <td>
                                                    <?php 
                                                    $badges = ['hadir'=>'badge-success','izin'=>'badge-info','sakit'=>'badge-warning','alfa'=>'badge-danger','cuti'=>'badge-secondary'];
                                                    ?>
                                                    <span class="badge <?php echo $badges[$row['status']]; ?>">
                                                        <?php echo ucfirst($row['status']); ?>
                                                    </span>
                                                </td>
                                                <td>
                                                    <div class="action-buttons">
                                                        <button class="btn btn-sm btn-info btn-icon" 
                                                                onclick='editAbsensi(<?php echo json_encode($row); ?>)'>
                                                            <i class="fas fa-edit"></i>
                                                        </button>
                                                        <button class="btn btn-sm btn-danger btn-icon" 
                                                                onclick="deleteAbsensi(<?php echo $row['id']; ?>)">
                                                            <i class="fas fa-trash"></i>
                                                        </button>
                                                    </div>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    <?php else: ?>
                                        <tr>
                                            <td colspan="8" class="text-center">Tidak ada data absensi</td>
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
    
    <!-- Modal -->
    <div id="formModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3 id="modalTitle">Tambah Absensi</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="absensiForm">
                    <input type="hidden" id="absensiId" name="id">
                    <input type="hidden" id="formAction" name="action" value="create">
                    
                    <div class="form-group" id="karyawanGroup">
                        <label for="id_karyawan">Karyawan <span style="color: red;">*</span></label>
                        <select id="id_karyawan" name="id_karyawan" required>
                            <option value="">Pilih Karyawan</option>
                            <?php 
                            $karyawanList->data_seek(0);
                            while ($k = $karyawanList->fetch_assoc()): 
                            ?>
                                <option value="<?php echo $k['id']; ?>"><?php echo $k['nama']; ?></option>
                            <?php endwhile; ?>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="tanggal">Tanggal <span style="color: red;">*</span></label>
                        <input type="date" id="tanggal" name="tanggal" value="<?php echo date('Y-m-d'); ?>" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="jam_masuk">Jam Masuk</label>
                        <input type="time" id="jam_masuk" name="jam_masuk">
                    </div>
                    
                    <div class="form-group">
                        <label for="jam_keluar">Jam Keluar</label>
                        <input type="time" id="jam_keluar" name="jam_keluar">
                    </div>
                    
                    <div class="form-group">
                        <label for="status">Status <span style="color: red;">*</span></label>
                        <select id="status" name="status" required>
                            <option value="hadir">Hadir</option>
                            <option value="izin">Izin</option>
                            <option value="sakit">Sakit</option>
                            <option value="alfa">Alfa</option>
                            <option value="cuti">Cuti</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="keterangan">Keterangan</label>
                        <textarea id="keterangan" name="keterangan"></textarea>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button class="btn btn-primary" onclick="submitForm()">Simpan</button>
            </div>
        </div>
    </div>
    
    <script src="../assets/js/absensi.js"></script>
</body>
</html>
