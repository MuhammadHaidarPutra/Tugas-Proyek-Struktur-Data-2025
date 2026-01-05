<?php
require_once 'config.php';

// Cek login
if (!isLoggedIn()) {
    redirect('../index.php');
}

// Logout
session_destroy();
redirect('../index.php');
?>
