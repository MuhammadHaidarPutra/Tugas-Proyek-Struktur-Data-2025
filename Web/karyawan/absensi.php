<?php
require_once '../config.php';
if (!isLoggedIn() || isAdmin()) redirect('../index.php');
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Absensi Saya - Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <style>
    /* ABSENSI PAGE IMPROVED STYLE */
    .absensi-section {
        background: #fff;
        border-radius: 1.2rem;
        box-shadow: 0 4px 24px rgba(0,0,0,0.08);
        padding: 2rem 2.5rem;
        margin-bottom: 1.5rem;
        transition: box-shadow 0.2s;
        position: relative;
    }
    .absensi-section h3 {
        color: #2563eb;
        font-size: 1.3rem;
        margin-bottom: 1.2rem;
        text-align: center;
    }
    .absensi-section form label {
        font-weight: 500;
        color: #374151;
        margin-bottom: 0.5rem;
        display: flex;
        flex-direction: column;
        gap: 0.2rem;
    }
    .absensi-section input[type="date"],
    .absensi-section input[type="time"] {
        border: 1px solid #cbd5e1;
        border-radius: 6px;
        padding: 0.5rem 0.8rem;
        font-size: 1rem;
        margin-top: 0.2rem;
        margin-bottom: 0.7rem;
        background: #f8fafc;
        transition: border 0.2s;
    }
    .absensi-section input[type="date"]:focus,
    .absensi-section input[type="time"]:focus {
        border: 1.5px solid #2563eb;
        outline: none;
    }
    .absensi-section button[type="submit"] {
        background: linear-gradient(90deg, #2563eb 60%, #4f8cff 100%);
        color: #fff;
        border: none;
        border-radius: 8px;
        padding: 0.7rem 1.2rem;
        font-size: 1rem;
        font-weight: 600;
        cursor: pointer;
        margin-top: 0.5rem;
        box-shadow: 0 2px 8px rgba(37,99,235,0.08);
        transition: background 0.2s, box-shadow 0.2s;
    }
    .absensi-section button[type="submit"]:hover {
        background: linear-gradient(90deg, #1746a0 60%, #2563eb 100%);
        box-shadow: 0 4px 16px rgba(37,99,235,0.13);
    }
    .absensi-section .alert {
        border-radius: 8px;
        padding: 0.8rem 1.2rem;
        margin-top: 1rem;
        font-size: 1rem;
        font-weight: 500;
        display: flex;
        align-items: center;
        gap: 0.7rem;
    }
    .absensi-section .alert-success {
        background: #e0f7e9;
        color: #15803d;
        border: 1px solid #34d399;
    }
    .absensi-section .alert-danger {
        background: #fee2e2;
        color: #b91c1c;
        border: 1px solid #f87171;
    }
    .absensi-section .alert-warning {
        background: #fef9c3;
        color: #b45309;
        border: 1px solid #fde68a;
    }
    .absensi-section table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 1rem;
        background: #f8fafc;
        border-radius: 0.7rem;
        overflow: hidden;
        font-size: 1rem;
    }
    .absensi-section th, .absensi-section td {
        padding: 10px 12px;
        text-align: center;
    }
    .absensi-section thead {
        background: #2563eb;
        color: #fff;
    }
    .absensi-section tbody tr:nth-child(even) {
        background: #e0e7ef;
    }
    .absensi-section tbody tr:nth-child(odd) {
        background: #f8fafc;
    }
    @media (max-width: 900px) {
        .container { padding: 1rem 0.2rem !important; }
        .absensi-section { padding: 1rem 0.5rem; }
    }
</style>
<script>
// Real-time update for jam masuk and jam keluar
function updateTimeInputs() {
    var now = new Date();
    var jam = now.toLocaleTimeString('it-IT').slice(0,5);
    var jamMasuk = document.getElementById('jamMasuk');
    var jamKeluar = document.getElementById('jamKeluar');
    if (jamMasuk) jamMasuk.value = jam;
    if (jamKeluar) jamKeluar.value = jam;
}
setInterval(updateTimeInputs, 1000);
window.onload = updateTimeInputs;
</script>
</head>
<body>
<?php include 'sidebar.php'; ?>
    <div class="dashboard">
        <?php include 'sidebar.php'; ?>
        <main class="main-content">
            <?php include 'topbar.php'; ?>
            <div class="container" style="max-width: 900px; margin: 0 auto; padding: 2rem 0;">
                <h2 style="text-align:center; color:#2563eb; margin-bottom:2rem;">Absensi Saya</h2>
                <div style="display:flex; flex-wrap:wrap; gap:2rem; justify-content:center;">
                    <section class="absensi-section" style="flex:1 1 320px; min-width:320px; background:#fff; border-radius:1rem; box-shadow:0 2px 12px rgba(0,0,0,0.06); padding:2rem;">
                        <h3 style="text-align:center; color:#1e293b;">Check In</h3>
                        <form method="post" action="" style="display:flex; flex-direction:column; gap:1rem;">
                            <label>Tanggal <input type="date" name="tanggal_in" value="<?php echo date('Y-m-d'); ?>" required readonly></label>
                            <label style="position:relative;">Jam Masuk
                                <input type="time" id="jamMasuk" name="jam_masuk" value="<?php echo date('H:i'); ?>" required readonly>
                            </label>
                            <button type="submit" name="check_in" style="background:#2563eb; color:#fff; border:none; border-radius:6px; padding:0.5rem 1rem; cursor:pointer;">Check In</button>
                        </form>
                        <?php
                        if (isset($_POST['check_in'])) {
                            $tanggal = $_POST['tanggal_in'];
                            $jam_masuk = $_POST['jam_masuk'];
                            $idKaryawan = $_SESSION['id_karyawan'];
                            // Cek apakah sudah check in hari ini
                            $cek = $conn->query("SELECT id FROM absensi WHERE id_karyawan = $idKaryawan AND tanggal = '$tanggal'");
                            if ($cek === false) {
                                echo '<div class="alert alert-danger" style="margin-top:1rem;">Terjadi kesalahan pada database: ' . $conn->error . '</div>';
                            } else if ($cek->num_rows > 0) {
                                echo '<div class="alert alert-danger" style="margin-top:1rem;">Anda sudah melakukan check in untuk tanggal ini.</div>';
                            } else {
                                $status = 'hadir';
                                $keterangan = (strtotime($jam_masuk) <= strtotime('08:00')) ? 'Tepat waktu' : 'Terlambat';
                                $stmt = $conn->prepare("INSERT INTO absensi (id_karyawan, tanggal, jam_masuk, status, keterangan) VALUES (?, ?, ?, ?, ?)");
                                if ($stmt === false) {
                                    echo '<div class="alert alert-danger" style="margin-top:1rem;">Terjadi kesalahan pada database: ' . $conn->error . '</div>';
                                } else {
                                    $stmt->bind_param("issss", $idKaryawan, $tanggal, $jam_masuk, $status, $keterangan);
                                    if ($stmt->execute()) {
                                        echo '<div class="alert alert-success" style="margin-top:1rem;">Check in berhasil! ' . $keterangan . '</div>';
                                    } else {
                                        echo '<div class="alert alert-danger" style="margin-top:1rem;">Check in gagal! ' . $stmt->error . '</div>';
                                    }
                                }
                            }
                        }
                        ?>
                    </section>
                    <section class="absensi-section" style="flex:1 1 320px; min-width:320px; background:#fff; border-radius:1rem; box-shadow:0 2px 12px rgba(0,0,0,0.06); padding:2rem;">
                        <h3 style="text-align:center; color:#1e293b;">Check Out</h3>
                        <form method="post" action="" style="display:flex; flex-direction:column; gap:1rem;">
                            <label>Tanggal <input type="date" name="tanggal_out" value="<?php echo date('Y-m-d'); ?>" required readonly></label>
                            <label style="position:relative;">Jam Keluar
                                <input type="time" id="jamKeluar" name="jam_keluar" value="<?php echo date('H:i'); ?>" required readonly>
                            </label>
                            <button type="submit" name="check_out" style="background:#2563eb; color:#fff; border:none; border-radius:6px; padding:0.5rem 1rem; cursor:pointer;">Check Out</button>
                        </form>
                        <?php
                        if (isset($_POST['check_out'])) {
                            $tanggal = $_POST['tanggal_out'];
                            $jam_keluar = $_POST['jam_keluar'];
                            $idKaryawan = $_SESSION['id_karyawan'];
                            $result = $conn->query("SELECT jam_masuk FROM absensi WHERE id_karyawan = $idKaryawan AND tanggal = '$tanggal'");
                            $row = $result->fetch_assoc();
                            if ($row && $row['jam_masuk']) {
                                $jam_masuk = $row['jam_masuk'];
                                $durasi = (strtotime($jam_keluar) - strtotime($jam_masuk)) / 3600;
                                $durasi = round($durasi, 2);
                                $stmt = $conn->prepare("UPDATE absensi SET jam_keluar=?, durasi_kerja=? WHERE id_karyawan=? AND tanggal=?");
                                $stmt->bind_param("ssis", $jam_keluar, $durasi, $idKaryawan, $tanggal);
                                if ($stmt->execute()) {
                                    echo '<div class="alert alert-success" style="margin-top:1rem;">Check out berhasil! Durasi kerja: ' . $durasi . ' jam</div>';
                                } else {
                                    echo '<div class="alert alert-danger" style="margin-top:1rem;">Check out gagal!</div>';
                                }
                            } else {
                                echo '<div class="alert alert-warning" style="margin-top:1rem;">Check in belum dilakukan hari ini.</div>';
                            }
                        }
                        ?>
                    </section>
                </div>
                <section class="absensi-section" style="margin-top:2.5rem; background:#fff; border-radius:1rem; box-shadow:0 2px 12px rgba(0,0,0,0.06); padding:2rem;">
                    <h3 style="text-align:center; color:#1e293b;">Lihat Absensi</h3>
                    <div style="overflow-x:auto;">
                    <table style="width:100%; border-collapse:collapse; margin-top:1rem;">
                        <thead style="background:#f1f5f9;">
                            <tr>
                                <th style="padding:8px;">ID</th>
                                <th style="padding:8px;">Tanggal</th>
                                <th style="padding:8px;">Jam Masuk</th>
                                <th style="padding:8px;">Jam Keluar</th>
                                <th style="padding:8px;">Durasi (jam)</th>
                                <th style="padding:8px;">Status</th>
                            </tr>
                        </thead>
                        <tbody>
                        <?php
                        $idKaryawan = $_SESSION['id_karyawan'];
                        $result = $conn->query("SELECT * FROM absensi WHERE id_karyawan = $idKaryawan ORDER BY tanggal DESC");
                        if ($result->num_rows > 0) {
                            while ($row = $result->fetch_assoc()) {
                                echo '<tr>';
                                echo '<td style="padding:8px;">' . $row['id'] . '</td>';
                                echo '<td style="padding:8px;">' . $row['tanggal'] . '</td>';
                                echo '<td style="padding:8px;">' . ($row['jam_masuk'] ?: '-') . '</td>';
                                echo '<td style="padding:8px;">' . ($row['jam_keluar'] ?: '-') . '</td>';
                                echo '<td style="padding:8px;">' . ($row['durasi_kerja'] ?: '0') . '</td>';
                                echo '<td style="padding:8px;">' . ucfirst($row['status']) . '</td>';
                                echo '</tr>';
                            }
                        } else {
                            echo '<tr><td colspan="6" style="text-align:center;">Belum ada data absensi</td></tr>';
                        }
                        ?>
                        </tbody>
                    </table>
                    </div>
                </section>
            </div>
        </main>
    </div>
</main>
</body>
</html>
