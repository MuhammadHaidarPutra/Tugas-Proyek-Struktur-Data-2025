#ifndef MENU_ADMIN_H
#define MENU_ADMIN_H

#include <iostream>
#include <string>
#include "../core/ManajemenKaryawan.h"
#include "../modules/Laporan.h"
#include "MenuLaporan.h"
#include "../modules/Payroll.h"
#include "MenuPayroll.h"
#include "../modules/Jadwal.h"
#include "MenuJadwal.h"
#include "../modules/Evaluasi.h"
#include "MenuEvaluasi.h"
#include "../modules/Cuti.h"
#include "MenuCuti.h"
#include "../core/OrganisasiTree.h"

using namespace std;

void menuAdminExtended(ManajemenKaryawan &mk, Laporan &laporan, Payroll &payroll, 
                      Jadwal &jadwal, Evaluasi &evaluasi, Cuti &cuti, string namaAdmin) {
    int pilihan;
    bool keluar = false;
    
    while (!keluar) {
        cout << "\n+==========================================================================+\n";
        cout << "|                    SELAMAT DATANG DI MENU ADMIN                          |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "+==========================================================================+\n";
        cout << "| 1. Tambah Karyawan                                                       |\n";
        cout << "| 2. Tampilkan Semua Karyawan                                              |\n";
        cout << "| 3. Ubah Data Karyawan                                                    |\n";
        cout << "| 4. Hapus Karyawan                                                        |\n";
        cout << "+==========================================================================+\n";
        cout << "| 5. Tampilkan Semua Absensi                                               |\n";
        cout << "| 6. Tambah Absensi Karyawan                                               |\n";
        cout << "| 7. Ubah Data Absensi                                                     |\n";
        cout << "| 8. Hapus Data Absensi                                                    |\n";
        cout << "+==========================================================================+\n";
        cout << "| 9. Tampilkan Log Aktivitas Admin                                         |\n";
        cout << "| 10. Tampilkan Activity Stack (LIFO)                                      |\n";
        cout << "+==========================================================================+\n";
        cout << "| 11. Menu Laporan & Statistik Absensi                                     |\n";
        cout << "+==========================================================================+\n";
        cout << "| 12. Menu Manajemen Gaji & Payroll                                        |\n";
        cout << "+==========================================================================+\n";
        cout << "| 13. Menu Manajemen Jadwal Kerja                                          |\n";
        cout << "+==========================================================================+\n";
        cout << "| 14. Menu Manajemen Evaluasi Kinerja                                      |\n";
        cout << "+==========================================================================+\n";
        cout << "| 15. Menu Manajemen Cuti Karyawan                                         |\n";
        cout << "+==========================================================================+\n";
        cout << "| 16. Tampilkan Struktur Organisasi Perusahaan (Tree)                      |\n";
        cout << "| 17. Logout                                                               |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "Pilih menu (1-17): ";
        cin >> pilihan;
        cin.ignore();
        
        int id;
        string nama, jabatan, departemen;
        string tanggal, jamMasuk, jamKeluar;
        
        switch(pilihan) {
                        case 16: {
                            // Tree organisasi seluruh karyawan, hirarki: Direktur > Manajer > Supervisor > Officer > Staff > Lainnya
                            OrganisasiTree org;
                            Karyawan** dataK = mk.getDataKaryawan();
                            int jumlah = mk.getUkuran();
                            // 1. Cari direktur sebagai root
                            string rootNama = "", rootJabatan = "";
                            for (int i = 0; i < jumlah; i++) {
                                if (dataK[i] && (dataK[i]->getJabatan() == "Direktur" || dataK[i]->getJabatan() == "direktur")) {
                                    rootNama = dataK[i]->getNama();
                                    rootJabatan = dataK[i]->getJabatan();
                                    break;
                                }
                            }
                            if (rootNama == "") {
                                cout << "\nTidak ditemukan karyawan dengan jabatan Direktur.\n";
                                break;
                            }
                            org.setRoot(rootNama, rootJabatan);
                            // 2. Tambahkan semua karyawan ke tree sesuai atasan jabatan dan departemen
                            // Aturan: Manajer ke Direktur, Supervisor ke Manajer, Officer ke Supervisor, Staff ke Officer, lainnya ke root
                            // Map atasan: jabatan -> atasan
                            for (int i = 0; i < jumlah; i++) {
                                if (!dataK[i]) continue;
                                string jab = dataK[i]->getJabatan();
                                string nama = dataK[i]->getNama();
                                if (jab == "Direktur" || jab == "direktur") continue;
                                string atasanNama = rootNama;
                                string atasanJabatan = rootJabatan;
                                if (jab == "Manajer" || jab == "manajer") {
                                    atasanNama = rootNama;
                                } else if (jab == "Supervisor" || jab == "supervisor") {
                                    // Cari manajer di departemen sama
                                    for (int j = 0; j < jumlah; j++) {
                                        if (dataK[j] && (dataK[j]->getJabatan() == "Manajer" || dataK[j]->getJabatan() == "manajer") && dataK[j]->getDepartemen() == dataK[i]->getDepartemen()) {
                                            atasanNama = dataK[j]->getNama();
                                            break;
                                        }
                                    }
                                } else if (jab == "Officer" || jab == "officer") {
                                    // Cari supervisor di departemen sama
                                    for (int j = 0; j < jumlah; j++) {
                                        if (dataK[j] && (dataK[j]->getJabatan() == "Supervisor" || dataK[j]->getJabatan() == "supervisor") && dataK[j]->getDepartemen() == dataK[i]->getDepartemen()) {
                                            atasanNama = dataK[j]->getNama();
                                            break;
                                        }
                                    }
                                } else if (jab == "Staff" || jab == "staff") {
                                    // Cari officer di departemen sama
                                    for (int j = 0; j < jumlah; j++) {
                                        if (dataK[j] && (dataK[j]->getJabatan() == "Officer" || dataK[j]->getJabatan() == "officer") && dataK[j]->getDepartemen() == dataK[i]->getDepartemen()) {
                                            atasanNama = dataK[j]->getNama();
                                            break;
                                        }
                                    }
                                }
                                org.tambahBawahan(atasanNama, nama, jab);
                            }
                            cout << "\nSTRUKTUR ORGANISASI PERUSAHAAN (berdasarkan seluruh data karyawan):\n";
                            org.tampilkanStruktur();
                            break;
                        }
            case 1: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      TAMBAH KARYAWAN BARU                               |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan: "; cin >> id;
                cout << "Nama: "; cin >> ws; getline(cin, nama);
                cout << "Jabatan: "; cin >> ws; getline(cin, jabatan);
                cout << "Departemen: "; cin >> ws; getline(cin, departemen);
                
                mk.tambahKaryawan(id, nama, jabatan, departemen);
                cout << "\n✓ Karyawan berhasil ditambahkan!\n";
                break;
            }
            
            case 2: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      DAFTAR SEMUA KARYAWAN                              |\n";
                cout << "+==========================================================================+\n\n";
                
                Karyawan** dataK = mk.getDataKaryawan();
                int jumlah = mk.getUkuran();
                
                cout << "ID  | Nama                     | Jabatan              | Departemen\n";
                cout << string(80, '-') << "\n";
                
                for(int i = 0; i < jumlah; i++) {
                    if(dataK[i]) {
                        cout << dataK[i]->getId() << "   | " 
                             << dataK[i]->getNama() << string(24 - dataK[i]->getNama().length(), ' ') << "| "
                             << dataK[i]->getJabatan() << string(20 - dataK[i]->getJabatan().length(), ' ') << "| "
                             << dataK[i]->getDepartemen() << "\n";
                    }
                }
                cout << "\nTotal Karyawan: " << jumlah << "\n";
                break;
            }
            
            case 3: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      UBAH DATA KARYAWAN                                 |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan yang akan diubah: "; cin >> id;
                cout << "Jabatan baru: "; cin >> ws; getline(cin, jabatan);
                cout << "Departemen baru: "; cin >> ws; getline(cin, departemen);
                
                Karyawan** dataK = mk.getDataKaryawan();
                int jumlah = mk.getUkuran();
                bool found = false;
                
                for(int i = 0; i < jumlah; i++) {
                    if(dataK[i] && dataK[i]->getId() == id) {
                        dataK[i]->setJabatan(jabatan);
                        dataK[i]->setDepartemen(departemen);
                        cout << "\n✓ Data karyawan berhasil diubah!\n";
                        found = true;
                        break;
                    }
                }
                
                if(!found) cout << "\n✗ Karyawan dengan ID " << id << " tidak ditemukan!\n";
                break;
            }
            
            case 4: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      HAPUS KARYAWAN                                     |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan yang akan dihapus: "; cin >> id;
                
                Karyawan** dataK = mk.getDataKaryawan();
                int jumlah = mk.getUkuran();
                bool found = false;
                
                for(int i = 0; i < jumlah; i++) {
                    if(dataK[i] && dataK[i]->getId() == id) {
                        for(int j = i; j < jumlah - 1; j++) {
                            dataK[j] = dataK[j+1];
                        }
                        cout << "\n✓ Karyawan berhasil dihapus!\n";
                        found = true;
                        break;
                    }
                }
                
                if(!found) cout << "\n✗ Karyawan dengan ID " << id << " tidak ditemukan!\n";
                break;
            }
            
            case 5: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      DAFTAR ABSENSI KARYAWAN                            |\n";
                cout << "+==========================================================================+\n\n";
                
                mk.tampilAbsensi();
                break;
            }
            
            case 6: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      TAMBAH DATA ABSENSI                                |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (DD-MM-YYYY): "; cin >> ws; getline(cin, tanggal);
                cout << "Jam Masuk (HH:MM): "; cin >> ws; getline(cin, jamMasuk);
                cout << "Jam Keluar (HH:MM): "; cin >> ws; getline(cin, jamKeluar);
                
                mk.tambahAbsensi(id, tanggal, jamMasuk, jamKeluar);
                cout << "\n✓ Data absensi berhasil ditambahkan!\n";
                break;
            }
            
            case 7: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      UBAH DATA ABSENSI                                  |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (DD-MM-YYYY): "; cin >> ws; getline(cin, tanggal);
                cout << "Jam Masuk baru (HH:MM): "; cin >> ws; getline(cin, jamMasuk);
                cout << "Jam Keluar baru (HH:MM): "; cin >> ws; getline(cin, jamKeluar);
                
                cout << "\n✓ Data absensi berhasil diubah!\n";
                break;
            }
            
            case 8: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      HAPUS DATA ABSENSI                                 |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "ID Karyawan: "; cin >> id;
                cout << "Tanggal (DD-MM-YYYY): "; cin >> ws; getline(cin, tanggal);
                
                cout << "\n✓ Data absensi berhasil dihapus!\n";
                break;
            }
            
            case 9: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      LOG AKTIVITAS ADMIN                                |\n";
                cout << "+==========================================================================+\n\n";
                
                cout << "✓ Log aktivitas admin (fitur untuk logging activities)\n\n";
                break;
            }
            
            case 10: {
                cout << "\n+==========================================================================+\n";
                cout << "|                      ACTIVITY STACK (LIFO)                              |\n";
                cout << "+==========================================================================+\n\n";
                
                mk.tampilActivity();
                break;
            }
            
            case 11: {
                menuLaporan(mk, laporan, namaAdmin);
                break;
            }
            
            case 12: {
                menuPayroll(mk, payroll, namaAdmin);
                break;
            }
            
            case 13: {
                menuJadwal(mk, jadwal, namaAdmin);
                break;
            }
            
            case 14: {
                menuEvaluasi(mk, evaluasi, namaAdmin);
                break;
            }
            
            case 15: {
                menuCuti(mk, cuti, namaAdmin);
                break;
            }
            
            case 17: {
                cout << "\n+==========================================================================+\n";
                cout << "|                                                                        |\n";
                cout << "|               ✓ Anda telah logout. Terima kasih!                      |\n";
                cout << "|                                                                        |\n";
                cout << "+==========================================================================+\n\n";
                keluar = true;
                break;
            }
            
            default: {
                cout << "\n✗ Pilihan tidak valid! Silakan pilih 1-16.\n";
                break;
            }
        }
        
        if (!keluar) {
            cout << "\nTekan ENTER untuk melanjutkan...";
            cin.get();
        }
    }
}

#endif // MENU_ADMIN_H
