#ifndef JADWAL_H
#define JADWAL_H

#include <iostream>
#include <string>
#include <fstream>
#include "../core/ManajemenKaryawan.h"

using namespace std;

// Struktur untuk hari dan jam kerja
struct JamKerja {
    string hari;
    string jamMulai;
    string jamSelesai;
    
    JamKerja() : hari(""), jamMulai(""), jamSelesai("") {}
    JamKerja(string h, string m, string s) : hari(h), jamMulai(m), jamSelesai(s) {}
};

// Struktur data jadwal kerja
struct DataJadwal {
    int idKaryawan;
    string namaKaryawan;
    string departemen;
    JamKerja jadwalMinggu[7];  // 7 hari
    string shiftKerja;  // "Pagi", "Siang", "Malam"
    string status;      // "Aktif" atau "Tidak Aktif"
    
    DataJadwal() : idKaryawan(0), namaKaryawan(""), departemen(""), shiftKerja("Pagi"), status("Aktif") {}
};

// Node untuk circular linked list Jadwal
struct NodeJadwal {
    DataJadwal data;
    NodeJadwal* next;
    
    NodeJadwal() : next(nullptr) {}
};

// Class Jadwal untuk manajemen jadwal kerja
class Jadwal {
private:
    NodeJadwal* headJadwal;
    int jumlahJadwal;
    
    // Fungsi private untuk traversal
    NodeJadwal* findByIdKaryawan(int idKaryawan) {
        if (headJadwal == nullptr) return nullptr;
        
        NodeJadwal* temp = headJadwal;
        do {
            if (temp->data.idKaryawan == idKaryawan) return temp;
            temp = temp->next;
        } while (temp != headJadwal);
        
        return nullptr;
    }
    
    // Validasi format jam
    bool validasiJam(string jam) {
        if (jam.length() != 5) return false;
        if (jam[2] != ':') return false;
        
        int jam_val = stoi(jam.substr(0, 2));
        int menit_val = stoi(jam.substr(3, 2));
        
        return (jam_val >= 0 && jam_val < 24 && menit_val >= 0 && menit_val < 60);
    }
    
    // Hitung durasi kerja dalam jam
    double hitungDurasiKerja(string jamMulai, string jamSelesai) {
        int jam1 = stoi(jamMulai.substr(0, 2));
        int menit1 = stoi(jamMulai.substr(3, 2));
        int jam2 = stoi(jamSelesai.substr(0, 2));
        int menit2 = stoi(jamSelesai.substr(3, 2));
        
        int totalMenit1 = jam1 * 60 + menit1;
        int totalMenit2 = jam2 * 60 + menit2;
        
        if (totalMenit2 < totalMenit1) {
            totalMenit2 += 24 * 60;  // Shift malam
        }
        
        return (double)(totalMenit2 - totalMenit1) / 60.0;
    }

public:
    Jadwal() : headJadwal(nullptr), jumlahJadwal(0) {}
    
    // Tambah jadwal karyawan (circular)
    void tambahJadwal(int idKaryawan, string namaKaryawan, string departemen, string shift) {
        NodeJadwal* nodeBaru = new NodeJadwal();
        nodeBaru->data.idKaryawan = idKaryawan;
        nodeBaru->data.namaKaryawan = namaKaryawan;
        nodeBaru->data.departemen = departemen;
        nodeBaru->data.shiftKerja = shift;
        
        if (headJadwal == nullptr) {
            headJadwal = nodeBaru;
            nodeBaru->next = nodeBaru;  // Circular
        } else {
            NodeJadwal* temp = headJadwal;
            while (temp->next != headJadwal) {
                temp = temp->next;
            }
            temp->next = nodeBaru;
            nodeBaru->next = headJadwal;
        }
        
        jumlahJadwal++;
        cout << "✓ Jadwal untuk " << namaKaryawan << " berhasil ditambahkan.\n";
    }
    
    // Set jadwal hari tertentu
    void setJadwalHari(int idKaryawan, int hariKe, string hari, string jamMulai, string jamSelesai) {
        if (hariKe < 1 || hariKe > 7) {
            cout << "✗ Hari harus antara 1-7.\n";
            return;
        }
        
        if (!validasiJam(jamMulai) || !validasiJam(jamSelesai)) {
            cout << "✗ Format jam tidak valid (gunakan HH:MM).\n";
            return;
        }
        
        NodeJadwal* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        node->data.jadwalMinggu[hariKe - 1] = JamKerja(hari, jamMulai, jamSelesai);
        cout << "✓ Jadwal " << hari << " untuk karyawan ID " << idKaryawan << " diatur.\n";
    }
    
