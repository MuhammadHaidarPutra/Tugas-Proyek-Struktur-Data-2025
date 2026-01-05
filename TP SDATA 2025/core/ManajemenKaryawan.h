#ifndef MANAJEMEN_KARYAWAN_H
#define MANAJEMEN_KARYAWAN_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Karyawan.h"
#include "Absensi.h"
#include "Log.h"
#include "Stack.h"

using namespace std;

class ManajemenKaryawan {
private:
    Karyawan** dataKaryawan;
    int ukuran;
    int kapasitas;

    NodeAbsensi *headAbsensi;

    NodeLog *headLog;
    NodeLog *tailLog; 

    StackActivity activityStack;

    string jamMulaiKerja = "08:00";
    string jamPulangKerja = "17:00";

    double hitungDurasi(string masuk, string keluar) {
        try {
            if (masuk.size() < 5 || keluar.size() < 5)
                return 0.0;    

            int jamMasuk = stoi(masuk.substr(0, 2));
            int menitMasuk = stoi(masuk.substr(3, 2));
            int jamKeluar = stoi(keluar.substr(0, 2));
            int menitKeluar = stoi(keluar.substr(3, 2));

            double durasi = (jamKeluar + menitKeluar / 60.0) - 
                            (jamMasuk + menitMasuk / 60.0);

            if (durasi < 0) durasi += 24;
            return durasi;
        }
        catch (...) {
            return 0.0;
        }
    }

    void perbesarKapasitas() {
        kapasitas = (kapasitas == 0) ? 5 : kapasitas * 2;
        Karyawan** baru = new Karyawan*[kapasitas];
        for (int i = 0; i < ukuran; i++) baru[i] = dataKaryawan[i];
        if (dataKaryawan != NULL) delete[] dataKaryawan;
        dataKaryawan = baru;
    }

public:
    ManajemenKaryawan() {
        ukuran = 0;
        kapasitas = 0;
        dataKaryawan = NULL;
        perbesarKapasitas();

        headAbsensi = NULL;

        headLog = NULL;
        tailLog = NULL;
    }

    ~ManajemenKaryawan() {
        for (int i = 0; i < ukuran; i++) delete dataKaryawan[i];
        if (dataKaryawan != NULL) delete[] dataKaryawan;

        if (headAbsensi != NULL) {
            NodeAbsensi* current = headAbsensi->next;
            while(current != headAbsensi) {
                NodeAbsensi* next = current->next;
                delete current;
                current = next;
            }
            delete headAbsensi;
        }

        NodeLog* currentLog = headLog;
        while(currentLog != NULL) {
            NodeLog* next = currentLog->next;
            delete currentLog;
            currentLog = next;
        }
    }

    void tambahKaryawan(int id, string nama, string jabatan, string departemen) {
        if (ukuran == kapasitas) perbesarKapasitas();
        dataKaryawan[ukuran] = new Karyawan(id, nama, jabatan, departemen);
        ukuran++;
        cout << "Data karyawan berhasil ditambahkan!\n";
    }

    void tambahKaryawanStatis(int id, string nama, string jabatan, string departemen) {
        if (ukuran == kapasitas) perbesarKapasitas();
        dataKaryawan[ukuran] = new Karyawan(id, nama, jabatan, departemen);
        ukuran++;
    }

    void tampilkan() {
        if (ukuran == 0) {
            cout << "Belum ada data karyawan.\n"; return;
        }
        cout << "\n==============================================================\n";
        cout << " ID | Nama                 | Jabatan           | Departemen\n";
        cout << "==============================================================\n";
        for (int i = 0; i < ukuran; i++) {
            cout << " " << setw(2) << left << dataKaryawan[i]->id << " | "
                 << setw(20) << left << dataKaryawan[i]->nama << " | "
                 << setw(17) << left << dataKaryawan[i]->jabatan << " | "
                 << setw(12) << left << dataKaryawan[i]->departemen << "\n";
        }
        cout << "==============================================================\n";
    }

    void ubah(int id) {
        for (int i = 0; i < ukuran; i++) {
            if (dataKaryawan[i]->id == id) {
                cout << "Masukkan nama baru: "; cin >> ws; getline(cin, dataKaryawan[i]->nama);
                cout << "Masukkan jabatan baru: "; cin >> ws; getline(cin, dataKaryawan[i]->jabatan);
                cout << "Masukkan departemen baru: "; cin >> ws; getline(cin, dataKaryawan[i]->departemen);
                cout << "Data berhasil diubah.\n";
                return;
            }
        }
        cout << "Data tidak ditemukan.\n";
    }

