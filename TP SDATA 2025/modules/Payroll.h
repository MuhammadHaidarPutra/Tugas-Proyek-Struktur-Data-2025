#ifndef PAYROLL_H
#define PAYROLL_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Struktur untuk menyimpan detail tunjangan
struct TunjanganDetail {
    string nama;
    double nominal;
    
    TunjanganDetail() : nama(""), nominal(0.0) {}
    TunjanganDetail(string n, double nom) : nama(n), nominal(nom) {}
};

// Struktur untuk menyimpan detail potongan
struct PotonganDetail {
    string nama;
    double nominal;
    
    PotonganDetail() : nama(""), nominal(0.0) {}
    PotonganDetail(string n, double nom) : nama(n), nominal(nom) {}
};

// Struktur data slip gaji
struct DataGaji {
    int idKaryawan;
    string namaKaryawan;
    string bulanTahun;
    double gajiPokok;
    double totalTunjangan;
    double totalPotongan;
    double gajiKotor;
    double gajiNetto;
    string status;  // "Lunas" atau "Belum"
    
    DataGaji() : idKaryawan(0), namaKaryawan(""), bulanTahun(""), 
                 gajiPokok(0.0), totalTunjangan(0.0), totalPotongan(0.0),
                 gajiKotor(0.0), gajiNetto(0.0), status("Belum") {}
};

// Node untuk circular linked list Payroll
struct NodePayroll {
    DataGaji data;
    TunjanganDetail tunjangan[10];
    PotonganDetail potongan[10];
    int jumlahTunjangan;
    int jumlahPotongan;
    NodePayroll* next;
    
    NodePayroll() : jumlahTunjangan(0), jumlahPotongan(0), next(nullptr) {}
};

// Class Payroll untuk manajemen gaji
class Payroll {
private:
    NodePayroll* headPayroll;
    int jumlahDataPayroll;
    const char* filePayroll = "payroll_data.txt";
    
    // Fungsi private untuk traversal
    NodePayroll* findByIdKaryawan(int idKaryawan) {
        if (headPayroll == nullptr) return nullptr;
        
        NodePayroll* temp = headPayroll;
        do {
            if (temp->data.idKaryawan == idKaryawan) return temp;
            temp = temp->next;
        } while (temp != headPayroll);
        
        return nullptr;
    }
    
    // Hitung gaji kotor dan netto
    void hitungGaji(NodePayroll* node) {
        node->data.gajiKotor = node->data.gajiPokok + node->data.totalTunjangan;
        node->data.gajiNetto = node->data.gajiKotor - node->data.totalPotongan;
    }
    
    // Validasi bulan tahun
    bool validasiBulanTahun(string bulanTahun) {
        if (bulanTahun.length() != 7) return false;
        if (bulanTahun[2] != '-') return false;
        
        int bulan = stoi(bulanTahun.substr(0, 2));
        if (bulan < 1 || bulan > 12) return false;
        
        return true;
    }

public:
    Payroll() : headPayroll(nullptr), jumlahDataPayroll(0) {}
    
    // Tambah data gaji (circular)
    void tambahDataGaji(int idKaryawan, string namaKaryawan, string bulanTahun, double gajiPokok) {
        NodePayroll* nodeBaru = new NodePayroll();
        nodeBaru->data.idKaryawan = idKaryawan;
        nodeBaru->data.namaKaryawan = namaKaryawan;
        nodeBaru->data.bulanTahun = bulanTahun;
        nodeBaru->data.gajiPokok = gajiPokok;
        
        if (headPayroll == nullptr) {
            headPayroll = nodeBaru;
            nodeBaru->next = nodeBaru;  // Circular
        } else {
            NodePayroll* temp = headPayroll;
            while (temp->next != headPayroll) {
                temp = temp->next;
            }
            temp->next = nodeBaru;
            nodeBaru->next = headPayroll;
        }
        
        jumlahDataPayroll++;
        cout << "✓ Data gaji untuk " << namaKaryawan << " bulan " << bulanTahun << " ditambahkan.\n";
    }
    
    // Tambah tunjangan ke data gaji
    void tambahTunjangan(int idKaryawan, string namaTunjangan, double nominal) {
        NodePayroll* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Data gaji untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        if (node->jumlahTunjangan >= 10) {
            cout << "✗ Maksimal 10 tunjangan per karyawan.\n";
            return;
        }
        
        node->tunjangan[node->jumlahTunjangan] = TunjanganDetail(namaTunjangan, nominal);
        node->data.totalTunjangan += nominal;
        node->jumlahTunjangan++;
        hitungGaji(node);
        
        cout << "✓ Tunjangan " << namaTunjangan << " (" << nominal << ") ditambahkan.\n";
    }
    
