#ifndef ABSENSI_H
#define ABSENSI_H

#include <iostream>
#include <string>

using namespace std;

struct NodeAbsensi {
    int idKaryawan;
    string tanggal;
    string jamMasuk;
    string jamKeluar;
    string status;
    double durasiKerja;
    NodeAbsensi* next;
    
    NodeAbsensi() : idKaryawan(0), tanggal(""), jamMasuk(""), 
                    jamKeluar(""), status("Hadir"), durasiKerja(0), next(NULL) {}
    
    NodeAbsensi(int id, string tgl, string jam) : idKaryawan(id), tanggal(tgl), jamMasuk(jam),
                    jamKeluar(""), status("Hadir"), durasiKerja(0), next(NULL) {}
};

#endif
