<?php
require_once '../config.php';
if (!isLoggedIn() || isAdmin()) redirect('../index.php');
?>
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gaji Saya - Karyawan</title>
    <link rel="stylesheet" href="../assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body>
<?php include 'sidebar.php'; ?>
<main class="main-content">
    <?php include 'topbar.php'; ?>
    <div class="container">
        <h2>Gaji Saya</h2>
        <p>Fitur gaji karyawan akan ditampilkan di sini.</p>
    </div>
</main>
</body>
</html>
