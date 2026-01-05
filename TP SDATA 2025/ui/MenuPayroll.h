#ifndef MENUPAYROLL_H
#define MENUPAYROLL_H

#include <iostream>
#include <string>
#include "../modules/Payroll.h"
#include "../core/ManajemenKaryawan.h"

using namespace std;

// Menu untuk manajemen Payroll
void menuPayroll(ManajemenKaryawan& mk, Payroll& payroll, string namaAdmin) {
    int pilihan;
    bool kembali = false;
    
    while (!kembali) {
        cout << "\n+==========================================================================+\n";
        cout << "|                  MENU MANAJEMEN GAJI & PAYROLL                           |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "1. Tambah Data Gaji Karyawan\n";
        cout << "2. Tambah Tunjangan\n";
        cout << "3. Tambah Potongan\n";
        cout << "4. Tandai Gaji Sudah Lunas\n";
        cout << "5. Tampilkan Slip Gaji Individual\n";
        cout << "6. Tampilkan Laporan Gaji Semua Karyawan\n";
        cout << "7. Tampilkan Ringkasan Gaji\n";
        cout << "8. Filter Gaji Berdasarkan Status\n";
        cout << "9. Ubah Status Pembayaran Gaji\n";
        cout << "10. Hapus Data Gaji\n";
        cout << "11. Kembali ke Menu Admin\n";
        
        cout << "\nPilih menu (1-11): ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                cout << "\n--- TAMBAH DATA GAJI KARYAWAN ---\n";
                int idKaryawan;
                double gajiPokok;
                string bulanTahun;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Bulan/Tahun (MM-YYYY): ";
                getline(cin, bulanTahun);
                
                cout << "Masukkan Gaji Pokok (Rp): ";
                cin >> gajiPokok;
                cin.ignore();
                
                // Cari nama karyawan dari ManajemenKaryawan
                Karyawan** dataKaryawan = mk.getDataKaryawan();
                string namaKaryawan = "Unknown";
                for (int i = 0; i < mk.getUkuran(); i++) {
                    if (dataKaryawan[i] && dataKaryawan[i]->getId() == idKaryawan) {
                        namaKaryawan = dataKaryawan[i]->getNama();
                        break;
                    }
                }
                
                payroll.tambahDataGaji(idKaryawan, namaKaryawan, bulanTahun, gajiPokok);
                break;
            }
            
            case 2: {
                cout << "\n--- TAMBAH TUNJANGAN ---\n";
                int idKaryawan;
                double nominal;
                string namaTunjangan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Nama Tunjangan (misal: Tunjangan Makan): ";
                getline(cin, namaTunjangan);
                
                cout << "Masukkan Nominal Tunjangan (Rp): ";
                cin >> nominal;
                cin.ignore();
                
                payroll.tambahTunjangan(idKaryawan, namaTunjangan, nominal);
                break;
            }
            
            case 3: {
                cout << "\n--- TAMBAH POTONGAN ---\n";
                int idKaryawan;
                double nominal;
                string namaPotongan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Nama Potongan (misal: Potongan Pajak): ";
                getline(cin, namaPotongan);
                
                cout << "Masukkan Nominal Potongan (Rp): ";
                cin >> nominal;
                cin.ignore();
                
                payroll.tambahPotongan(idKaryawan, namaPotongan, nominal);
                break;
            }
            
            case 4: {
                cout << "\n--- TANDAI GAJI SUDAH LUNAS ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                payroll.tandaiLunas(idKaryawan);
                break;
            }
            
            case 5: {
                cout << "\n--- TAMPILKAN SLIP GAJI INDIVIDUAL ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                payroll.tampilkanSlipGaji(idKaryawan);
                break;
            }
            
            case 6: {
                cout << "\n";
                payroll.tampilkanLaporanGaji();
                break;
            }
            
            case 7: {
                cout << "\n";
                payroll.tampilkanRingkasanGaji();
                break;
            }
            
            case 8: {
                cout << "\n--- FILTER GAJI BERDASARKAN STATUS ---\n";
                string status;
                
                cout << "Masukkan Status (Lunas/Belum): ";
                getline(cin, status);
                
                payroll.filterGajiByStatus(status);
                break;
            }
            
            case 9: {
                cout << "\n--- UBAH STATUS PEMBAYARAN GAJI ---\n";
                int idKaryawan;
                string statusBaru;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Status Baru (Lunas/Belum): ";
                getline(cin, statusBaru);
                
                if (statusBaru == "Lunas") {
                    payroll.tandaiLunas(idKaryawan);
                } else {
                    cout << "✗ Status tidak dikenal.\n";
                }
                break;
            }
            
            case 10: {
                cout << "\n--- HAPUS DATA GAJI ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                payroll.hapusDataGaji(idKaryawan);
                break;
            }
            
            case 11: {
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
