<div class="topbar">
    <div class="topbar-left">
        <h1><?php 
            $page = basename($_SERVER['PHP_SELF'], '.php');
            $titles = [
                'dashboard' => 'Dashboard',
                'karyawan' => 'Data Karyawan',
                'absensi' => 'Manajemen Absensi',
                'cuti' => 'Manajemen Cuti',
                'jadwal' => 'Jadwal Kerja',
                'payroll' => 'Payroll',
                'evaluasi' => 'Evaluasi Karyawan',
                'laporan' => 'Laporan',
                'users' => 'Manajemen User'
            ];
            echo $titles[$page] ?? 'Admin Panel';
        ?></h1>
    </div>
    <div class="topbar-right">
        <div class="user-info">
            <div class="user-avatar"><?php echo strtoupper(substr($_SESSION['nama'], 0, 1)); ?></div>
            <div class="user-details">
                <strong><?php echo $_SESSION['nama']; ?></strong>
                <small>Administrator</small>
            </div>
        </div>
    </div>
</div>
