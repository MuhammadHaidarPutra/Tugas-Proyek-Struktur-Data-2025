<?php
require_once '../config.php';
if (!isLoggedIn() || isAdmin()) redirect('../index.php');
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Jadwal Saya - Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <style>
    body {
        min-height: 100vh;
        background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        margin: 0;
        font-family: 'Segoe UI', Arial, sans-serif;
    }
    .container {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: flex-start;
        min-height: 100vh;
        padding-top: 40px;
    }
    .jadwal-card {
        background: #fff;
        border-radius: 1.2rem;
        box-shadow: 0 8px 32px rgba(37,99,235,0.13);
        padding: 2.5rem 2rem 2rem 2rem;
        margin-bottom: 2rem;
        width: 100%;
        max-width: 600px;
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    .jadwal-card h2 {
        color: #2563eb;
        margin-bottom: 1.5rem;
        text-align: center;
    }
    .jadwal-table {
        width: 100%;
        border-collapse: separate;
        border-spacing: 0;
        background: #fff;
        border-radius: 1rem;
        overflow: hidden;
        box-shadow: 0 2px 16px rgba(37,99,235,0.10);
    }
    .jadwal-table thead {
        background: linear-gradient(90deg,#2563eb 60%,#4f8cff 100%);
        color: #fff;
    }
    .jadwal-table th, .jadwal-table td {
        padding: 14px 10px;
        font-size: 1.05rem;
        text-align: center;
    }
    .jadwal-table tbody tr {
        border-bottom: 1px solid #e5e7eb;
    }
    .jadwal-table tbody tr:last-child {
        border-bottom: none;
    }
    .jadwal-table td:first-child {
        font-weight: 600;
        color: #2563eb;
    }
    .jadwal-info {
        margin-top: 1.5rem;
        text-align: center;
        color: #64748b;
        font-size: 1rem;
    }
    @media (max-width: 700px) {
        .jadwal-card { padding: 1.2rem 0.2rem; }
        .jadwal-table th, .jadwal-table td { padding: 8px 4px; font-size: 0.98rem; }
    }
    </style>
</head>
<body>
<?php include 'sidebar.php'; ?>
<main class="main-content">
    <?php include 'topbar.php'; ?>
    <div class="container">
        <div class="jadwal-card">
            <h2>Jadwal Kerja Minggu Ini</h2>
            <table class="jadwal-table">
                <thead>
                    <tr>
                        <th>Hari</th>
                        <th>Tanggal</th>
                        <th>Shift</th>
                        <th>Jam Kerja</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                $idKaryawan = $_SESSION['id_karyawan'];
                $hariList = ["Senin","Selasa","Rabu","Kamis","Jumat","Sabtu","Minggu"];
                $defaultShift = "Pagi";
                $defaultJamMulai = "08:00";
                $defaultJamSelesai = "16:00";
                $today = date('Y-m-d');
                $monday = date('Y-m-d', strtotime('monday this week', strtotime($today)));
                for ($i=0; $i<7; $i++) {
                    $tanggal = date('Y-m-d', strtotime("$monday +$i day"));
                    $jadwal = $conn->query("SELECT * FROM jadwal WHERE id_karyawan=$idKaryawan AND tanggal='$tanggal'")->fetch_assoc();
                    echo '<tr>';
                    echo '<td>'.$hariList[$i].'</td>';
                    echo '<td>'.date('d-m-Y', strtotime($tanggal)).'</td>';
                    if ($jadwal) {
                        echo '<td>'.ucfirst($jadwal['shift']).'</td>';
                        echo '<td>'.substr($jadwal['jam_mulai'],0,5).' - '.substr($jadwal['jam_selesai'],0,5).'</td>';
                    } else {
                        if ($i < 6) {
                            echo '<td>'.$defaultShift.'</td>';
                            echo '<td>'.$defaultJamMulai.' - '.$defaultJamSelesai.'</td>';
                        } else {
                            echo '<td>-</td>';
                            echo '<td>Libur</td>';
                        }
                    }
                    echo '</tr>';
                }
                ?>
                </tbody>
            </table>
            <div class="jadwal-info">* Jadwal default: Senin-Sabtu 08:00-16:00, Minggu Libur</div>
        </div>
    </div>
</main>
</body>
</html>
