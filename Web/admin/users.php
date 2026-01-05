<?php
require_once '../config.php';

if (!isLoggedIn() || !isAdmin()) {
    redirect('../index.php');
}

// Handle CRUD
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'];
    
    if ($action === 'create') {
        $username = sanitize($_POST['username']);
        $password = md5($_POST['password']);
        $role = $_POST['role'];
        $nama = sanitize($_POST['nama']);
        $id_karyawan = $_POST['id_karyawan'] ?: null;
        
        // Check duplicate username
        $check = $conn->query("SELECT id FROM users WHERE username='$username'");
        if ($check->num_rows > 0) {
            jsonResponse(false, 'Username sudah digunakan!');
        }
        
        $stmt = $conn->prepare("INSERT INTO users (username, password, role, nama, id_karyawan) VALUES (?, ?, ?, ?, ?)");
        $stmt->bind_param("ssssi", $username, $password, $role, $nama, $id_karyawan);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Tambah User', "Username: $username");
            jsonResponse(true, 'User berhasil ditambahkan!');
        } else {
            jsonResponse(false, 'Gagal menambahkan user!');
        }
    }
    
    if ($action === 'reset_password') {
        $id = $_POST['id'];
        $newPassword = md5('password123');
        
        $stmt = $conn->prepare("UPDATE users SET password=? WHERE id=?");
        $stmt->bind_param("si", $newPassword, $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Reset Password', "User ID: $id");
            jsonResponse(true, 'Password direset ke: password123');
        } else {
            jsonResponse(false, 'Gagal reset password!');
        }
    }
    
    if ($action === 'delete') {
        $id = $_POST['id'];
        
        if ($id == $_SESSION['user_id']) {
            jsonResponse(false, 'Tidak bisa menghapus user sendiri!');
        }
        
        $stmt = $conn->prepare("DELETE FROM users WHERE id=?");
        $stmt->bind_param("i", $id);
        
        if ($stmt->execute()) {
            logActivity($_SESSION['user_id'], 'Hapus User', "User ID: $id");
            jsonResponse(true, 'User berhasil dihapus!');
        } else {
            jsonResponse(false, 'Gagal menghapus user!');
        }
    }
}

$userList = $conn->query("
    SELECT u.*, k.nama as nama_karyawan 
    FROM users u 
    LEFT JOIN karyawan k ON u.id_karyawan = k.id 
    ORDER BY u.id DESC
");

$karyawanList = $conn->query("SELECT id, nama FROM karyawan WHERE status='aktif' AND id NOT IN (SELECT id_karyawan FROM users WHERE id_karyawan IS NOT NULL) ORDER BY nama");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Manajemen User - Admin</title>
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
                        <h3><i class="fas fa-user-shield"></i> Manajemen User</h3>
                        <button class="btn btn-primary" onclick="showAddModal()">
                            <i class="fas fa-plus"></i> Tambah User
                        </button>
                    </div>
                    <div class="card-body">
                        <div id="alert-container"></div>
                        
                        <div class="table-responsive">
                            <table>
                                <thead>
                                    <tr>
                                        <th>ID</th>
                                        <th>Username</th>
                                        <th>Nama</th>
                                        <th>Role</th>
                                        <th>Karyawan</th>
                                        <th>Dibuat</th>
                                        <th>Aksi</th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php while ($row = $userList->fetch_assoc()): ?>
                                        <tr>
                                            <td><?php echo $row['id']; ?></td>
                                            <td><strong><?php echo $row['username']; ?></strong></td>
                                            <td><?php echo $row['nama']; ?></td>
                                            <td>
                                                <span class="badge <?php echo $row['role']==='admin' ? 'badge-danger' : 'badge-info'; ?>">
                                                    <?php echo ucfirst($row['role']); ?>
                                                </span>
                                            </td>
                                            <td><?php echo $row['nama_karyawan'] ?: '-'; ?></td>
                                            <td><?php echo date('d/m/Y', strtotime($row['created_at'])); ?></td>
                                            <td>
                                                <div class="action-buttons">
                                                    <button class="btn btn-sm btn-warning btn-icon" 
                                                            onclick="resetPassword(<?php echo $row['id']; ?>)" 
                                                            title="Reset Password">
                                                        <i class="fas fa-key"></i>
                                                    </button>
                                                    <?php if ($row['id'] != $_SESSION['user_id']): ?>
                                                        <button class="btn btn-sm btn-danger btn-icon" 
                                                                onclick="deleteUser(<?php echo $row['id']; ?>)" 
                                                                title="Hapus">
                                                            <i class="fas fa-trash"></i>
                                                        </button>
                                                    <?php endif; ?>
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
    
    <!-- Modal -->
    <div id="formModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Tambah User</h3>
                <button class="modal-close" onclick="closeModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="userForm">
                    <input type="hidden" name="action" value="create">
                    
                    <div class="form-group">
                        <label for="username">Username <span style="color: red;">*</span></label>
                        <input type="text" id="username" name="username" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="password">Password <span style="color: red;">*</span></label>
                        <input type="password" id="password" name="password" required>
                    </div>
                    
                    <div class="form-group">
                        <label for="role">Role <span style="color: red;">*</span></label>
                        <select id="role" name="role" required onchange="toggleKaryawan()">
                            <option value="admin">Admin</option>
                            <option value="karyawan">Karyawan</option>
                        </select>
                    </div>
                    
                    <div class="form-group">
                        <label for="nama">Nama <span style="color: red;">*</span></label>
                        <input type="text" id="nama" name="nama" required>
                    </div>
                    
                    <div class="form-group" id="karyawanGroup" style="display: none;">
                        <label for="id_karyawan">Link ke Karyawan</label>
                        <select id="id_karyawan" name="id_karyawan">
                            <option value="">Pilih Karyawan</option>
                            <?php 
                            $karyawanList->data_seek(0);
                            while ($k = $karyawanList->fetch_assoc()): 
                            ?>
                                <option value="<?php echo $k['id']; ?>"><?php echo $k['nama']; ?></option>
                            <?php endwhile; ?>
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
    
    <script>
        const modal = document.getElementById('formModal');
        
        function showAddModal() {
            document.getElementById('userForm').reset();
            modal.classList.add('show');
        }
        
        function closeModal() {
            modal.classList.remove('show');
        }
        
        function toggleKaryawan() {
            const role = document.getElementById('role').value;
            document.getElementById('karyawanGroup').style.display = role === 'karyawan' ? 'block' : 'none';
        }
        
        async function submitForm() {
            const form = document.getElementById('userForm');
            const formData = new FormData(form);
            
            try {
                const response = await fetch('users.php', {
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
        
        async function resetPassword(id) {
            if (!confirm('Reset password user ini ke: password123?')) return;
            
            const formData = new FormData();
            formData.append('action', 'reset_password');
            formData.append('id', id);
            
            try {
                const response = await fetch('users.php', {
                    method: 'POST',
                    body: formData
                });
                
                const data = await response.json();
                
                if (data.success) {
                    showAlert('success', data.message);
                } else {
                    showAlert('error', data.message);
                }
            } catch (error) {
                showAlert('error', 'Terjadi kesalahan sistem!');
            }
        }
        
        async function deleteUser(id) {
            if (!confirm('Hapus user ini?')) return;
            
            const formData = new FormData();
            formData.append('action', 'delete');
            formData.append('id', id);
            
            try {
                const response = await fetch('users.php', {
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
        
        window.onclick = function(event) {
            if (event.target === modal) closeModal();
        }
    </script>
</body>
</html>
