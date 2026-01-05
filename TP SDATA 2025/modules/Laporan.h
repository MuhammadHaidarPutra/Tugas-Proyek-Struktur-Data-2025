#ifndef LAPORAN_H
#define LAPORAN_H

#include <iostream>
#include <string>
#include <iomanip>
#include "../core/Karyawan.h"
#include "../core/Absensi.h"

using namespace std;

struct DataStatistik {
    int idKaryawan;
    string namaKaryawan;
    int jumlahHadir;
    int jumlahIzin;
    int jumlahAlfa;
    double totalJamKerja;
    double rataRataJamKerja;
    
    DataStatistik() 
        : idKaryawan(0), namaKaryawan(""), jumlahHadir(0), 
          jumlahIzin(0), jumlahAlfa(0), totalJamKerja(0.0), 
          rataRataJamKerja(0.0) {}
};

struct NodeLaporan {
    DataStatistik data;
    NodeLaporan* next;
    
    NodeLaporan(DataStatistik d) : data(d), next(NULL) {}
};

class Laporan {
private:
    NodeLaporan* headLaporan;
    
public:
    Laporan() : headLaporan(NULL) {}
    
    ~Laporan() {
        if (headLaporan != NULL) {
            NodeLaporan* current = headLaporan->next;
            while (current != headLaporan) {
                NodeLaporan* next = current->next;
                delete current;
                current = next;
            }
            delete headLaporan;
            headLaporan = NULL;
        }
    }
    
    // Membersihkan data laporan lama
    void clear() {
        if (headLaporan != NULL) {
            NodeLaporan* current = headLaporan->next;
            while (current != headLaporan) {
                NodeLaporan* next = current->next;
                delete current;
                current = next;
            }
            delete headLaporan;
            headLaporan = NULL;
        }
    }
    
    // Menambah data statistik ke circular linked list
    void tambahStatistik(DataStatistik statistik) {
        NodeLaporan* baru = new NodeLaporan(statistik);
        
        if (headLaporan == NULL) {
            headLaporan = baru;
            headLaporan->next = headLaporan;
        } else {
            NodeLaporan* temp = headLaporan;
            while (temp->next != headLaporan) {
                temp = temp->next;
            }
            temp->next = baru;
            baru->next = headLaporan;
        }
    }
    
    // Menampilkan rekap absensi per karyawan
    void tampilkanRekapAbsensi() {
        if (headLaporan == NULL) {
            cout << "Belum ada data statistik. Silakan generate laporan terlebih dahulu.\n";
            return;
        }
        
        cout << "\n";
        cout << "+==========================================================================+\n";
        cout << "|                      REKAP ABSENSI PER KARYAWAN                         |\n";
        cout << "+==========================================================================+\n";
        cout << "\n";
        cout << setw(4) << "ID" << " | "
             << setw(20) << "Nama Karyawan" << " | "
             << setw(7) << "Hadir" << " | "
             << setw(7) << "Izin" << " | "
             << setw(7) << "Alfa" << " | "
             << setw(8) << "Total Hari" << endl;
        cout << "+--------------------------------------------------------------------------+\n";
        
        NodeLaporan* temp = headLaporan;
        do {
            int totalHari = temp->data.jumlahHadir + temp->data.jumlahIzin + temp->data.jumlahAlfa;
            cout << setw(4) << temp->data.idKaryawan << " | "
                 << setw(20) << left << temp->data.namaKaryawan << " | "
                 << right << setw(7) << temp->data.jumlahHadir << " | "
                 << setw(7) << temp->data.jumlahIzin << " | "
                 << setw(7) << temp->data.jumlahAlfa << " | "
                 << setw(8) << totalHari << endl;
            temp = temp->next;
        } while (temp != headLaporan);
        
        cout << "+--------------------------------------------------------------------------+\n\n";
    }
    
