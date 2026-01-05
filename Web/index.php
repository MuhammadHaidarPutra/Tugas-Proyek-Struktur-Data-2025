<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login - Sistem Manajemen Karyawan</title>
    <link rel="stylesheet" href="assets/css/style.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
</head>
<body class="login-page">
    <div class="login-container">
        <div class="login-box">
            <div class="login-header">
                <i class="fas fa-users-cog"></i>
                <h1>Sistem Manajemen Karyawan</h1>
                <p>Silakan login untuk melanjutkan</p>
            </div>
            
            <div id="alert-container"></div>
            
            <form id="loginForm" class="login-form">
                <div class="form-group">
                    <label for="username">
                        <i class="fas fa-user"></i> Username
                    </label>
                    <input type="text" id="username" name="username" required autocomplete="username">
                </div>
                
                <div class="form-group" style="position:relative;">
                    <label for="password">
                        <i class="fas fa-lock"></i> Password
                    </label>
                    <input type="password" id="password" name="password" required autocomplete="current-password" style="padding-right:40px;">
                    <span class="toggle-password" onclick="togglePassword('password', this)" style="position:absolute;top:38px;right:16px;cursor:pointer;">
                        <i class="fas fa-eye"></i>
                    </span>
                </div>
                
                <div class="form-group">
                    <button type="submit" class="btn btn-primary btn-block">
                        <i class="fas fa-sign-in-alt"></i> Login
                    </button>
                </div>
                <div class="form-group">
                    <button type="button" class="btn btn-secondary btn-block" onclick="showRegisterModal()">
                        <i class="fas fa-user-plus"></i> Daftar Akun Baru
                    </button>
                </div>
            </form>
            
            <div class="login-footer">
        
            </div>
        </div>
    </div>

    <!-- Modal Daftar Akun Baru -->
    <div id="registerModal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h3>Daftar Akun Baru</h3>
                <button class="modal-close" onclick="closeRegisterModal()">&times;</button>
            </div>
            <div class="modal-body">
                <form id="registerForm">
                    <div class="form-group">
                        <label for="reg_nama">Nama Lengkap <span style="color: red;">*</span></label>
                        <input type="text" id="reg_nama" name="nama" required>
                    </div>
                    <div class="form-group">
                        <label for="reg_username">Username <span style="color: red;">*</span></label>
                        <input type="text" id="reg_username" name="username" required>
                    </div>
                    <div class="form-group" style="position:relative;">
                        <label for="reg_password">Password <span style="color: red;">*</span></label>
                        <input type="password" id="reg_password" name="password" required style="padding-right:40px;">
                        <span class="toggle-password" onclick="togglePassword('reg_password', this)" style="position:absolute;top:38px;right:16px;cursor:pointer;">
                            <i class="fas fa-eye"></i>
                        </span>
                    </div>
                    <div class="form-group">
                        <label for="reg_role">Role <span style="color: red;">*</span></label>
                        <select id="reg_role" name="role" required>
                            <option value="karyawan">Karyawan</option>
                            <option value="admin">Admin</option>
                        </select>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" onclick="closeRegisterModal()">Batal</button>
                <button class="btn btn-primary" onclick="submitRegister()">Daftar</button>
            </div>
        </div>
    </div>
        </div>
    </div>
    
    <script src="assets/js/login.js"></script>
    <script>
    function togglePassword(inputId, el) {
        const input = document.getElementById(inputId);
        const icon = el.querySelector('i');
        if (input.type === 'password') {
            input.type = 'text';
            icon.classList.remove('fa-eye');
            icon.classList.add('fa-eye-slash');
        } else {
            input.type = 'password';
            icon.classList.remove('fa-eye-slash');
            icon.classList.add('fa-eye');
        }
    }
    </script>
</body>
</html>