    void hapus(int id) {
        int indexHapus = -1;
        for (int i = 0; i < ukuran; i++) {
            if (dataKaryawan[i]->id == id) { indexHapus = i; break; }
        }
        if (indexHapus != -1) {
            delete dataKaryawan[indexHapus];
            for (int i = indexHapus; i < ukuran - 1; i++) {
                dataKaryawan[i] = dataKaryawan[i+1];
            }
            ukuran--;
            cout << "Data berhasil dihapus.\n";
        } else {
            cout << "ID tidak ditemukan.\n";
        }
    }

    // ==================== ABSENSI (CIRCULAR) ====================
    void checkIn(int id, string tanggal, string jamMasuk) {
        NodeAbsensi* baru = new NodeAbsensi(id, tanggal, jamMasuk);
        if (stoi(jamMasuk.substr(0,2)) > 8 || (stoi(jamMasuk.substr(0,2)) == 8 && stoi(jamMasuk.substr(3,2)) > 0)) {
            cout << "Anda terlambat! Check-in pada jam " << jamMasuk << " (Seharusnya 08:00)\n";
            baru->status = "Terlambat";
        } else {
            cout << "Tepat waktu! Check-in pada jam " << jamMasuk << endl;
        }

        if (headAbsensi == NULL) {
            headAbsensi = baru;
            headAbsensi->next = headAbsensi;
        } else {
            NodeAbsensi* temp = headAbsensi;
            while (temp->next != headAbsensi) temp = temp->next;
            temp->next = baru;
            baru->next = headAbsensi;
        }
    }

    void checkOut(int id, string tanggal, string jamKeluar) {
        if (headAbsensi == NULL) {
            cout << "Belum ada data absensi!\n"; return;
        }
        NodeAbsensi* temp = headAbsensi;
        do {
            if (temp->idKaryawan == id && temp->tanggal == tanggal && temp->jamKeluar == "") {
                temp->jamKeluar = jamKeluar;
                temp->durasiKerja = hitungDurasi(temp->jamMasuk, temp->jamKeluar);
                cout << "Check-out berhasil (Durasi kerja: " << temp->durasiKerja << " jam)\n";
                return;
            }
            temp = temp->next;
        } while (temp != headAbsensi);
        cout << "Data check-in tidak ditemukan atau sudah check-out.\n";
    }

    void tampilAbsensi() {
        if (headAbsensi == NULL) {
            cout << "Belum ada data absensi.\n"; return;
        }
        cout << "\n======================================================\n";
        cout << "ID | Tanggal | Jam Masuk | Jam Keluar | Durasi (jam) | Status\n";
        cout << "======================================================\n";
        NodeAbsensi* temp = headAbsensi;
        do {
            cout << temp->idKaryawan << " | " << temp->tanggal << " | "
                 << temp->jamMasuk << " | " << (temp->jamKeluar.empty() ? "-" : temp->jamKeluar) << " | "
                 << temp->durasiKerja << " | " << temp->status << endl;
            temp = temp->next;
        } while (temp != headAbsensi);
        cout << "======================================================\n";
    }

    void tambahAbsensi(int id, string tgl, string masuk, string keluar) {
        NodeAbsensi* baru = new NodeAbsensi(id, tgl, masuk);
        baru->jamKeluar = keluar;
        baru->durasiKerja = hitungDurasi(masuk, keluar);

        if (headAbsensi == NULL) {
            headAbsensi = baru;
            headAbsensi->next = headAbsensi;
        } else {
            NodeAbsensi* temp = headAbsensi;
            while (temp->next != headAbsensi) temp = temp->next;
            temp->next = baru;
            baru->next = headAbsensi; 
        }
        cout << "Data absensi berhasil ditambahkan/dikoreksi oleh Admin.\n";
    }