    // Tambah potongan ke data gaji
    void tambahPotongan(int idKaryawan, string namaPotongan, double nominal) {
        NodePayroll* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Data gaji untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        if (node->jumlahPotongan >= 10) {
            cout << "✗ Maksimal 10 potongan per karyawan.\n";
            return;
        }
        
        node->potongan[node->jumlahPotongan] = PotonganDetail(namaPotongan, nominal);
        node->data.totalPotongan += nominal;
        node->jumlahPotongan++;
        hitungGaji(node);
        
        cout << "✓ Potongan " << namaPotongan << " (" << nominal << ") ditambahkan.\n";
    }
    
    // Tandai gaji sudah dibayar
    void tandaiLunas(int idKaryawan) {
        NodePayroll* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Data gaji untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        node->data.status = "Lunas";
        cout << "✓ Gaji untuk " << node->data.namaKaryawan << " ditandai lunas.\n";
    }
    
    // Tampilkan slip gaji individual
    void tampilkanSlipGaji(int idKaryawan) {
        NodePayroll* node = findByIdKaryawan(idKaryawan);
        if (node == nullptr) {
            cout << "✗ Data gaji untuk ID " << idKaryawan << " tidak ditemukan.\n";
            return;
        }
        
        cout << "\n" << string(70, '=') << "\n";
        cout << "                          SLIP GAJI KARYAWAN                        \n";
        cout << string(70, '=') << "\n\n";
        
        cout << "ID Karyawan      : " << node->data.idKaryawan << "\n";
        cout << "Nama Karyawan    : " << node->data.namaKaryawan << "\n";
        cout << "Bulan/Tahun      : " << node->data.bulanTahun << "\n";
        cout << "Status Pembayaran : " << node->data.status << "\n";
        
        cout << "\n--- RINCIAN GAJI ---\n";
        cout << "Gaji Pokok       : Rp " << node->data.gajiPokok << "\n";
        
        if (node->jumlahTunjangan > 0) {
            cout << "\n--- TUNJANGAN ---\n";
            for (int i = 0; i < node->jumlahTunjangan; i++) {
                cout << "  " << (i+1) << ". " << node->tunjangan[i].nama 
                     << " : Rp " << node->tunjangan[i].nominal << "\n";
            }
        }
        
        if (node->jumlahPotongan > 0) {
            cout << "\n--- POTONGAN ---\n";
            for (int i = 0; i < node->jumlahPotongan; i++) {
                cout << "  " << (i+1) << ". " << node->potongan[i].nama 
                     << " : Rp " << node->potongan[i].nominal << "\n";
            }
        }
        
        cout << "\n--- TOTAL ---\n";
        cout << "Total Tunjangan  : Rp " << node->data.totalTunjangan << "\n";
        cout << "Total Potongan   : Rp " << node->data.totalPotongan << "\n";
        cout << "Gaji Kotor       : Rp " << node->data.gajiKotor << "\n";
        cout << "Gaji Netto       : Rp " << node->data.gajiNetto << "\n";
        cout << string(70, '=') << "\n\n";
    }
    
