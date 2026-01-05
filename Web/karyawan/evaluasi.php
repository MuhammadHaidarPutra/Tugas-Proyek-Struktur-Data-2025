<?php
require_once '../config.php';
if (!isLoggedIn() || isAdmin()) redirect('../index.php');
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Evaluasi Saya - Karyawan</title>
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
    .eval-card {
        background: #fff;
        border-radius: 1.2rem;
        box-shadow: 0 8px 32px rgba(37,99,235,0.13);
        padding: 2.5rem 2rem 2rem 2rem;
        margin-bottom: 2rem;
        width: 100%;
        max-width: 800px;
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    .eval-card h2 {
        color: #2563eb;
        margin-bottom: 1.5rem;
        text-align: center;
    }
    .eval-table {
        width: 100%;
        border-collapse: separate;
        border-spacing: 0;
        background: #fff;
        border-radius: 1rem;
        overflow: hidden;
        box-shadow: 0 2px 16px rgba(37,99,235,0.10);
    }
    .eval-table thead {
        background: linear-gradient(90deg,#2563eb 60%,#4f8cff 100%);
        color: #fff;
    }
    .eval-table th, .eval-table td {
        padding: 14px 10px;
        font-size: 1.05rem;
        text-align: center;
    }
    .eval-table tbody tr {
        border-bottom: 1px solid #e5e7eb;
    }
    .eval-table tbody tr:last-child {
        border-bottom: none;
    }
    .eval-table td:first-child {
        font-weight: 600;
        color: #2563eb;
    }
    @media (max-width: 900px) {
        .eval-card { padding: 1.2rem 0.2rem; }
        .eval-table th, .eval-table td { padding: 8px 4px; font-size: 0.98rem; }
    }
    </style>
</head>
<body>
<?php include 'sidebar.php'; ?>
<main class="main-content">
    <?php include 'topbar.php'; ?>
    <div class="container">
        <div class="eval-card">
            <h2>Evaluasi Kinerja Saya</h2>
            <table class="eval-table">
                <thead>
                    <tr>
                        <th>Periode</th>
                        <th>Nilai Kinerja</th>
                        <th>Kedisiplinan</th>
                        <th>Kerjasama</th>
                        <th>Catatan</th>
                        <th>Evaluator</th>
                    </tr>
                </thead>
                <tbody>
                <?php
                $idKaryawan = $_SESSION['id_karyawan'];
                $evaluasi = $conn->query("SELECT * FROM evaluasi WHERE id_karyawan = $idKaryawan ORDER BY tanggal_evaluasi DESC");
                if ($evaluasi && $evaluasi->num_rows > 0) {
                    while ($row = $evaluasi->fetch_assoc()) {
                        echo '<tr>';
                        echo '<td>'.htmlspecialchars($row['periode']).'</td>';
                        echo '<td>'.htmlspecialchars($row['nilai_kinerja']).'</td>';
                        echo '<td>'.htmlspecialchars($row['nilai_kedisiplinan']).'</td>';
                        echo '<td>'.htmlspecialchars($row['nilai_kerjasama']).'</td>';
                        echo '<td>'.htmlspecialchars($row['catatan']).'</td>';
                        echo '<td>'.htmlspecialchars($row['evaluator']).'</td>';
                        echo '</tr>';
                    }
                } else {
                    echo '<tr><td colspan="6" style="text-align:center;">Belum ada data evaluasi</td></tr>';
                }
                ?>
                </tbody>
            </table>
        </div>
    </div>
</main>
</body>
</html>
