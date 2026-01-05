#ifndef MENUCUTI_H
#define MENUCUTI_H

#include <iostream>
#include <string>
#include "../modules/Cuti.h"
#include "../core/ManajemenKaryawan.h"

using namespace std;

// Menu untuk manajemen Cuti Karyawan
void menuCuti(ManajemenKaryawan& mk, Cuti& cuti, string namaAdmin) {
    int pilihan;
    bool kembali = false;
    
    while (!kembali) {
        // pastikan saldo up-to-date dengan data karyawan
        cuti.syncSaldos(mk);
        cout << "\n====== MENU MANAJEMEN CUTI KARYAWAN ======\n\n";
        
        // Admin menu: admin can only approve/reject and view requests/statistics
        cout << "1. Setujui Cuti\n";
        cout << "2. Tolak Cuti\n";
        cout << "3. Tampilkan Riwayat Cuti Karyawan\n";
        cout << "4. Tampilkan Semua Permintaan Cuti\n";
        cout << "5. Tampilkan Cuti Pending\n";
        cout << "6. Tampilkan Saldo Cuti Karyawan\n";
        cout << "7. Laporan Statistik Cuti\n";
        cout << "8. Kembali ke Menu Admin\n";
        
        cout << "\nPilih menu (1-9): ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                cout << "\n--- SETUJUI CUTI ---\n";
                int nomorCuti;
                string catatan;

                cout << "Masukkan Nomor Cuti: ";
                cin >> nomorCuti;
                cin.ignore();

                cout << "Masukkan Catatan Penyetujuan: ";
                getline(cin, catatan);

                cuti.setujuiCuti(nomorCuti, catatan);
                break;
            }

            case 2: {
                cout << "\n--- TOLAK CUTI ---\n";
                int nomorCuti;
                string alasan;

                cout << "Masukkan Nomor Cuti: ";
                cin >> nomorCuti;
                cin.ignore();

                cout << "Masukkan Alasan Penolakan: ";
                getline(cin, alasan);

                cuti.tolakCuti(nomorCuti, alasan);
                break;
            }

            case 3: {
                cout << "\n--- TAMPILKAN RIWAYAT CUTI KARYAWAN ---\n";
                int idKaryawan;

                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();

                cuti.tampilkanRiwayatCutiKaryawan(idKaryawan);
                break;
            }

            case 4: {
                cout << "\n";
                cuti.tampilkanSemuaPermintaanCuti();
                break;
            }

            case 5: {
                cout << "\n";
                cuti.tampilkanCutiPending();
                break;
            }

            case 6: {
                cout << "\n--- TAMPILKAN SALDO CUTI KARYAWAN ---\n";
                int idKaryawan;

                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();

                cuti.tampilkanSaldoCutiKaryawan(idKaryawan);
                break;
            }

            case 7: {
                cout << "\n";
                cuti.laporanStatistikCuti();
                break;
            }

            case 8: {
                kembali = true;
                cout << "\nKembali ke Menu Admin.\n";
                break;
            }
            
            default: {
                cout << "Pilihan tidak valid!\n";
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
