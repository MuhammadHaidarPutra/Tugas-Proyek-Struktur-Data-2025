#ifndef MENUEVALUASI_H
#define MENUEVALUASI_H

#include <iostream>
#include <string>
#include "../modules/Evaluasi.h"
#include "../core/ManajemenKaryawan.h"

using namespace std;

// Menu untuk manajemen Evaluasi Karyawan
void menuEvaluasi(ManajemenKaryawan& mk, Evaluasi& evaluasi, string namaAdmin) {
    int pilihan;
    bool kembali = false;
    
    while (!kembali) {
        cout << "\n+==========================================================================+\n";
        cout << "|              MENU MANAJEMEN EVALUASI KARYAWAN                          |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "1. Buat Evaluasi Baru\n";
        cout << "2. Tambah Kriteria Penilaian\n";
        cout << "3. Ubah Nilai Kriteria\n";
        cout << "4. Tampilkan Evaluasi Individual\n";
        cout << "5. Tampilkan Semua Evaluasi\n";
        cout << "6. Tampilkan Evaluasi Per Periode\n";
        cout << "7. Tampilkan Evaluasi Per Grade\n";
        cout << "8. Laporan Statistik Evaluasi\n";
        cout << "9. Hapus Evaluasi\n";
        cout << "10. Kembali ke Menu Admin\n";
        
        cout << "\nPilih menu (1-10): ";
        cin >> pilihan;
        cin.ignore();
        
        switch (pilihan) {
            case 1: {
                cout << "\n--- BUAT EVALUASI BARU ---\n";
                int idKaryawan;
                string periode, tanggalEvaluasi;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                // Cari data karyawan
                Karyawan** dataKaryawan = mk.getDataKaryawan();
                string namaKaryawan = "Unknown";
                string jabatan = "Unknown";
                
                for (int i = 0; i < mk.getUkuran(); i++) {
                    if (dataKaryawan[i] && dataKaryawan[i]->getId() == idKaryawan) {
                        namaKaryawan = dataKaryawan[i]->getNama();
                        jabatan = dataKaryawan[i]->getJabatan();
                        break;
                    }
                }
                
                cout << "Masukkan Periode Evaluasi (Q1 2025/Q2 2025/...): ";
                getline(cin, periode);
                
                cout << "Masukkan Tanggal Evaluasi (DD-MM-YYYY): ";
                getline(cin, tanggalEvaluasi);
                
                evaluasi.tambahEvaluasi(idKaryawan, namaKaryawan, jabatan, periode, tanggalEvaluasi);
                break;
            }
            
            case 2: {
                cout << "\n--- TAMBAH KRITERIA PENILAIAN ---\n";
                int idKaryawan, nilai;
                string kriteria, catatan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Kriteria Penilaian (misal: Kedisiplinan): ";
                getline(cin, kriteria);
                
                cout << "Masukkan Nilai (1-100): ";
                cin >> nilai;
                cin.ignore();
                
                cout << "Masukkan Catatan: ";
                getline(cin, catatan);
                
                evaluasi.tambahKriteria(idKaryawan, kriteria, nilai, catatan);
                break;
            }
            
            case 3: {
                cout << "\n--- UBAH NILAI KRITERIA ---\n";
                int idKaryawan, indexKriteria, nilaiBaruu;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                cout << "Masukkan Index Kriteria (1, 2, 3, ...): ";
                cin >> indexKriteria;
                cin.ignore();
                
                cout << "Masukkan Nilai Baru (1-100): ";
                cin >> nilaiBaruu;
                cin.ignore();
                
                evaluasi.ubahNilaiKriteria(idKaryawan, indexKriteria, nilaiBaruu);
                break;
            }
            
            case 4: {
                cout << "\n--- TAMPILKAN EVALUASI INDIVIDUAL ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                evaluasi.tampilkanEvaluasiIndividu(idKaryawan);
                break;
            }
            
            case 5: {
                cout << "\n";
                evaluasi.tampilkanSemuaEvaluasi();
                break;
            }
            
            case 6: {
                cout << "\n--- TAMPILKAN EVALUASI PER PERIODE ---\n";
                string periode;
                
                cout << "Masukkan Periode (Q1 2025/Q2 2025/...): ";
                getline(cin, periode);
                
                evaluasi.tampilkanEvaluasiPerPeriode(periode);
                break;
            }
            
            case 7: {
                cout << "\n--- TAMPILKAN EVALUASI PER GRADE ---\n";
                string grade;
                
                cout << "Masukkan Grade (A/B/C/D): ";
                getline(cin, grade);
                
                evaluasi.tampilkanEvaluasiPerGrade(grade);
                break;
            }
            
            case 8: {
                cout << "\n";
                evaluasi.laporanStatistikEvaluasi();
                break;
            }
            
            case 9: {
                cout << "\n--- HAPUS EVALUASI ---\n";
                int idKaryawan;
                
                cout << "Masukkan ID Karyawan: ";
                cin >> idKaryawan;
                cin.ignore();
                
                evaluasi.hapusEvaluasi(idKaryawan);
                break;
            }
            
            case 10: {
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
