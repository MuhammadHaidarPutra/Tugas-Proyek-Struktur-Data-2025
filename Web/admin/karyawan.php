<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Handle CRUD
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'];
    
    if ($action === 'create') {
        $nama = sanitize($_POST['nama']);
        $jabatan = sanitize($_POST['jabatan']);
        $departemen = sanitize($_POST['departemen']);
        $email = sanitize($_POST['email']);
        $telepon = sanitize($_POST['telepon']);
        $tanggal_masuk = $_POST['tanggal_masuk'];
        $gaji_pokok = $_POST['gaji_pokok'];
        
        $stmt = $conn->prepare("INSERT INTO karyawan (nama, jabatan, departemen, email, telepon, tanggal_masuk, gaji_pokok) VALUES (?, ?, ?, ?, ?, ?, ?)");
        $stmt->bind_param("ssssssd", $nama, $jabatan, $departemen, $email, $telepon, $tanggal_masuk, $gaji_pokok);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Tambah Karyawan', "Menambah karyawan: $nama");
            jsonResponse(true, 'Karyawan berhasil ditambahkan!');
        } else {
            jsonResponse(false, 'Gagal menambahkan karyawan!');
        }
    }
    
    if ($action === 'update') {
        $id = $_POST['id'];
        $nama = sanitize($_POST['nama']);
        $jabatan = sanitize($_POST['jabatan']);
        $departemen = sanitize($_POST['departemen']);
        $email = sanitize($_POST['email']);
        $telepon = sanitize($_POST['telepon']);
        $gaji_pokok = $_POST['gaji_pokok'];
        $status = $_POST['status'];
        
        $stmt = $conn->prepare("UPDATE karyawan SET nama=?, jabatan=?, departemen=?, email=?, telepon=?, gaji_pokok=?, status=? WHERE id=?");
        $stmt->bind_param("sssssdsi", $nama, $jabatan, $departemen, $email, $telepon, $gaji_pokok, $status, $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Update Karyawan', "Mengupdate karyawan ID: $id");
            jsonResponse(true, 'Karyawan berhasil diupdate!');
        } else {
            jsonResponse(false, 'Gagal mengupdate karyawan!');
        }
    }
    
    if ($action === 'delete') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("DELETE FROM karyawan WHERE id=?");
        $stmt->bind_param("i", $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Hapus Karyawan', "Menghapus karyawan ID: $id");
            jsonResponse(true, 'Karyawan berhasil dihapus!');
        } else {
            jsonResponse(false, 'Gagal menghapus karyawan!');
        }
    }
}

// Get data karyawan
$search = isset($_GET['search']) ? sanitize($_GET['search']) : '';
$whereClause = $search ? "WHERE nama LIKE '%$search%' OR jabatan LIKE '%$search%' OR departemen LIKE '%$search%'" : '';
$karyawanList = $conn->query("SELECT * FROM karyawan $whereClause ORDER BY id DESC");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Data Karyawan - Admin</title>
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
                        <h3><i class="fas fa-users"></i> Data Karyawan</h3>
                        <button class="btn btn-primary" onclick="showAddModal()">
                            <i class="fas fa-plus"></i> Tambah Karyawan
                        </button>
                    </div>
                    <div class="card-body">
                        <!-- Search -->
                        <div class="mb-20">
                            <input type="text" id="searchInput" placeholder="Cari nama, jabatan, atau departemen..." 
                                   style="width: 100%; max-width: 400px; padding: 10px; border: 2px solid #e5e7eb; border-radius: 8px;">
                        </div>
                        
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>ID</th>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Departemen</th>
                                        <th>Email</th>
                                        <th>Telepon</th>
                                        <th>Gaji Pokok</th>
                                        <th>Status</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody id="karyawanTable">
                                    <?php while ($row = $karyawanList->fetch_assoc()): ?>
                                        <tr>
                                            <td><?php echo $row['id']; ?></td>
                                            <td><?php echo $row['nama']; ?></td>
                                            <td><?php echo $row['jabatan']; ?></td>
                                            <td><?php echo $row['departemen']; ?></td>
                                            <td><?php echo $row['email'] ?: '-'; ?></td>
                                            <td><?php echo $row['telepon'] ?: '-'; ?></td>
                                            <td><?php echo formatRupiah($row['gaji_pokok']); ?></td>
                                            <td>
                                                <span class="badge <?php echo $row['status'] === 'aktif' ? 'badge-success' : 'badge-danger'; ?>">
                                                    <?php echo ucfirst($row['status']); ?>
                                                </span>
                                            </td>
                                            <td>
                                                <div class="action-buttons">
                                                    <button class="btn btn-sm btn-info btn-icon" 
                                                            onclick='editKaryawan(<?php echo json_encode($row); ?>)' 
                                                            title="Edit">
                                                        <i class="fas fa-edit"></i>
                                                    </button>
                                                    <button class="btn btn-sm btn-danger btn-icon" 
                                                            onclick="deleteKaryawan(<?php echo $row['id']; ?>)" 
                                                            title="Hapus">
                                                        <i class="fas fa-trash"></i>
                                                    </button>
                                                </div>
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
    
    <!-- Modal Form -->
    <div id="formModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3 id="modalTitle">Tambah Karyawan</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="karyawanForm">
                    <input type="hidden" id="karyawanId" name="id">
                    <input type="hidden" id="formAction" name="action" value="create">
                    
                    <div class="form-group">
                        <label for="nama">Nama Lengkap <span style="color: red;">*</span></label>
                        <input type="text" id="nama" name="nama" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="jabatan">Jabatan <span style="color: red;">*</span></label>
                        <input type="text" id="jabatan" name="jabatan" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="departemen">Departemen <span style="color: red;">*</span></label>
                        <select id="departemen" name="departemen" required>
                            <option value="">Pilih Departemen</option>
                            <option value="IT">IT</option>
                            <option value="HR">HR</option>
                            <option value="Finance">Finance</option>
                            <option value="Marketing">Marketing</option>
                            <option value="Operations">Operations</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="email">Email</label>
                        <input type="email" id="email" name="email">
                    </div>
                    
                    <div class="form-group">
                        <label for="telepon">Telepon</label>
                        <input type="text" id="telepon" name="telepon">
                    </div>
                    
                    <div class="form-group">
                        <label for="tanggal_masuk">Tanggal Masuk</label>
                        <input type="date" id="tanggal_masuk" name="tanggal_masuk">
                    </div>
                    
                    <div class="form-group">
                        <label for="gaji_pokok">Gaji Pokok</label>
                        <input type="number" id="gaji_pokok" name="gaji_pokok" value="0" step="1000">
                    </div>
                    
                    <div class="form-group" id="statusGroup" style="display: none;">
                        <label for="status">Status</label>
                        <select id="status" name="status">
                            <option value="aktif">Aktif</option>
                            <option value="non-aktif">Non-Aktif</option>
                        </select>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button class="btn btn-primary" onclick="submitForm()">Simpan</button>
            </div>
        </div>
    </div>
    
    <script src="../assets/js/karyawan.js"></script>
</body>
</html>
