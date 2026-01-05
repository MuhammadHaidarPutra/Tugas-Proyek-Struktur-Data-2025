const modal = document.getElementById('formModal');

function showAddModal() {
    document.getElementById('modalTitle').textContent = 'Tambah Absensi';
    document.getElementById('absensiForm').reset();
    document.getElementById('absensiId').value = '';
    document.getElementById('formAction').value = 'create';
    document.getElementById('karyawanGroup').style.display = 'block';
    document.getElementById('tanggal').value = new Date().toISOString().split('T')[0];
    modal.classList.add('show');
}

function editAbsensi(data) {
    document.getElementById('modalTitle').textContent = 'Edit Absensi';
    document.getElementById('absensiId').value = data.id;
    document.getElementById('formAction').value = 'update';
    document.getElementById('karyawanGroup').style.display = 'none';
    document.getElementById('jam_masuk').value = data.jam_masuk || '';
    document.getElementById('jam_keluar').value = data.jam_keluar || '';
    document.getElementById('status').value = data.status;
    document.getElementById('keterangan').value = data.keterangan || '';
    modal.classList.add('show');
}

function closeModal() {
    modal.classList.remove('show');
}

async function submitForm() {
    const form = document.getElementById('absensiForm');
    const formData = new FormData(form);
    
    try {
        const response = await fetch('absensi.php', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (data.success) {
            showAlert('success', data.message);
            setTimeout(() => location.reload(), 1500);
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Terjadi kesalahan sistem!');
    }
}

async function deleteAbsensi(id) {
    if (!confirm('Hapus data absensi ini?')) return;
    
    const formData = new FormData();
    formData.append('action', 'delete');
    formData.append('id', id);
    
    try {
        const response = await fetch('absensi.php', {
            method: 'POST',
            body: formData
        });
        
        const data = await response.json();
        
        if (data.success) {
            showAlert('success', data.message);
            setTimeout(() => location.reload(), 1500);
        } else {
            showAlert('error', data.message);
        }
    } catch (error) {
        showAlert('error', 'Terjadi kesalahan sistem!');
    }
}

function filterByDate() {
    const tanggal = document.getElementById('filterTanggal').value;
    location.href = 'absensi.php?tanggal=' + tanggal;
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
    
    setTimeout(() => alertContainer.innerHTML = '', 5000);
}

window.onclick = function(event) {
    if (event.target === modal) closeModal();
}
