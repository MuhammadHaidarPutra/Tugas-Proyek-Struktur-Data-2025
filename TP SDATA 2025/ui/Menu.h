#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include "../core/User.h"
#include "../core/ManajemenKaryawan.h"
#include "../modules/Cuti.h"

using namespace std;

// Note: `login(...)` is implemented centrally in `main.cpp` to avoid duplicate
// definitions and maintain a single login flow. Do not re-declare it here.

void menuAdmin(ManajemenKaryawan &mk, string namaAdmin) {
    int pilihan;
    do {
        cout << "+============================================+\n";
        cout << "|              SELAMAT DATANG DI             |\n";
        cout << "|                 MENU ADMIN                 |\n";
        cout << "+============================================+\n";
        cout << "\n--- Manajemen Karyawan---\n";
        cout << "1. Tambah karyawan\n";
        cout << "2. Tampilkan karyawan\n";
        cout << "3. Ubah karyawan\n";
        cout << "4. Hapus karyawan\n";
        cout << " \n--- Manajemen Absensi ---\n";
        cout << "5. Tampilkan semua absensi\n";
        cout << "6. Tambah/Koreksi absensi\n";
        cout << "7. Ubah absensi \n";
        cout << "8. Hapus absensi \n";
        cout << "\n--- Log Aktivitas\n";
        cout << "9. Tampilkan Log (Terbaru di Atas)\n";
        cout << "10. Tampilkan Log (Terbaru di Bawah)\n";
        cout << "11. Logout\n";
        cout << "12. Lihat Riwayat Aktivitas\n";
        cout << "13. Undo Aktivitas Terakhir\n";
        cout << "Pilih: ";
        cin >> pilihan;

        int id;
        string nama, jabatan, departemen;
        string tanggal, jamMasuk, jamKeluar;
        switch(pilihan) {
            case 1:
                cout << "ID: "; cin >> id;
                cout << "Nama: "; cin >> ws; getline(cin, nama);
                cout << "Jabatan: "; cin >> ws; getline(cin, jabatan);
                cout << "Departemen: "; cin >> ws; getline(cin, departemen);
                mk.tambahKaryawan(id, nama, jabatan, departemen);

                mk.tambahLog(namaAdmin, "Tambah Karyawan (ID: " + to_string(id) + ")");
                mk.pushActivity("Tambah Karyawan ID " + to_string(id));
                break;

            case 2:
                mk.tampilkan();
                break;

            case 3:
                cout << "Masukkan ID yang ingin diubah: "; cin >> id;
                mk.ubah(id);

                mk.tambahLog(namaAdmin, "Ubah Karyawan (ID: " + to_string(id) + ")");
                mk.pushActivity("Ubah Karyawan ID " + to_string(id));
                break;

            case 4:
                cout << "Masukkan ID yang ingin dihapus: "; cin >> id;
                mk.hapus(id);

                mk.tambahLog(namaAdmin, "Hapus Karyawan (ID: " + to_string(id) + ")");
                mk.pushActivity("Hapus Karyawan ID " + to_string(id));
                break;

            case 5:
                mk.tampilAbsensi();
                break;

            case 6:
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (YYYY-MM-DD): "; cin >> tanggal;
                cout << "Jam Masuk: "; cin >> jamMasuk;
                cout << "Jam Keluar: "; cin >> jamKeluar;

                mk.tambahAbsensi(id, tanggal, jamMasuk, jamKeluar);
                mk.tambahLog(namaAdmin, "Tambah Absensi (ID: " + to_string(id) + ")");
                mk.pushActivity("Tambah Absensi ID " + to_string(id));
                break;

            case 7:
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (YYYY-MM-DD): "; cin >> tanggal;
                mk.ubahAbsensi(id, tanggal);
                mk.tambahLog(namaAdmin, "Ubah Absensi (ID: " + to_string(id) + " Tgl: " + tanggal + ")");
                mk.pushActivity("Ubah Absensi ID " + to_string(id) + " Tgl " + tanggal);
                break;

            case 8:
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (YYYY-MM-DD): "; cin >> tanggal;
                mk.hapusAbsensi(id, tanggal);
                mk.tambahLog(namaAdmin, "Hapus Absensi (ID: " + to_string(id) + " Tgl: " + tanggal + ")");
                mk.pushActivity("Hapus Absensi ID " + to_string(id) + " Tgl " + tanggal);
                break;

            case 9:
                mk.tampilkanLogMundur();
                break;

            case 10:
                mk.tampilkanLogMaju();
                break;

            case 11:
                cout << "Logout berhasil.\n";
                break;

            case 12:
                mk.tampilActivity();
                break;

            case 13:
                mk.popActivity();
                break;

            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 11);
}

void menuKaryawan(string namaKaryawan, int idKaryawan, ManajemenKaryawan &mk, Cuti &cuti) {
    int pilihan;
    string tanggal, jam;
    do {
        cout << "\n=== MENU KARYAWAN ===\n";
        cout << "1. Check-in" << endl;
        cout << "2. Check-out" << endl;
        cout << "3. Lihat absensi" << endl;
        cout << "4. Permintaan Cuti" << endl;
        cout << "5. Logout" << endl;
        cout << "Pilih: ";
        cin >> pilihan;
        switch(pilihan) {
            case 1:
                cout << "Tanggal (YYYY-MM-DD): "; cin >> tanggal;
                cout << "Jam masuk (HH:MM): "; cin >> jam;
                mk.checkIn(idKaryawan, tanggal, jam);
                break;
            case 2:
                cout << "Tanggal (YYYY-MM-DD): "; cin >> tanggal;
                cout << "Jam keluar (HH:MM): "; cin >> jam;
                mk.checkOut(idKaryawan, tanggal, jam);
                break;
            case 3:
                mk.tampilAbsensi();
                break;
            case 4: {
                // Buat permintaan cuti oleh karyawan yang sedang login
                cuti.syncSaldos(mk);
                cout << "\n--- BUAT PERMINTAAN CUTI ---\n";
                string jenisCuti, tanggalMulai, tanggalSelesai, alasan;
                int jumlahHari;

                cout << "Masukkan Jenis Cuti (Tahunan/Sakit/Pribadi/Khusus): "; cin >> ws; getline(cin, jenisCuti);
                cout << "Masukkan Tanggal Mulai (DD-MM-YYYY): "; getline(cin, tanggalMulai);
                cout << "Masukkan Tanggal Selesai (DD-MM-YYYY): "; getline(cin, tanggalSelesai);
                cout << "Masukkan Jumlah Hari: "; cin >> jumlahHari; cin.ignore();
                cout << "Masukkan Alasan: "; getline(cin, alasan);

                cuti.buatPermintaanCuti(idKaryawan, namaKaryawan, jenisCuti, tanggalMulai, tanggalSelesai, jumlahHari, alasan);
                break;
            }
            case 5:
                cout << "Logout berhasil.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 5);
}

#endif // MENU_H