    // Set shift kerja
    void setShift(int idKaryawan, string shift) {
        NodeJadwal* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        if (shift != "Pagi" && shift != "Siang" && shift != "Malam") {
            cout << "✗ Shift hanya boleh: Pagi, Siang, atau Malam.\n";
            return;
        }
        
        node->data.shiftKerja = shift;
        cout << "✓ Shift " << shift << " berhasil diatur untuk karyawan ID " << idKaryawan << ".\n";
    }
    
    // Tampilkan jadwal individual
    void tampilkanJadwalIndividu(int idKaryawan) {
        NodeJadwal* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        cout << "\n" << string(70, '=') << "\n";
        cout << "                     JADWAL KERJA KARYAWAN                        \n";
        cout << string(70, '=') << "\n\n";
        
        cout << "ID Karyawan    : " << node->data.idKaryawan << "\n";
        cout << "Nama Karyawan  : " << node->data.namaKaryawan << "\n";
        cout << "Departemen     : " << node->data.departemen << "\n";
        cout << "Shift Kerja    : " << node->data.shiftKerja << "\n";
        cout << "Status         : " << node->data.status << "\n\n";
        
        cout << "--- JADWAL MINGGUAN ---\n";
        cout << "No  Hari       Jam Mulai    Jam Selesai    Durasi Kerja\n";
        cout << string(60, '-') << "\n";
        
        for (int i = 0; i < 7; i++) {
            if (!node->data.jadwalMinggu[i].hari.empty()) {
                double durasi = hitungDurasiKerja(node->data.jadwalMinggu[i].jamMulai, 
                                                   node->data.jadwalMinggu[i].jamSelesai);
                cout << (i+1) << ".  " << node->data.jadwalMinggu[i].hari 
                     << string(max(0, 10 - (int)node->data.jadwalMinggu[i].hari.length()), ' ')
                     << node->data.jadwalMinggu[i].jamMulai 
                     << string(max(0, 13 - (int)node->data.jadwalMinggu[i].jamMulai.length()), ' ')
                     << node->data.jadwalMinggu[i].jamSelesai 
                     << string(max(0, 14 - (int)node->data.jadwalMinggu[i].jamSelesai.length()), ' ')
                     << durasi << " jam\n";
            }
        }
        
        cout << string(70, '=') << "\n\n";
    }
    
