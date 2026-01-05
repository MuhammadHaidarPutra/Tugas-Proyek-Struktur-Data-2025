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
        $shift = $_POST['shift'];
        $jam_mulai = $_POST['jam_mulai'];
        $jam_selesai = $_POST['jam_selesai'];
        $keterangan = sanitize($_POST['keterangan']);
        
        $stmt = $conn->prepare("INSERT INTO jadwal (id_karyawan, tanggal, shift, jam_mulai, jam_selesai, keterangan) VALUES (?, ?, ?, ?, ?, ?)");
        $stmt->bind_param("isssss", $id_karyawan, $tanggal, $shift, $jam_mulai, $jam_selesai, $keterangan);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Tambah Jadwal', "ID Karyawan: $id_karyawan");
            jsonResponse(true, 'Jadwal berhasil ditambahkan!');
        } else {
            jsonResponse(false, 'Gagal menambahkan jadwal!');
        }
    }
    
    if ($action === 'delete') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("DELETE FROM jadwal WHERE id=?");
        $stmt->bind_param("i", $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Hapus Jadwal', "ID Jadwal: $id");
            jsonResponse(true, 'Jadwal berhasil dihapus!');
        } else {
            jsonResponse(false, 'Gagal menghapus jadwal!');
        }
    }
}

