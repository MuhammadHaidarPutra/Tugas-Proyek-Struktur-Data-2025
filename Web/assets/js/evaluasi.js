const modal = document.getElementById('formModal');

function showAddModal() {
    document.getElementById('modalTitle').textContent = 'Tambah Evaluasi';
    document.getElementById('evaluasiForm').reset();
    document.getElementById('evaluasiId').value = '';
    document.getElementById('formAction').value = 'create';
    document.getElementById('karyawanGroup').style.display = 'block';
    document.getElementById('periodeGroup').style.display = 'block';
    modal.classList.add('show');
}

function editEvaluasi(data) {
    document.getElementById('modalTitle').textContent = 'Edit Evaluasi';
    document.getElementById('evaluasiId').value = data.id;
    document.getElementById('formAction').value = 'update';
    document.getElementById('karyawanGroup').style.display = 'none';
    document.getElementById('periodeGroup').style.display = 'none';
    document.getElementById('nilai_kinerja').value = data.nilai_kinerja;
    document.getElementById('nilai_kedisiplinan').value = data.nilai_kedisiplinan;
    document.getElementById('nilai_kerjasama').value = data.nilai_kerjasama;
    document.getElementById('catatan').value = data.catatan || '';
    modal.classList.add('show');
}

function closeModal() {
    modal.classList.remove('show');
}

async function submitForm() {
    const form = document.getElementById('evaluasiForm');
    const formData = new FormData(form);
    
    try {
        const response = await fetch('evaluasi.php', {
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
