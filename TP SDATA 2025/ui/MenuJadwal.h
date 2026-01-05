#ifndef MENUJADWAL_H
#define MENUJADWAL_H

#include <iostream>
#include <string>
#include "../modules/Jadwal.h"
#include "../core/ManajemenKaryawan.h"

using namespace std;

// Menu untuk manajemen Jadwal Kerja
void menuJadwal(ManajemenKaryawan& mk, Jadwal& jadwal, string namaAdmin) {
    int pilihan;
    bool kembali = false;
    // Terapkan jadwal statis (sama untuk semua karyawan) saat masuk menu Jadwal
    JamKerja defaultWeek[7] = {
        JamKerja("Senin", "08:00", "16:00"),
        JamKerja("Selasa", "08:00", "16:00"),
        JamKerja("Rabu", "08:00", "16:00"),
        JamKerja("Kamis", "08:00", "16:00"),
        JamKerja("Jumat", "08:00", "16:00"),
        JamKerja("Sabtu", "", ""),
        JamKerja("Minggu", "", "")
    };
    jadwal.setJadwalStatisUntukSemua(mk, "Pagi", defaultWeek);
    
    while (!kembali) {
        cout << "\n+==========================================================================+\n";
        cout << "|                MENU MANAJEMEN JADWAL KERJA KARYAWAN                     |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "1. Tambah Jadwal Karyawan\n";
        cout << "2. Set Jadwal Harian\n";
        cout << "3. Set Shift Kerja\n";
        cout << "4. Tampilkan Jadwal Individual\n";
        cout << "5. Tampilkan Semua Jadwal\n";
        cout << "6. Tampilkan Jadwal Per Shift\n";
        cout << "7. Ubah Status Jadwal (Aktif/Tidak Aktif)\n";
        cout << "8. Hitung Total Jam Kerja Mingguan\n";
        cout << "9. Laporan Distribusi Shift Kerja\n";
        cout << "10. Hapus Jadwal Karyawan\n";
        cout << "11. Kembali ke Menu Admin\n";
        
        cout << "\nPilih menu (1-11): ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                cout << "\n--- TAMBAH JADWAL KARYAWAN ---\n";
                int idKaryawan;
                string shift;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                // Cari data karyawan
                Karyawan** dataKaryawan = mk.getDataKaryawan();
                string namaKaryawan = "Unknown";
                string departemen = "Unknown";
                
                for (int i = 0; i < mk.getUkuran(); i++) {
                    if (dataKaryawan[i] && dataKaryawan[i]->getId() == idKaryawan) {
                        namaKaryawan = dataKaryawan[i]->getNama();
                        departemen = dataKaryawan[i]->getDepartemen();
                        break;
                    }
                }
                
                cout << "Masukkan Shift Kerja (Pagi/Siang/Malam): ";
                getline(cin, shift);
                
                jadwal.tambahJadwal(idKaryawan, namaKaryawan, departemen, shift);
                break;
            }
            
            case 2: {
                cout << "\n--- SET JADWAL HARIAN ---\n";
                int idKaryawan, hariKe;
                string hari, jamMulai, jamSelesai;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Hari Ke (1-7): ";
                cin >> hariKe;
                cin.ignore();
                
                cout << "Masukkan Nama Hari (Senin/Selasa/...): ";
                getline(cin, hari);
                
                cout << "Masukkan Jam Mulai (HH:MM): ";
                getline(cin, jamMulai);
                
                cout << "Masukkan Jam Selesai (HH:MM): ";
                getline(cin, jamSelesai);
                
                jadwal.setJadwalHari(idKaryawan, hariKe, hari, jamMulai, jamSelesai);
                break;
            }
            
            case 3: {
                cout << "\n--- SET SHIFT KERJA ---\n";
                int idKaryawan;
                string shift;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Shift (Pagi/Siang/Malam): ";
                getline(cin, shift);
                
                jadwal.setShift(idKaryawan, shift);
                break;
            }
            
            case 4: {
                cout << "\n--- TAMPILKAN JADWAL INDIVIDUAL ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                jadwal.tampilkanJadwalIndividu(idKaryawan);
                break;
            }
            
            case 5: {
                cout << "\n";
                jadwal.tampilkanSemuaJadwal();
                break;
            }
            
            case 6: {
                cout << "\n--- TAMPILKAN JADWAL PER SHIFT ---\n";
                string shift;
                
                cout << "Masukkan Shift (Pagi/Siang/Malam): ";
                getline(cin, shift);
                
                jadwal.tampilkanJadwalPerShift(shift);
                break;
            }
            
            case 7: {
                cout << "\n--- UBAH STATUS JADWAL ---\n";
                int idKaryawan;
                string status;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Status Baru (Aktif/Tidak Aktif): ";
                getline(cin, status);
                
                jadwal.ubahStatus(idKaryawan, status);
                break;
            }
            
            case 8: {
                cout << "\n--- HITUNG TOTAL JAM KERJA MINGGUAN ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                jadwal.hitungTotalJamMinggu(idKaryawan);
                break;
            }
            
            case 9: {
                cout << "\n";
                jadwal.laporanDistribusiShift();
                break;
            }
            
            case 10: {
                cout << "\n--- HAPUS JADWAL KARYAWAN ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                jadwal.hapusJadwal(idKaryawan);
                break;
            }
            
            
            
            case 12: {
                kembali = true;
                cout << "\n✓ Kembali ke Menu Admin.\n";
                break;
            }
            
            default: {
                cout << "✗ Pilihan tidak valid!\n";
                break;
            }
        }
        
        if (!kembali) {
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.ignore();
        }
    }
}

#endif
