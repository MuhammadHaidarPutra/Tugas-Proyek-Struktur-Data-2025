<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Handle Actions
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'];
    
    if ($action === 'generate') {
        $bulan = $_POST['bulan'];
        $tahun = $_POST['tahun'];
        
        // Get all active employees
        $karyawanList = $conn->query("SELECT * FROM karyawan WHERE status='aktif'");
        
        $success = 0;
        while ($k = $karyawanList->fetch_assoc()) {
            $gaji_pokok = $k['gaji_pokok'];
            $tunjangan = $gaji_pokok * 0.2; // 20% tunjangan
            $potongan = $gaji_pokok * 0.05; // 5% potongan
            $total_gaji = $gaji_pokok + $tunjangan - $potongan;
            
            // Check if already exists
            $check = $conn->query("SELECT id FROM payroll WHERE id_karyawan={$k['id']} AND bulan=$bulan AND tahun=$tahun");
            if ($check->num_rows === 0) {
                $stmt = $conn->prepare("INSERT INTO payroll (id_karyawan, bulan, tahun, gaji_pokok, tunjangan, potongan, total_gaji) VALUES (?, ?, ?, ?, ?, ?, ?)");
                $stmt->bind_param("iiidddd", $k['id'], $bulan, $tahun, $gaji_pokok, $tunjangan, $potongan, $total_gaji);
                if ($stmt->execute()) $success++;
            }
        }
        
        logActivity($_SESSION['user_id'], 'Generate Payroll', "Bulan: $bulan, Tahun: $tahun, Total: $success");
        jsonResponse(true, "Berhasil generate $success payroll!");
    }
    
    if ($action === 'bayar') {
        $id = $_POST['id'];
        $stmt = $conn->prepare("UPDATE payroll SET status='dibayar', tanggal_bayar=CURDATE() WHERE id=?");
        $stmt->bind_param("i", $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Bayar Payroll', "ID Payroll: $id");
            jsonResponse(true, 'Payroll berhasil dibayar!');
        } else {
            jsonResponse(false, 'Gagal membayar payroll!');
        }
    }
}

$bulan = isset($_GET['bulan']) ? $_GET['bulan'] : date('n');
$tahun = isset($_GET['tahun']) ? $_GET['tahun'] : date('Y');

