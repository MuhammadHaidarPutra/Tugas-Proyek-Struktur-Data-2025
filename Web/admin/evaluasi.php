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
        $periode = sanitize($_POST['periode']);
        $nilai_kinerja = $_POST['nilai_kinerja'];
        $nilai_kedisiplinan = $_POST['nilai_kedisiplinan'];
        $nilai_kerjasama = $_POST['nilai_kerjasama'];
        $catatan = sanitize($_POST['catatan']);
        $evaluator = $_SESSION['nama'];
        $tanggal_evaluasi = date('Y-m-d');
        
        $stmt = $conn->prepare("INSERT INTO evaluasi (id_karyawan, periode, nilai_kinerja, nilai_kedisiplinan, nilai_kerjasama, catatan, evaluator, tanggal_evaluasi) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        $stmt->bind_param("isiiiiss", $id_karyawan, $periode, $nilai_kinerja, $nilai_kedisiplinan, $nilai_kerjasama, $catatan, $evaluator, $tanggal_evaluasi);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Tambah Evaluasi', "ID Karyawan: $id_karyawan, Periode: $periode");
            jsonResponse(true, 'Evaluasi berhasil ditambahkan!');
        } else {
            jsonResponse(false, 'Gagal menambahkan evaluasi!');
        }
    }
    
    if ($action === 'update') {
        $id = $_POST['id'];
        $nilai_kinerja = $_POST['nilai_kinerja'];
        $nilai_kedisiplinan = $_POST['nilai_kedisiplinan'];
        $nilai_kerjasama = $_POST['nilai_kerjasama'];
        $catatan = sanitize($_POST['catatan']);
        
        $stmt = $conn->prepare("UPDATE evaluasi SET nilai_kinerja=?, nilai_kedisiplinan=?, nilai_kerjasama=?, catatan=? WHERE id=?");
        $stmt->bind_param("iiisi", $nilai_kinerja, $nilai_kedisiplinan, $nilai_kerjasama, $catatan, $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Update Evaluasi', "ID Evaluasi: $id");
            jsonResponse(true, 'Evaluasi berhasil diupdate!');
        } else {
            jsonResponse(false, 'Gagal mengupdate evaluasi!');
        }
    }
}

$evaluasiList = $conn->query("
    SELECT e.*, k.nama, k.jabatan 
    FROM evaluasi e 
    JOIN karyawan k ON e.id_karyawan = k.id 
    ORDER BY e.tanggal_evaluasi DESC
");

$karyawanList = $conn->query("SELECT id, nama FROM karyawan WHERE status='aktif' ORDER BY nama");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Evaluasi Karyawan - Admin</title>
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
                        <h3><i class="fas fa-chart-line"></i> Evaluasi Karyawan</h3>
                        <button class="btn btn-primary" onclick="showAddModal()">
                            <i class="fas fa-plus"></i> Tambah Evaluasi
                        </button>
                    </div>
                    <div class="card-body">
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Periode</th>
                                        <th>Kinerja</th>
                                        <th>Kedisiplinan</th>
                                        <th>Kerjasama</th>
                                        <th>Rata-rata</th>
                                        <th>Evaluator</th>
                                        <th>Tanggal</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php while ($row = $evaluasiList->fetch_assoc()): 
                                        $rata = ($row['nilai_kinerja'] + $row['nilai_kedisiplinan'] + $row['nilai_kerjasama']) / 3;
                                    ?>
                                        <tr>
                                            <td><?php echo $row['nama']; ?></td>
                                            <td><?php echo $row['jabatan']; ?></td>
                                            <td><span class="badge badge-info"><?php echo $row['periode']; ?></span></td>
                                            <td><?php echo $row['nilai_kinerja']; ?></td>
                                            <td><?php echo $row['nilai_kedisiplinan']; ?></td>
                                            <td><?php echo $row['nilai_kerjasama']; ?></td>
                                            <td>
                                                <strong style="color: <?php echo $rata >= 80 ? '#10b981' : ($rata >= 60 ? '#f59e0b' : '#ef4444'); ?>">
                                                    <?php echo number_format($rata, 1); ?>
                                                </strong>
                                            </td>
                                            <td><?php echo $row['evaluator']; ?></td>
                                            <td><?php echo formatTanggalIndo($row['tanggal_evaluasi']); ?></td>
                                            <td>
                                                <button class="btn btn-sm btn-info btn-icon" 
                                                        onclick='editEvaluasi(<?php echo json_encode($row); ?>)'>
                                                    <i class="fas fa-edit"></i>
                                                </button>
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
    
    <!-- Modal -->
    <div id="formModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3 id="modalTitle">Tambah Evaluasi</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="evaluasiForm">
                    <input type="hidden" id="evaluasiId" name="id">
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
                    
                    <div class="form-group" id="periodeGroup">
                        <label for="periode">Periode <span style="color: red;">*</span></label>
                        <input type="text" id="periode" name="periode" placeholder="Contoh: Q1 2025" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="nilai_kinerja">Nilai Kinerja (1-100) <span style="color: red;">*</span></label>
                        <input type="number" id="nilai_kinerja" name="nilai_kinerja" min="1" max="100" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="nilai_kedisiplinan">Nilai Kedisiplinan (1-100) <span style="color: red;">*</span></label>
                        <input type="number" id="nilai_kedisiplinan" name="nilai_kedisiplinan" min="1" max="100" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="nilai_kerjasama">Nilai Kerjasama (1-100) <span style="color: red;">*</span></label>
                        <input type="number" id="nilai_kerjasama" name="nilai_kerjasama" min="1" max="100" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="catatan">Catatan</label>
                        <textarea id="catatan" name="catatan"></textarea>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button class="btn btn-primary" onclick="submitForm()">Simpan</button>
            </div>
        </div>
    </div>
    
    <script src="../assets/js/evaluasi.js"></script>
</body>
</html>
