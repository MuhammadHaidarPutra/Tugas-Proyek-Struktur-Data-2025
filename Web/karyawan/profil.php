<?php
require_once '../config.php';

if (!isLoggedIn() || isAdmin()) {
    redirect('../index.php');
}

$idKaryawan = $_SESSION['id_karyawan'];
$dataKaryawan = $conn->query("SELECT * FROM karyawan WHERE id = $idKaryawan")->fetch_assoc();

// Handle update profil
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $email = sanitize($_POST['email']);
    $telepon = sanitize($_POST['telepon']);
    
    $stmt = $conn->prepare("UPDATE karyawan SET email=?, telepon=? WHERE id=?");
    $stmt->bind_param("ssi", $email, $telepon, $idKaryawan);
    
    if ($stmt->execute()) {
        $success = "Profil berhasil diupdate!";
        $dataKaryawan = $conn->query("SELECT * FROM karyawan WHERE id = $idKaryawan")->fetch_assoc();
    } else {
        $error = "Gagal mengupdate profil!";
    }
}
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Profil Saya - Karyawan</title>
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
                
                <div class="card">
                    <div class="card-header">
                        <h3><i class="fas fa-user-circle"></i> Informasi Profil</h3>
                    </div>
                    <div class="card-body">
                        <form method="POST">
                            <div class="form-group">
                                <label>ID Karyawan</label>
                                <input type="text" value="<?php echo $dataKaryawan['id']; ?>" readonly>
                            </div>
                            
                            <div class="form-group">
                                <label>Nama Lengkap</label>
                                <input type="text" value="<?php echo $dataKaryawan['nama']; ?>" readonly>
                            </div>
                            
                            <div class="form-group">
                                <label>Jabatan</label>
                                <input type="text" value="<?php echo $dataKaryawan['jabatan']; ?>" readonly>
                            </div>
                            
                            <div class="form-group">
                                <label>Departemen</label>
                                <input type="text" value="<?php echo $dataKaryawan['departemen']; ?>" readonly>
                            </div>
                            
                            <div class="form-group">
                                <label for="email">Email</label>
                                <input type="email" id="email" name="email" value="<?php echo $dataKaryawan['email']; ?>">
                            </div>
                            
                            <div class="form-group">
                                <label for="telepon">Telepon</label>
                                <input type="text" id="telepon" name="telepon" value="<?php echo $dataKaryawan['telepon']; ?>">
                            </div>
                            
                            <div class="form-group">
                                <label>Tanggal Masuk</label>
                                <input type="text" value="<?php echo formatTanggalIndo($dataKaryawan['tanggal_masuk']); ?>" readonly>
                            </div>
                            
                            <div class="form-group">
                                <label>Status</label>
                                <input type="text" value="<?php echo ucfirst($dataKaryawan['status']); ?>" readonly>
                            </div>
                            
                            <button type="submit" class="btn btn-primary">
                                <i class="fas fa-save"></i> Update Profil
                            </button>
                        </form>
                    </div>
                </div>
            </div>
        </main>
    </div>
</body>
</html>
