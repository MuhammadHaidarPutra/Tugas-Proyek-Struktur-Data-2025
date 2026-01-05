<?php
require_once '../config.php';
if (!isLoggedIn() || isAdmin()) redirect('../index.php');
$idKaryawan = $_SESSION['id_karyawan'];
// Ambil data slip gaji dari tabel payroll
$payrolls = $conn->query("SELECT * FROM payroll WHERE id_karyawan = $idKaryawan ORDER BY tahun DESC, bulan DESC");
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Slip Gaji Saya - Karyawan</title>
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
    .slip-card {
        background: #fff;
        border-radius: 1.2rem;
        box-shadow: 0 8px 32px rgba(37,99,235,0.13);
        padding: 2.5rem 2rem 2rem 2rem;
        margin-bottom: 2rem;
        width: 100%;
        max-width: 700px;
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    .slip-card h2 {
        color: #2563eb;
        margin-bottom: 1.5rem;
        text-align: center;
    }
    .slip-table {
        width: 100%;
        border-collapse: separate;
        border-spacing: 0;
        background: #fff;
        border-radius: 1rem;
        overflow: hidden;
        box-shadow: 0 2px 16px rgba(37,99,235,0.10);
    }
    .slip-table thead {
        background: linear-gradient(90deg,#2563eb 60%,#4f8cff 100%);
        color: #fff;
    }
    .slip-table th, .slip-table td {
        padding: 14px 10px;
        font-size: 1.05rem;
        text-align: center;
    }
    .slip-table tbody tr {
        border-bottom: 1px solid #e5e7eb;
    }
    .slip-table tbody tr:last-child {
        border-bottom: none;
    }
    .slip-table td:first-child {
        font-weight: 600;
        color: #2563eb;
    }
    @media (max-width: 800px) {
        .slip-card { padding: 1.2rem 0.2rem; }
        .slip-table th, .slip-table td { padding: 8px 4px; font-size: 0.98rem; }
    }
    </style>
</head>
<body>
<?php include 'sidebar.php'; ?>
<main class="main-content">
    <?php include 'topbar.php'; ?>
    <div class="container">
        <div class="slip-card">
            <h2>Slip Gaji Saya</h2>
            <table class="slip-table">
                <thead>
                    <tr>
                        <th>Bulan</th>
                        <th>Tahun</th>
                        <th>Gaji Pokok</th>
                        <th>Tunjangan</th>
                        <th>Potongan</th>
                        <th>Total Gaji</th>
                        <th>Status</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                if ($payrolls && $payrolls->num_rows > 0) {
                    while ($row = $payrolls->fetch_assoc()) {
                        echo '<tr>';
                        echo '<td>'.date('F', mktime(0,0,0,$row['bulan'],1)).'</td>';
                        echo '<td>'.$row['tahun'].'</td>';
                        echo '<td>Rp '.number_format($row['gaji_pokok'],0,',','.').'</td>';
                        echo '<td>Rp '.number_format($row['tunjangan'],0,',','.').'</td>';
                        echo '<td>Rp '.number_format($row['potongan'],0,',','.').'</td>';
                        echo '<td>Rp '.number_format($row['total_gaji'],0,',','.').'</td>';
                        echo '<td>'.ucfirst($row['status']).'</td>';
                        echo '</tr>';
                    }
                } else {
                    echo '<tr><td colspan="7">Belum ada data slip gaji</td></tr>';
                }
                ?>
                </tbody>
            </table>
        </div>
    </div>
</main>
</body>
</html>