$payrollList = $conn->query("
    SELECT p.*, k.nama, k.jabatan 
    FROM payroll p 
    JOIN karyawan k ON p.id_karyawan = k.id 
    WHERE p.bulan = $bulan AND p.tahun = $tahun
    ORDER BY k.nama
");

$totalGaji = $conn->query("SELECT SUM(total_gaji) as total FROM payroll WHERE bulan=$bulan AND tahun=$tahun")->fetch_assoc()['total'];
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Payroll - Admin</title>
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
                        <h3><i class="fas fa-money-bill-wave"></i> Payroll Management</h3>
                        <button class="btn btn-primary" onclick="showGenerateModal()">
                            <i class="fas fa-cog"></i> Generate Payroll
                        </button>
                    </div>
                    <div class="card-body">
                        <div class="mb-20 d-flex gap-10">
                            <select id="filterBulan" style="padding: 10px; border: 2px solid #e5e7eb; border-radius: 8px;">
                                <?php 
                                $namaBulan = ['', 'Januari', 'Februari', 'Maret', 'April', 'Mei', 'Juni', 
                                             'Juli', 'Agustus', 'September', 'Oktober', 'November', 'Desember'];
                                for($i=1; $i<=12; $i++):
                                ?>
                                    <option value="<?php echo $i; ?>" <?php echo $i==$bulan ? 'selected' : ''; ?>>
                                        <?php echo $namaBulan[$i]; ?>
                                    </option>
                                <?php endfor; ?>
                            </select>
                            
                            <select id="filterTahun" style="padding: 10px; border: 2px solid #e5e7eb; border-radius: 8px;">
                                <?php for($y=2023; $y<=2030; $y++): ?>
                                    <option value="<?php echo $y; ?>" <?php echo $y==$tahun ? 'selected' : ''; ?>><?php echo $y; ?></option>
                                <?php endfor; ?>
                            </select>
                            
                            <button class="btn btn-info" onclick="filterData()">
                                <i class="fas fa-filter"></i> Filter
                            </button>
                        </div>
                        
                        <div class="alert alert-info mb-20">
                            <i class="fas fa-info-circle"></i>
                            Total Gaji Periode Ini: <strong><?php echo formatRupiah($totalGaji ?? 0); ?></strong>
                        </div>
                        
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>Nama</th>
                                        <th>Jabatan</th>
                                        <th>Gaji Pokok</th>
                                        <th>Tunjangan</th>
                                        <th>Potongan</th>
                                        <th>Total Gaji</th>
                                        <th>Status</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php if ($payrollList->num_rows > 0): ?>
                                        <?php while ($row = $payrollList->fetch_assoc()): ?>
                                            <tr>
                                                <td><?php echo $row['nama']; ?></td>
                                                <td><?php echo $row['jabatan']; ?></td>
                                                <td><?php echo formatRupiah($row['gaji_pokok']); ?></td>
                                                <td><?php echo formatRupiah($row['tunjangan']); ?></td>
                                                <td><?php echo formatRupiah($row['potongan']); ?></td>
                                                <td><strong><?php echo formatRupiah($row['total_gaji']); ?></strong></td>
                                                <td>
                                                    <span class="badge <?php echo $row['status']==='dibayar' ? 'badge-success' : 'badge-warning'; ?>">
                                                        <?php echo ucfirst($row['status']); ?>
                                                    </span>
                                                </td>
                                                <td>
                                                    <?php if ($row['status'] === 'draft'): ?>
                                                        <button class="btn btn-sm btn-success" onclick="bayarPayroll(<?php echo $row['id']; ?>)">
                                                            <i class="fas fa-check"></i> Bayar
                                                        </button>
                                                    <?php else: ?>
                                                        <small><?php echo formatTanggalIndo($row['tanggal_bayar']); ?></small>
                                                    <?php endif; ?>
                                                </td>
                                            </tr>
                                        <?php endwhile; ?>
                                    <?php else: ?>
                                        <tr>
                                            <td colspan="8" class="text-center">Belum ada data payroll untuk periode ini</td>
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
    
    <!-- Modal Generate -->
    <div id="generateModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Generate Payroll</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="generateForm">
                    <input type="hidden" name="action" value="generate">
                    
                    <div class="form-group">
                        <label for="bulan">Bulan <span style="color: red;">*</span></label>
                        <select id="bulan" name="bulan" required>
                            <?php 
                            for($i=1; $i<=12; $i++):
                            ?>
                                <option value="<?php echo $i; ?>"><?php echo $namaBulan[$i]; ?></option>
                            <?php endfor; ?>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="tahun">Tahun <span style="color: red;">*</span></label>
                        <select id="tahun" name="tahun" required>
                            <?php for($y=2023; $y<=2030; $y++): ?>
                                <option value="<?php echo $y; ?>" <?php echo $y==date('Y') ? 'selected' : ''; ?>><?php echo $y; ?></option>
                            <?php endfor; ?>
                        </select>
                    </div>
                    
                    <div class="alert alert-warning">
                        <i class="fas fa-exclamation-triangle"></i>
                        Akan generate payroll untuk semua karyawan aktif dengan perhitungan:
                        <ul style="margin-top: 10px;">
                            <li>Gaji Pokok: Sesuai data karyawan</li>
                            <li>Tunjangan: 20% dari gaji pokok</li>
                            <li>Potongan: 5% dari gaji pokok</li>
                        </ul>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" onclick="closeModal()">Batal</button>
                <button class="btn btn-primary" onclick="submitGenerate()">Generate</button>
            </div>
        </div>
    </div>
    
    <script>
        const modal = document.getElementById('generateModal');
        
        function showGenerateModal() {
            modal.classList.add('show');
        }
        
        function closeModal() {
            modal.classList.remove('show');
        }
        
        async function submitGenerate() {
            const form = document.getElementById('generateForm');
            const formData = new FormData(form);
            
            try {
                const response = await fetch('payroll.php', {
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
        
        async function bayarPayroll(id) {
            if (!confirm('Konfirmasi pembayaran payroll ini?')) return;
            
            const formData = new FormData();
            formData.append('action', 'bayar');
            formData.append('id', id);
            
            try {
                const response = await fetch('payroll.php', {
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
        
        function filterData() {
            const bulan = document.getElementById('filterBulan').value;
            const tahun = document.getElementById('filterTahun').value;
            location.href = 'payroll.php?bulan=' + bulan + '&tahun=' + tahun;
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