$tanggal = isset($_GET['tanggal']) ? $_GET['tanggal'] : date('Y-m-d');
$jadwalList = $conn->query("
    SELECT j.*, k.nama, k.jabatan 
    FROM jadwal j 
    JOIN karyawan k ON j.id_karyawan = k.id 
    WHERE j.tanggal >= '$tanggal'
    ORDER BY j.tanggal, j.jam_mulai
");

$karyawanList = $conn->query("SELECT id, nama FROM karyawan WHERE status='aktif' ORDER BY nama");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Jadwal Kerja - Admin</title>
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
                        <h3><i class="fas fa-clock"></i> Jadwal Kerja</h3>
                        <div style="display:flex;gap:10px;flex-wrap:wrap;">
                            <button class="btn btn-primary" onclick="showAddModal()">
                                <i class="fas fa-plus"></i> Tambah Jadwal
                            </button>
                            <button class="btn btn-success" onclick="generateDefaultJadwal(event)">
                                <i class="fas fa-magic"></i> Generate Jadwal Default
                            </button>
                        </div>
                    </div>
                    <div class="card-body">
                        <div class="mb-20">
                            <input type="date" id="filterTanggal" value="<?php echo $tanggal; ?>" 
                                   onchange="filterByDate()" style="padding: 10px; border: 2px solid #e5e7eb; border-radius: 8px;">
                        </div>
                        
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive" style="overflow-x:auto;">
                            <table style="min-width:900px;">
                                <thead>
                                    <tr>
                                        <th>Tanggal</th>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Shift</th>
                                        <th>Jam Mulai</th>
                                        <th>Jam Selesai</th>
                                        <th>Keterangan</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php while ($row = $jadwalList->fetch_assoc()): ?>
                                        <tr>
                                            <td><?php echo formatTanggalIndo($row['tanggal']); ?></td>
                                            <td><?php echo $row['nama']; ?></td>
                                            <td><?php echo $row['jabatan']; ?></td>
                                            <td>
                                                <?php 
                                                $shiftColors = ['pagi'=>'badge-warning','siang'=>'badge-info','malam'=>'badge-secondary'];
                                                ?>
                                                <span class="badge <?php echo $shiftColors[$row['shift']]; ?>">
                                                    <?php echo ucfirst($row['shift']); ?>
                                                </span>
                                            </td>
                                            <td><?php echo $row['jam_mulai']; ?></td>
                                            <td><?php echo $row['jam_selesai']; ?></td>
                                            <td><?php echo $row['keterangan'] ?: '-'; ?></td>
                                            <td>
                                                <button class="btn btn-sm btn-danger btn-icon" 
                                                        onclick="deleteJadwal(<?php echo $row['id']; ?>)">
                                                    <i class="fas fa-trash"></i>
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
    <script>
    function generateDefaultJadwal(e) {
        e.preventDefault();
        if(!confirm('Generate jadwal default untuk semua karyawan aktif selama 1 minggu ke depan?')) return;
        fetch('jadwal.php', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: 'action=generate_default'
        })
        .then(r=>r.json()).then(res=>{
            alert(res.message);
            if(res.success) location.reload();
        });
    }
    </script>
    
    <!-- Modal -->
    <div id="formModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Tambah Jadwal</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            // Handle CRUD & Generate Default Jadwal
            if ($_SERVER['REQUEST_METHOD'] === 'POST') {
                $action = $_POST['action'];
                if ($action === 'create') {
                    $id_karyawan = $_POST['id_karyawan'];
                    $tanggal = $_POST['tanggal'];
                    $shift = $_POST['shift'];
                    $jam_mulai = $_POST['jam_mulai'];
                    $jam_selesai = $_POST['jam_selesai'];
                    $keterangan = sanitize($_POST['keterangan']);
                    $stmt = $conn->prepare("INSERT INTO jadwal (id_karyawan, tanggal, shift, jam_mulai, jam_selesai, keterangan) VALUES (?, ?, ?, ?, ?, ?)");
                    $stmt->bind_param("isssss", $id_karyawan, $tanggal, $shift, $jam_mulai, $jam_selesai, $keterangan);
                    if ($stmt->execute()) {
                        logActivity($_SESSION['user_id'], 'Tambah Jadwal', "ID Karyawan: $id_karyawan");
                        jsonResponse(true, 'Jadwal berhasil ditambahkan!');
                    } else {
                        jsonResponse(false, 'Gagal menambahkan jadwal!');
                    }
                }
                if ($action === 'delete') {
                    $id = $_POST['id'];
                    $stmt = $conn->prepare("DELETE FROM jadwal WHERE id=?");
                    $stmt->bind_param("i", $id);
                    if ($stmt->execute()) {
                        logActivity($_SESSION['user_id'], 'Hapus Jadwal', "ID Jadwal: $id");
                        jsonResponse(true, 'Jadwal berhasil dihapus!');
                    } else {
                        jsonResponse(false, 'Gagal menghapus jadwal!');
                    }
                }
                if ($action === 'generate_default') {
                    // Generate jadwal default 1 minggu ke depan untuk semua karyawan aktif
                    $today = date('Y-m-d');
                    $end = date('Y-m-d', strtotime('+6 days'));
                    $shifts = [
                        ['pagi','07:00','15:00'],
                        ['siang','15:00','23:00'],
                        ['malam','23:00','07:00']
                    ];
                    $karyawan = $conn->query("SELECT id FROM karyawan WHERE status='aktif'");
                    $inserted = 0;
                    while ($k = $karyawan->fetch_assoc()) {
                        $idk = $k['id'];
                        $shiftIdx = 0;
                        for ($d = strtotime($today); $d <= strtotime($end); $d += 86400) {
                            $tanggal = date('Y-m-d', $d);
                            // Cek jika sudah ada jadwal, skip
                            $cek = $conn->query("SELECT id FROM jadwal WHERE id_karyawan=$idk AND tanggal='$tanggal'");
                            if ($cek->num_rows > 0) continue;
                            $s = $shifts[$shiftIdx % 3];
                            $stmt = $conn->prepare("INSERT INTO jadwal (id_karyawan, tanggal, shift, jam_mulai, jam_selesai, keterangan) VALUES (?, ?, ?, ?, ?, ?)");
                            $ket = '';
                            $stmt->bind_param("isssss", $idk, $tanggal, $s[0], $s[1], $s[2], $ket);
                            if ($stmt->execute()) $inserted++;
                            $shiftIdx++;
                        }
                    }
                    jsonResponse(true, "Jadwal default berhasil digenerate ($inserted jadwal ditambahkan)");
                }
            }
                    </div>
                    
                    <div class="form-group">
                        <label for="jam_selesai">Jam Selesai <span style="color: red;">*</span></label>
                        <input type="time" id="jam_selesai" name="jam_selesai" required>
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
    
    <script>
        const modal = document.getElementById('formModal');
        
        function showAddModal() {
            document.getElementById('jadwalForm').reset();
            modal.classList.add('show');
        }
        
        function closeModal() {
            modal.classList.remove('show');
        }
        
        function setJamShift() {
            const shift = document.getElementById('shift').value;
            const jamMulai = document.getElementById('jam_mulai');
            const jamSelesai = document.getElementById('jam_selesai');
            
            if (shift === 'pagi') {
                jamMulai.value = '07:00';
                jamSelesai.value = '15:00';
            } else if (shift === 'siang') {
                jamMulai.value = '15:00';
                jamSelesai.value = '23:00';
            } else if (shift === 'malam') {
                jamMulai.value = '23:00';
                jamSelesai.value = '07:00';
            }
        }
        
        async function submitForm() {
            const form = document.getElementById('jadwalForm');
            const formData = new FormData(form);
            
            try {
                const response = await fetch('jadwal.php', {
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
        
        async function deleteJadwal(id) {
            if (!confirm('Hapus jadwal ini?')) return;
            
            const formData = new FormData();
            formData.append('action', 'delete');
            formData.append('id', id);
            
            try {
                const response = await fetch('jadwal.php', {
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
        
        function filterByDate() {
            const tanggal = document.getElementById('filterTanggal').value;
            location.href = 'jadwal.php?tanggal=' + tanggal;
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
        
        window.onclick = function(event) {
            if (event.target === modal) closeModal();
        }
    </script>
</body>
</html>
