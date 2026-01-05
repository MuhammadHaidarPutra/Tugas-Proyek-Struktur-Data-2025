// Karyawan Management
const modal = document.getElementById('formModal');

function showAddModal() {
    document.getElementById('modalTitle').textContent = 'Tambah Karyawan';
    document.getElementById('karyawanForm').reset();
    document.getElementById('karyawanId').value = '';
    document.getElementById('formAction').value = 'create';
    document.getElementById('statusGroup').style.display = 'none';
    modal.classList.add('show');
}

function editKaryawan(data) {
    document.getElementById('modalTitle').textContent = 'Edit Karyawan';
    document.getElementById('karyawanId').value = data.id;
    document.getElementById('formAction').value = 'update';
    document.getElementById('nama').value = data.nama;
    document.getElementById('jabatan').value = data.jabatan;
    document.getElementById('departemen').value = data.departemen;
    document.getElementById('email').value = data.email || '';
    document.getElementById('telepon').value = data.telepon || '';
    document.getElementById('gaji_pokok').value = data.gaji_pokok;
    document.getElementById('status').value = data.status;
    document.getElementById('statusGroup').style.display = 'block';
    modal.classList.add('show');
}

function closeModal() {
    modal.classList.remove('show');
}

async function submitForm() {
    const form = document.getElementById('karyawanForm');
    const formData = new FormData(form);
    
    try {
        const response = await fetch('karyawan.php', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (data.success) {
            showAlert('success', data.message);
            setTimeout(() => {
                location.reload();
            }, 1500);
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Terjadi kesalahan sistem!');
    }
}

async function deleteKaryawan(id) {
    if (!confirm('Apakah Anda yakin ingin menghapus karyawan ini?')) {
        return;
    }
    
    const formData = new FormData();
    formData.append('action', 'delete');
    formData.append('id', id);
    
    try {
        const response = await fetch('karyawan.php', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (data.success) {
            showAlert('success', data.message);
            setTimeout(() => {
                location.reload();
            }, 1500);
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Terjadi kesalahan sistem!');
    }
}

function showAlert(type, message) {
    const alertContainer = document.getElementById('alert-container');
    const alertClass = type === 'success' ? 'alert-success' : 'alert-error';
    const icon = type === 'success' ? 'fa-check-circle' : 'fa-exclamation-circle';
    
    alertContainer.innerHTML = `
        <div class="alert ${alertClass}">
            <i class="fas ${icon}"></i> ${message}
        </div>
    `;
    
    setTimeout(() => {
        alertContainer.innerHTML = '';
    }, 5000);
}

// Search functionality
document.getElementById('searchInput')?.addEventListener('input', function(e) {
    const searchTerm = e.target.value.toLowerCase();
    const rows = document.querySelectorAll('#karyawanTable tr');
    
    rows.forEach(row => {
        const text = row.textContent.toLowerCase();
        row.style.display = text.includes(searchTerm) ? '' : 'none';
    });
});

// Close modal when clicking outside
window.onclick = function(event) {
    if (event.target === modal) {
        closeModal();
    }
}