    void ubahAbsensi(int id, string tgl) {
        if (headAbsensi == NULL) {
            cout << "Data absensi kosong.\n"; return;
        }
        NodeAbsensi* temp = headAbsensi;
        do {
            if (temp->idKaryawan == id && temp->tanggal == tgl) {
                cout << "Data ditemukan! Masukkan data koreksi:\n";
                cout << "Jam Masuk baru (HH:MM): "; cin >> temp->jamMasuk;
                cout << "Jam Keluar baru (HH:MM): "; cin >> temp->jamKeluar;
                temp->durasiKerja = hitungDurasi(temp->jamMasuk, temp->jamKeluar);
                cout << "Status: (Hadir/Izin/Sakit): "; cin >> temp->status;
                cout << "Data absensi berhasil diubah/dikoreksi.\n";
                return;
            }
            temp = temp->next;
        } while (temp != headAbsensi);
        cout << "Data absensi untuk ID dan Tanggal tersebut tidak ditemukan!\n";
    }

    void hapusAbsensi(int id, string tgl) {
        if (headAbsensi == NULL) {
            cout << "Tidak ada data absensi untuk dihapus.\n"; return;
        }

        NodeAbsensi *curr = headAbsensi, *prev = NULL;

        bool found = false;
        do {
            if (curr->idKaryawan == id && curr->tanggal == tgl) {
                found = true;
                break;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != headAbsensi);

        if (!found) {
            cout << "Data absensi untuk ID dan Tanggal tersebut tidak ditemukan.\n";
            return;
        }

        if (curr == headAbsensi && curr->next == headAbsensi) {
            delete curr;
            headAbsensi = NULL;
        } else if (curr == headAbsensi) {
            NodeAbsensi* last = headAbsensi;
            while (last->next != headAbsensi) last = last->next;
            headAbsensi = headAbsensi->next;
            last->next = headAbsensi;
            delete curr;
        } else {
            prev->next = curr->next;
            delete curr;
        }

        cout << "Data absensi berhasil dihapus.\n";
    }

    // ==================== LOG ====================
    void tambahLog(string adminName, string aksi) {
        NodeLog* baru = new NodeLog(adminName, aksi);

        if (headLog == NULL) {
            headLog = baru;
            tailLog = baru;
        } else {
            tailLog->next = baru;
            baru->prev = tailLog;
            tailLog = baru;
        }
    }

    void tampilkanLogMaju() {
        cout << "\n--- LOG AKTIVITAS ADMIN (Terbaru di Bawah) ---\n";
        if (headLog == NULL) {
            cout << "Belum ada aktivitas log.\n";
            return;
        }
        NodeLog* temp = headLog;
        while (temp != NULL) {
            cout << "[" << temp->timestamp << "] "
                 << "Admin: " << temp->adminName
                 << " | Aksi: " << temp->aksi << endl;
            temp = temp->next;
        }
        cout << "----------------------------------------------\n";
    }

    void tampilkanLogMundur() {
        cout << "\n---------------------------------------------\n";
        cout << "\n    LOG AKTIVITAS ADMIN (Terbaru di Atas)    \n";
        cout << "\n---------------------------------------------\n";
        if (tailLog == NULL) {
            cout << "Belum ada aktivitas log.\n";
            return;
        }
        NodeLog* temp = tailLog;
        while (temp != NULL) {
            cout << "[" << temp->timestamp << "] "
                 << "Admin: " << temp->adminName
                 << " | Aksi: " << temp->aksi << endl;
            temp = temp->prev;
        }
        cout << "----------------------------------------------\n";
    }

    // ==================== FILE ====================
    void simpanKeFile() {
        ofstream file("data_karyawan.txt");
        if (!file.is_open()) {
            cout << "Gagal membuka file (data_karyawan.txt)!\n"; return;
        }

        file << "==============================================================\n";
        file << " ID | Nama                 | Jabatan           | Departemen\n";
        file << "==============================================================\n";
        for (int i = 0; i < ukuran; i++) {
            file << " " << setw(2) << left << dataKaryawan[i]->id << " | "
                 << setw(20) << left << dataKaryawan[i]->nama << " | "
                 << setw(17) << left << dataKaryawan[i]->jabatan << " | "
                 << setw(12) << left << dataKaryawan[i]->departemen << "\n";
        }
        file << "==============================================================\n";
        file.close();
        cout << "Data karyawan berhasil disimpan.\n";
    }

    void simpanAbsensiKeFile() {
        ofstream file("data_absensi.txt");
        if (!file.is_open()) {
            cout << "Gagal membuka file (data_absensi.txt)!\n"; return;
        }
        file << "==============================================================================================\n";
        file << " ID | Nama                 | Departemen    | Tanggal     | Jam Masuk | Jam Keluar | Durasi  | Status\n";
        file << "==============================================================================================\n";
        if (headAbsensi != NULL) {
            NodeAbsensi* temp = headAbsensi;
            do {
                string nama = "-", departemen = "-";
                for (int i = 0; i < ukuran; i++) {
                    if (dataKaryawan[i]->id == temp->idKaryawan) {
                        nama = dataKaryawan[i]->nama;
                        departemen = dataKaryawan[i]->departemen;
                        break;
                    }
                }
                file << " " << setw(2) << left << temp->idKaryawan << " | "
                     << setw(17) << left << nama << " | "
                     << setw(13) << left << departemen << " | "
                     << setw(11) << left << temp->tanggal << " | "
                     << setw(9) << left << temp->jamMasuk << " | "
                     << setw(10) << left << (temp->jamKeluar.empty() ? "-" : temp->jamKeluar) << " | "
                     << setw(7) << left << fixed << setprecision(2) << temp->durasiKerja << " | "
                     << setw(10) << left << temp->status << "\n";
                temp = temp->next;
            } while (temp != headAbsensi);
        }
        file << "==============================================================================================\n";
        file.close();
        cout << "Data absensi berhasil disimpan.\n";
    }

    void muatDariFile() {
        ifstream file("data_karyawan.txt");
        if (!file.is_open()) return;
        string baris;
        for (int i = 0; i < 3 && getline(file, baris); i++);
        int id;
        string nama, jabatan, departemen;
        while (file >> id) {
            file.ignore();
            getline(file, nama, '|');
            getline(file, jabatan, '|');
            getline(file, departemen);
            auto trim = [](string &s) {
                while (!s.empty() && isspace(s.front())) s.erase(s.begin());
                while (!s.empty() && isspace(s.back())) s.pop_back();
            };
            trim(nama); trim(jabatan); trim(departemen);
            tambahKaryawanStatis(id, nama, jabatan, departemen);
        }
        file.close();
        cout << "Data karyawan berhasil dimuat.\n";
    }

    void muatAbsensiDariFile() {
        ifstream file("data_absensi.txt");
        if (!file.is_open()) return;

        string baris;
        for (int i = 0; i < 3 && getline(file, baris); i++);

        int id;
        string tanggal, jamMasuk, jamKeluar, status, dummy;
        double durasi;

        while (getline(file, baris)) {
            if (baris.find("===") != string::npos) break;
            stringstream ss(baris);
            string segment;
            try {
                getline(ss, segment, '|'); id = stoi(segment);
                getline(ss, segment, '|'); 
                getline(ss, segment, '|'); 
                getline(ss, segment, '|'); tanggal = segment.substr(1, segment.find_last_not_of(' ') - 1);
                getline(ss, segment, '|'); jamMasuk = segment.substr(1, segment.find_last_not_of(' ') - 1);
                getline(ss, segment, '|'); jamKeluar = segment.substr(1, segment.find_last_not_of(' ') - 1);
                getline(ss, segment, '|'); durasi = stod(segment);
                getline(ss, segment, '|'); status = segment.substr(1, segment.find_last_not_of(' ') - 1);
            } catch (...) { continue; }

            tambahAbsensi(id, tanggal, jamMasuk, (jamKeluar == "-" ? "" : jamKeluar));
            NodeAbsensi* temp = headAbsensi;
            while(temp->next != headAbsensi) temp = temp->next; 
            temp->durasiKerja = durasi;
            temp->status = status;
        }
        file.close();
        cout << "Data absensi berhasil dimuat.\n";
    }

    // ==================== STACK ====================
    void pushActivity(const string &act) {
        activityStack.push(act);
    }

    void popActivity() {
        activityStack.pop();
    }

    void tampilActivity() {
        activityStack.tampilkanRiwayat();
    }

    // ==================== GETTER FUNCTIONS ====================
    NodeAbsensi* getHeadAbsensi() const {
        return headAbsensi;
    }

    Karyawan** getDataKaryawan() const {
        return dataKaryawan;
    }

    int getUkuran() const {
        return ukuran;
    }
};

#endif // MANAJEMEN_KARYAWAN_H
