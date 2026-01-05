#ifndef MENU_LAPORAN_H
#define MENU_LAPORAN_H

#include <iostream>
#include <string>
#include "../modules/Laporan.h"
#include "../core/ManajemenKaryawan.h"

using namespace std;

void menuLaporan(ManajemenKaryawan &mk, Laporan &laporan, string namaAdmin) {
    int pilihan;
    do {
        cout << "\n";
        cout << "+============================================+\n";
        cout << "|             MENU LAPORAN & STATISTIK       |\n";
        cout << "+============================================+\n";
        cout << "1. Generate Laporan (dari data absensi)\n";
        cout << "2. Tampilkan Rekap Absensi Per Karyawan\n";
        cout << "3. Tampilkan Rata-Rata Jam Kerja\n";
        cout << "4. Tampilkan Laporan Lengkap\n";
        cout << "5. Tampilkan Detail Statistik Karyawan\n";
        cout << "6. Kembali ke Menu Utama\n";
        cout << "+============================================+\n";
        cout << "Pilih: ";
        cin >> pilihan;
        
        switch(pilihan) {
            case 1: {
                cout << "\nMenghasilkan laporan dari data absensi...\n";
                // Akses private members melalui getter functions
                // Untuk ini kita perlu menambahkan getter di ManajemenKaryawan
                laporan.generateLaporan(mk.getHeadAbsensi(), mk.getDataKaryawan(), mk.getUkuran());
                mk.tambahLog(namaAdmin, "Generate Laporan");
                mk.pushActivity("Generate Laporan");
                break;
            }
            case 2:
                laporan.tampilkanRekapAbsensi();
                break;
            
            case 3:
                laporan.tampilkanRataRataJamKerja();
                break;
            
            case 4:
                laporan.tampilkanLaporanLengkap();
                break;
            
            case 5: {
                int idKaryawan;
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                laporan.tampilkanDetailKaryawan(idKaryawan);
                break;
            }
            
            case 6:
                cout << "Kembali ke menu utama.\n";
                break;
            
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 6);
}

#endif // MENU_LAPORAN_H
