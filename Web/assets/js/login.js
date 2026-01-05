document.getElementById('loginForm').addEventListener('submit', async function(e) {
    e.preventDefault();
    
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    const alertContainer = document.getElementById('alert-container');
    
    // Clear previous alerts
    alertContainer.innerHTML = '';
    
    try {
        const formData = new FormData();
        formData.append('username', username);
        formData.append('password', password);
        
        const response = await fetch('login.php', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (data.success) {
            showAlert('success', data.message);
            setTimeout(() => {
                window.location.href = data.data.redirect;
            }, 1000);
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Terjadi kesalahan sistem!');
    }
});

function showAlert(type, message) {
    const alertContainer = document.getElementById('alert-container');
    const alertClass = type === 'success' ? 'alert-success' : 'alert-error';
    const icon = type === 'success' ? 'fa-check-circle' : 'fa-exclamation-circle';
    
    alertContainer.innerHTML = `
        <div class="alert ${alertClass}">
            <i class="fas ${icon}"></i> ${message}
        </div>
    `;
    // Tambahan: scroll ke alert
    alertContainer.scrollIntoView({behavior: 'smooth'});
}

// Modal Daftar Akun
const registerModal = document.getElementById('registerModal');
function showRegisterModal() {
    registerModal.classList.add('show');
}
function closeRegisterModal() {
    registerModal.classList.remove('show');
}

async function submitRegister() {
    const form = document.getElementById('registerForm');
    const formData = new FormData(form);
    try {
        const response = await fetch('register.php', {
            method: 'POST',
            body: formData
        });
        const data = await response.json();
        if (data.success) {
            showAlert('success', data.message);
            closeRegisterModal();
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Gagal daftar akun!');
    }
}

// Tutup modal jika klik di luar
window.onclick = function(event) {
    if (event.target === registerModal) closeRegisterModal();
}