    // Tampilkan jadwal semua karyawan
    void tampilkanSemuaJadwal() {
        if (headJadwal == nullptr) {
            cout << "✗ Belum ada data jadwal.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "+                     JADWAL KERJA SELURUH KARYAWAN                       +\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "No  ID   Nama Karyawan          Departemen           Shift        Status\n";
        cout << string(90, '-') << "\n";
        
        NodeJadwal* temp = headJadwal;
        int no = 1;
        
        do {
            cout << no << ".  " << temp->data.idKaryawan << "   " 
                 << temp->data.namaKaryawan << string(max(0, 25 - (int)temp->data.namaKaryawan.length()), ' ')
                 << temp->data.departemen << string(max(0, 20 - (int)temp->data.departemen.length()), ' ')
                 << temp->data.shiftKerja << string(max(0, 12 - (int)temp->data.shiftKerja.length()), ' ')
                 << temp->data.status << "\n";
            
            temp = temp->next;
            no++;
        } while (temp != headJadwal);
        
        cout << string(90, '-') << "\n\n";
    }
    
    // Tampilkan jadwal per shift
    void tampilkanJadwalPerShift(string shift) {
        if (headJadwal == nullptr) {
            cout << "✗ Belum ada data jadwal.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "+              JADWAL KERJA - SHIFT " << shift << string(45 - shift.length(), ' ') << "+\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "No  ID   Nama Karyawan          Departemen           Status\n";
        cout << string(80, '-') << "\n";
        
        NodeJadwal* temp = headJadwal;
        int no = 1;
        bool ada = false;
        
        do {
            if (temp->data.shiftKerja == shift) {
                cout << no << ".  " << temp->data.idKaryawan << "   " 
                     << temp->data.namaKaryawan << string(max(0, 25 - (int)temp->data.namaKaryawan.length()), ' ')
                     << temp->data.departemen << string(max(0, 20 - (int)temp->data.departemen.length()), ' ')
                     << temp->data.status << "\n";
                ada = true;
                no++;
            }
            temp = temp->next;
        } while (temp != headJadwal);
        
        if (!ada) {
            cout << "Tidak ada karyawan dengan shift " << shift << ".\n";
        }
        cout << "\n";
    }
    
    // Ubah status jadwal
    void ubahStatus(int idKaryawan, string status) {
        NodeJadwal* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        if (status != "Aktif" && status != "Tidak Aktif") {
            cout << "✗ Status hanya boleh: Aktif atau Tidak Aktif.\n";
            return;
        }
        
        node->data.status = status;
        cout << "✓ Status jadwal untuk karyawan ID " << idKaryawan << " diubah menjadi " << status << ".\n";
    }
    
    // Hitung total jam kerja per minggu
    void hitungTotalJamMinggu(int idKaryawan) {
        NodeJadwal* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        double totalJam = 0;
        for (int i = 0; i < 7; i++) {
            if (!node->data.jadwalMinggu[i].hari.empty()) {
                totalJam += hitungDurasiKerja(node->data.jadwalMinggu[i].jamMulai, 
                                             node->data.jadwalMinggu[i].jamSelesai);
            }
        }
        
        cout << "\n✓ Total jam kerja per minggu untuk " << node->data.namaKaryawan 
             << " : " << totalJam << " jam\n\n";
    }
    
    // Laporan distribusi karyawan per shift
    void laporanDistribusiShift() {
        if (headJadwal == nullptr) {
            cout << "✗ Belum ada data jadwal.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "+              LAPORAN DISTRIBUSI SHIFT KERJA                             +\n";
        cout << "+==========================================================================+\n\n";
        
        int pagi = 0, siang = 0, malam = 0;
        
        NodeJadwal* temp = headJadwal;
        do {
            if (temp->data.shiftKerja == "Pagi") pagi++;
            else if (temp->data.shiftKerja == "Siang") siang++;
            else if (temp->data.shiftKerja == "Malam") malam++;
            
            temp = temp->next;
        } while (temp != headJadwal);
        
        cout << "Total Karyawan              : " << jumlahJadwal << "\n\n";
        cout << "Shift Pagi                  : " << pagi << " karyawan\n";
        cout << "Shift Siang                 : " << siang << " karyawan\n";
        cout << "Shift Malam                 : " << malam << " karyawan\n\n";
    }
    
    // Hapus jadwal
    void hapusJadwal(int idKaryawan) {
        if (headJadwal == nullptr) {
            cout << "✗ Belum ada data jadwal.\n";
            return;
        }
        
        NodeJadwal* toDelete = nullptr;
        
        if (headJadwal->data.idKaryawan == idKaryawan) {
            if (headJadwal->next == headJadwal) {
                toDelete = headJadwal;
                headJadwal = nullptr;
            } else {
                NodeJadwal* temp = headJadwal;
                while (temp->next != headJadwal) {
                    temp = temp->next;
                }
                toDelete = headJadwal;
                headJadwal = headJadwal->next;
                temp->next = headJadwal;
            }
        } else {
            NodeJadwal* temp = headJadwal;
            while (temp->next != headJadwal && temp->next->data.idKaryawan != idKaryawan) {
                temp = temp->next;
            }
            
            if (temp->next != headJadwal) {
                toDelete = temp->next;
                temp->next = temp->next->next;
            }
        }
        
        if (toDelete != nullptr) {
            delete toDelete;
            jumlahJadwal--;
            cout << "✓ Jadwal untuk ID " << idKaryawan << " berhasil dihapus.\n";
        } else {
            cout << "✗ Jadwal untuk ID " << idKaryawan << " tidak ditemukan.\n";
        }
    }
    
    // Getter
    int getJumlahJadwal() { return jumlahJadwal; }
    NodeJadwal* getHeadJadwal() { return headJadwal; }

    // Terapkan jadwal statis (sama untuk semua karyawan)
    void setJadwalStatisUntukSemua(ManajemenKaryawan &mk, string shift, JamKerja weekTemplate[7]) {
        Karyawan** dataKaryawan = mk.getDataKaryawan();
        int jumlah = mk.getUkuran();

        for (int i = 0; i < jumlah; i++) {
            if (!dataKaryawan[i]) continue;
            int id = dataKaryawan[i]->getId();
            string nama = dataKaryawan[i]->getNama();
            string departemen = dataKaryawan[i]->getDepartemen();

            NodeJadwal* node = findByIdKaryawan(id);
            if (node == nullptr) {
                // belum ada jadwal -> tambah
                tambahJadwal(id, nama, departemen, shift);
                node = findByIdKaryawan(id);
            } else {
                // sudah ada -> set shift
                node->data.shiftKerja = shift;
            }

            // set jadwal mingguan sesuai template
            for (int d = 0; d < 7; d++) {
                if (!weekTemplate[d].hari.empty()) {
                    // gunakan setJadwalHari untuk validasi jam
                    setJadwalHari(id, d+1, weekTemplate[d].hari, weekTemplate[d].jamMulai, weekTemplate[d].jamSelesai);
                }
            }
        }
    }
    
    // Destruktor
    ~Jadwal() {
        if (headJadwal != nullptr) {
            NodeJadwal* temp = headJadwal->next;
            headJadwal->next = nullptr;
            
            while (temp != nullptr) {
                NodeJadwal* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            
            delete headJadwal;
        }
    }
};

#endif
