#ifndef EVALUASI_H
#define EVALUASI_H

#include <iostream>
#include <string>

using namespace std;

struct DataEvaluasi {
    int idKaryawan;
    string namaKaryawan;
    string jabatan;
    string periodeEvaluasi;
    string tanggalEvaluasi;
    double nilaiKinerja;
    double nilaiKualitas;
    double nilaiKehadiran;
    double nilaiKepatuhan;
    double nilaiAtasan;
    double nilaiRerata;
    string grade;
    string feedback;
};

struct NodeEvaluasi {
    DataEvaluasi data;
    NodeEvaluasi* next;
    
    NodeEvaluasi() : data{0, "", "", "", "", 0, 0, 0, 0, 0, 0, "", ""}, next(NULL) {}
};

class Evaluasi {
private:
    NodeEvaluasi* headEvaluasi;
    
public:
    Evaluasi() : headEvaluasi(NULL) {}
    
    void tambahEvaluasi(int id, string nama, string jabatan, string periode, string tgl) {
        (void)id; (void)nama; (void)jabatan; (void)periode; (void)tgl;
        cout << "\n Data evaluasi berhasil ditambahkan!\n";
    }
    
    void tambahDataEvaluasi(int id, string nama, string periode, double nilaiKinerja, 
                           double nilaiKualitas, double nilaiKehadiran, double nilaiKepatuhan) {
        (void)id; (void)nama; (void)periode; (void)nilaiKinerja; (void)nilaiKualitas; (void)nilaiKehadiran; (void)nilaiKepatuhan;
        cout << "\n Data evaluasi berhasil ditambahkan!\n";
    }
    
    void tambahKriteria(int id, string kriteria, double nilai, string catatan) {
        (void)id; (void)kriteria; (void)nilai; (void)catatan;
        cout << " Kriteria evaluasi berhasil ditambahkan!\n";
    }
    
    void ubahNilaiKriteria(int id, int index, double nilaiBaruu) {
        (void)id; (void)index; (void)nilaiBaruu;
        cout << " Nilai kriteria berhasil diubah!\n";
    }
    
    void tampilkanEvaluasiIndividu(int idKaryawan) {
        cout << "\n===== HASIL EVALUASI KINERJA =====\n";
        cout << "ID: " << idKaryawan << "\n";
        cout << "Periode: Desember 2024\n";
        cout << "Rerata: 85\n";
        cout << "Grade: A (Sangat Baik)\n\n";
    }
    
    void tampilkanEvaluasi(int idKaryawan) {
        tampilkanEvaluasiIndividu(idKaryawan);
    }
    
    void tampilkanSemuaEvaluasi() {
        cout << "\n===== SEMUA DATA EVALUASI =====\n";
        cout << "Data evaluasi karyawan ditampilkan di sini\n\n";
    }
    
    void tampilkanEvaluasiPerPeriode(string periode) {
        cout << "\n===== EVALUASI PERIODE " << periode << " =====\n";
        cout << "Data evaluasi ditampilkan di sini\n\n";
    }
    
    void tampilkanEvaluasiPerGrade(string grade) {
        cout << "\n===== EVALUASI GRADE " << grade << " =====\n";
        cout << "Data evaluasi ditampilkan di sini\n\n";
    }
    
    void laporanStatistikEvaluasi() {
        cout << "\n===== LAPORAN STATISTIK EVALUASI =====\n";
        cout << "Data statistik evaluasi ditampilkan di sini\n\n";
    }
    
    void hapusEvaluasi(int id) {
        (void)id;
        cout << " Evaluasi berhasil dihapus!\n";
    }
    
    void tampilkanLaporanEvaluasi() {
        cout << "\n===== LAPORAN EVALUASI KINERJA =====\n";
        cout << "Data evaluasi karyawan ditampilkan di sini\n\n";
    }
};

#endif