    // Tampilkan laporan gaji semua karyawan
    void tampilkanLaporanGaji() {
        if (headPayroll == nullptr) {
            cout << "✗ Belum ada data gaji.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "|                       LAPORAN GAJI KARYAWAN                            |\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "No  ID   Nama Karyawan          Gaji Pokok      Gaji Netto       Status\n";
        cout << string(80, '-') << "\n";
        
        NodePayroll* temp = headPayroll;
        int no = 1;
        double totalGajiNetto = 0;
        
        do {
            cout << no << ".  " << temp->data.idKaryawan << "   " 
                 << temp->data.namaKaryawan << string(max(0, 25 - (int)temp->data.namaKaryawan.length()), ' ')
                 << "Rp " << temp->data.gajiPokok << string(max(0, 15 - (int)to_string((int)temp->data.gajiPokok).length()), ' ')
                 << "Rp " << temp->data.gajiNetto << string(max(0, 15 - (int)to_string((int)temp->data.gajiNetto).length()), ' ')
                 << temp->data.status << "\n";
            
            totalGajiNetto += temp->data.gajiNetto;
            temp = temp->next;
            no++;
        } while (temp != headPayroll);
        
        cout << string(80, '-') << "\n";
        cout << "TOTAL GAJI NETTO SELURUH KARYAWAN : Rp " << totalGajiNetto << "\n\n";
    }
    
    // Tampilkan ringkasan gaji (summary)
    void tampilkanRingkasanGaji() {
        if (headPayroll == nullptr) {
            cout << "✗ Belum ada data gaji.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "|                    RINGKASAN GAJI KARYAWAN                             |\n";
        cout << "+==========================================================================+\n\n";
        
        double totalGajiPokok = 0, totalTunjangan = 0, totalPotongan = 0;
        double totalGajiKotor = 0, totalGajiNetto = 0;
        int sudahLunas = 0, belumLunas = 0;
        
        NodePayroll* temp = headPayroll;
        do {
            totalGajiPokok += temp->data.gajiPokok;
            totalTunjangan += temp->data.totalTunjangan;
            totalPotongan += temp->data.totalPotongan;
            totalGajiKotor += temp->data.gajiKotor;
            totalGajiNetto += temp->data.gajiNetto;
            
            if (temp->data.status == "Lunas") sudahLunas++;
            else belumLunas++;
            
            temp = temp->next;
        } while (temp != headPayroll);
        
        cout << "Total Karyawan              : " << jumlahDataPayroll << "\n";
        cout << "Sudah Lunas                 : " << sudahLunas << "\n";
        cout << "Belum Lunas                 : " << belumLunas << "\n\n";
        
        cout << "Total Gaji Pokok            : Rp " << totalGajiPokok << "\n";
        cout << "Total Tunjangan             : Rp " << totalTunjangan << "\n";
        cout << "Total Potongan              : Rp " << totalPotongan << "\n";
        cout << "Total Gaji Kotor            : Rp " << totalGajiKotor << "\n";
        cout << "Total Gaji Netto            : Rp " << totalGajiNetto << "\n\n";
        
        cout << "Rata-Rata Gaji Pokok        : Rp " << (totalGajiPokok / jumlahDataPayroll) << "\n";
        cout << "Rata-Rata Gaji Netto        : Rp " << (totalGajiNetto / jumlahDataPayroll) << "\n\n";
    }
    
    // Filter gaji berdasarkan status
    void filterGajiByStatus(string status) {
        if (headPayroll == nullptr) {
            cout << "✗ Belum ada data gaji.\n";
            return;
        }
        
        cout << "\n+==========================================================================+\n";
        cout << "|              LAPORAN GAJI - STATUS: " << status << string(30 - status.length(), ' ') << "|\n";
        cout << "+==========================================================================+\n\n";
        
        cout << "No  ID   Nama Karyawan          Gaji Netto      Bulan/Tahun\n";
        cout << string(70, '-') << "\n";
        
        NodePayroll* temp = headPayroll;
        int no = 1;
        bool ada = false;
        
        do {
            if (temp->data.status == status) {
                cout << no << ".  " << temp->data.idKaryawan << "   " 
                     << temp->data.namaKaryawan << string(max(0, 25 - (int)temp->data.namaKaryawan.length()), ' ')
                     << "Rp " << temp->data.gajiNetto << string(max(0, 15 - (int)to_string((int)temp->data.gajiNetto).length()), ' ')
                     << temp->data.bulanTahun << "\n";
                ada = true;
                no++;
            }
            temp = temp->next;
        } while (temp != headPayroll);
        
        if (!ada) {
            cout << "Tidak ada data gaji dengan status " << status << ".\n";
        }
        cout << "\n";
    }
    
    // Hapus data gaji
    void hapusDataGaji(int idKaryawan) {
        if (headPayroll == nullptr) {
            cout << "✗ Belum ada data gaji.\n";
            return;
        }
        
        NodePayroll* toDelete = nullptr;
        
        if (headPayroll->data.idKaryawan == idKaryawan) {
            if (headPayroll->next == headPayroll) {
                // Hanya satu node
                toDelete = headPayroll;
                headPayroll = nullptr;
            } else {
                NodePayroll* temp = headPayroll;
                while (temp->next != headPayroll) {
                    temp = temp->next;
                }
                toDelete = headPayroll;
                headPayroll = headPayroll->next;
                temp->next = headPayroll;
            }
        } else {
            NodePayroll* temp = headPayroll;
            while (temp->next != headPayroll && temp->next->data.idKaryawan != idKaryawan) {
                temp = temp->next;
            }
            
            if (temp->next != headPayroll) {
                toDelete = temp->next;
                temp->next = temp->next->next;
            }
        }
        
        if (toDelete != nullptr) {
            delete toDelete;
            jumlahDataPayroll--;
            cout << "✓ Data gaji untuk ID " << idKaryawan << " berhasil dihapus.\n";
        } else {
            cout << "✗ Data gaji untuk ID " << idKaryawan << " tidak ditemukan.\n";
        }
    }
    
    // Getter
    int getJumlahDataPayroll() { return jumlahDataPayroll; }
    NodePayroll* getHeadPayroll() { return headPayroll; }
    
    // Destruktor
    ~Payroll() {
        if (headPayroll != nullptr) {
            NodePayroll* temp = headPayroll->next;
            headPayroll->next = nullptr;
            
            while (temp != nullptr) {
                NodePayroll* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            
            delete headPayroll;
        }
    }
};

#endif