    // Menampilkan rata-rata jam kerja karyawan
    void tampilkanRataRataJamKerja() {
        if (headLaporan == NULL) {
            cout << "Belum ada data statistik. Silakan generate laporan terlebih dahulu.\n";
            return;
        }
        
        cout << "\n";
        cout << "+==========================================================================+\n";
        cout << "|                  RATA-RATA JAM KERJA PER KARYAWAN                         |\n";
        cout << "+==========================================================================+   \n";
        cout << "\n";
        cout << setw(4) << "ID" << " | "
             << setw(20) << "Nama Karyawan" << " | "
             << setw(15) << "Total Jam Kerja" << " | "
             << setw(15) << "Rata-Rata/Hari" << endl;
        cout << "+--------------------------------------------------------------------------+\n";
        
        NodeLaporan* temp = headLaporan;
        double totalJamKeseluruhan = 0.0;
        int totalKaryawan = 0;
        
        do {
            cout << setw(4) << temp->data.idKaryawan << " | "
                 << setw(20) << left << temp->data.namaKaryawan << " | "
                 << right << setw(15) << fixed << setprecision(2) << temp->data.totalJamKerja << " | "
                 << setw(15) << fixed << setprecision(2) << temp->data.rataRataJamKerja << endl;
            
            totalJamKeseluruhan += temp->data.totalJamKerja;
            totalKaryawan++;
            temp = temp->next;
        } while (temp != headLaporan);
        
        cout << "+--------------------------------------------------------------------------+\n";
        if (totalKaryawan > 0) {
            cout << "Rata-Rata Jam Kerja Seluruh Karyawan: " 
                 << fixed << setprecision(2) << (totalJamKeseluruhan / totalKaryawan) << " jam/hari\n";
        }
        cout << "\n";
    }
    
    // Generate laporan dari data absensi circular
    void generateLaporan(NodeAbsensi* headAbsensi, Karyawan** dataKaryawan, int jumlahKaryawan) {
        clear();
        
        if (headAbsensi == NULL || jumlahKaryawan == 0) {
            cout << "Data absensi atau karyawan kosong!\n";
            return;
        }
        
        // Proses setiap karyawan
        for (int i = 0; i < jumlahKaryawan; i++) {
            DataStatistik stat;
            stat.idKaryawan = dataKaryawan[i]->id;
            stat.namaKaryawan = dataKaryawan[i]->nama;
            stat.jumlahHadir = 0;
            stat.jumlahIzin = 0;
            stat.jumlahAlfa = 0;
            stat.totalJamKerja = 0.0;
            
            // Traverse circular linked list untuk absensi
            NodeAbsensi* temp = headAbsensi;
            int jumlahAbsensiKaryawan = 0;
            
            do {
                if (temp->idKaryawan == dataKaryawan[i]->id) {
                    // Hitung berdasarkan status
                    if (temp->status == "Hadir" || temp->status == "Terlambat") {
                        stat.jumlahHadir++;
                    } else if (temp->status == "Izin") {
                        stat.jumlahIzin++;
                    } else if (temp->status == "Sakit" || temp->status == "Alfa") {
                        stat.jumlahAlfa++;
                    }
                    
                    // Hitung jam kerja
                    stat.totalJamKerja += temp->durasiKerja;
                    jumlahAbsensiKaryawan++;
                }
                temp = temp->next;
            } while (temp != headAbsensi);
            
            // Hitung rata-rata jam kerja
            if (jumlahAbsensiKaryawan > 0) {
                stat.rataRataJamKerja = stat.totalJamKerja / jumlahAbsensiKaryawan;
            } else {
                stat.rataRataJamKerja = 0.0;
            }
            
            tambahStatistik(stat);
        }
        
        cout << "Laporan berhasil di-generate!\n";
    }
    
    // Menampilkan laporan lengkap
    void tampilkanLaporanLengkap() {
        tampilkanRekapAbsensi();
        tampilkanRataRataJamKerja();
    }
    
    // Statistik detail per karyawan
    void tampilkanDetailKaryawan(int idKaryawan) {
        if (headLaporan == NULL) {
            cout << "Belum ada data statistik.\n";
            return;
        }
        
        NodeLaporan* temp = headLaporan;
        do {
            if (temp->data.idKaryawan == idKaryawan) {
                cout << "\n";
                cout << "+==========================================================================+\n";
                cout << "|                    DETAIL STATISTIK KARYAWAN                             |\n";
                cout << "+==========================================================================+\n";
                cout << "\n";
                cout << "ID Karyawan             : " << temp->data.idKaryawan << endl;
                cout << "Nama Karyawan           : " << temp->data.namaKaryawan << endl;
                cout << "Jumlah Hadir            : " << temp->data.jumlahHadir << " hari\n";
                cout << "Jumlah Izin             : " << temp->data.jumlahIzin << " hari\n";
                cout << "Jumlah Alfa/Sakit       : " << temp->data.jumlahAlfa << " hari\n";
                cout << "Total Jam Kerja         : " << fixed << setprecision(2) << temp->data.totalJamKerja << " jam\n";
                cout << "Rata-Rata Jam Kerja     : " << fixed << setprecision(2) << temp->data.rataRataJamKerja << " jam/hari\n";
                cout << "\n";
                return;
            }
            temp = temp->next;
        } while (temp != headLaporan);
        
        cout << "Data karyawan dengan ID " << idKaryawan << " tidak ditemukan.\n";
    }
};

#endif // LAPORAN_H
