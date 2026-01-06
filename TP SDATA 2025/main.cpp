#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "core/ManajemenKaryawan.h"
#include "modules/Laporan.h"
#include "ui/MenuAdmin.h"
#include "modules/Payroll.h"
#include "modules/Jadwal.h"
#include "modules/Evaluasi.h"
#include "modules/Cuti.h"
#include "utils/Database.h"
#include "utils/FileIO.h"
#include "utils/Config.h"
#include "utils/Validator.h"
#include "ui/Menu.h"


using namespace std;

// Class untuk data KTP
class KTPUser {
public:
    string nik, nama, tempat_lahir, tanggal_lahir, jenis_kelamin, alamat, rt_rw, kelurahan, kecamatan, agama, status_perkawinan, pekerjaan, kewarganegaraan;

    // Parse dari string baris file (format spasi, kolom alamat fleksibel dan robust)
    bool fromString(const string& line) {
        istringstream iss(line);
        if (!(iss >> nik >> nama >> tempat_lahir >> tanggal_lahir >> jenis_kelamin)) return false;
        string arr[50];
        int idx = 0;
        while (iss >> arr[idx] && idx < 50) idx++;
        if (idx < 7) return false;
        // Alamat = gabungan arr[0] sampai arr[idx-7-1]
        alamat = arr[0];
        for (int i = 1; i < idx - 7; ++i) alamat += " " + arr[i];
        int n = idx;
        rt_rw = arr[n-7];
        kelurahan = arr[n-6];
        kecamatan = arr[n-5];
        agama = arr[n-4];
        status_perkawinan = arr[n-3];
        pekerjaan = arr[n-2];
        kewarganegaraan = arr[n-1];
        return true;
    }

    // Konversi ke string untuk simpan ke file (format spasi)
    string toString() const {
        return nik + " " + nama + " " + tempat_lahir + " " + tanggal_lahir + " " + jenis_kelamin + " " + alamat + " " + rt_rw + " " + kelurahan + " " + kecamatan + " " + agama + " " + status_perkawinan + " " + pekerjaan + " " + kewarganegaraan + "\n";
    }
};

// Class untuk handle file KTP
class KTPManager {
public:
    static const int MAX_USERS = 1000;
    KTPUser users[MAX_USERS];
    int userCount = 0;

    void loadFromFile(const string& filename) {
        userCount = 0;
        string absPath = filename;
        if (filename.find(":") == string::npos) {
            absPath = "d:/TP SDATA FIKS/TP SDATA 2025/" + filename;
        }
        ifstream file(absPath);
        if (!file.is_open()) {
            return;
        }
        string line;
        bool firstLine = true;
        while (getline(file, line)) {
            if (firstLine) { firstLine = false; continue; }
            if (users[userCount].fromString(line)) {
                userCount++;
                if (userCount >= MAX_USERS) break;
            }
        }
        file.close();
    }

    void saveToFile(const string& filename, const KTPUser& user) {
        ofstream file(filename, ios::app);
        file << user.toString();
        file.close();
    }

    int findUserByNIK(const string& nik) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].nik == nik) return i;
        }
        return -1;
    }
};


// Fungsi trim untuk menghapus spasi di awal/akhir string
string trim(const string& s) {
    size_t start = 0, end = s.length();
    while (start < end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
    while (end > start && (s[end-1] == ' ' || s[end-1] == '\t' || s[end-1] == '\r' || s[end-1] == '\n')) end--;
    string result;
    for (size_t i = start; i < end; ++i) result += s[i];
    return result;
}

// Fungsi untuk mengubah string ke lowercase
string toLower(const string& s) {
    string out = s;
    for (char& c : out) c = tolower(c);
    return out;
}

// Login dan daftar akun menggunakan file output_ktp.txt

int login(KTPManager& manager, string& namaLogin, string& roleLogin, int& idKaryawan, ManajemenKaryawan& mk) {
    int pilihan;
    string nama, nik;

    cout << "\n====================================================================\n";
    cout << "               SISTEM MANAJEMEN KARYAWAN - LOGIN\n";
    cout << "====================================================================\n";
    cout << "1. Login\n";
    cout << "2. Daftar Akun Baru\n";
    cout << "3. Keluar\n";
    cout << "====================================================================\n";
    cout << "Pilih menu: ";
    cin >> pilihan;
    cin.ignore();

    if (pilihan == 3) {
        cout << "\nKeluar dari program...\n";
        return 3;
    }

    if (pilihan == 2) {
        KTPUser newUser;
        cout << "\n=== Daftar Akun Baru ===\n";
        cout << "NIK: "; getline(cin, newUser.nik);
        manager.loadFromFile("KTP/output_ktp.txt");
        if (manager.findUserByNIK(newUser.nik) != -1) {
            cout << "\nNIK sudah terdaftar! Silakan login.\n";
            return 4;
        }
        cout << "Nama Lengkap: "; getline(cin, newUser.nama);
        cout << "Tempat Lahir: "; getline(cin, newUser.tempat_lahir);
        cout << "Tanggal Lahir (dd-mm-yyyy): "; getline(cin, newUser.tanggal_lahir);
        cout << "Jenis Kelamin: "; getline(cin, newUser.jenis_kelamin);
        cout << "Alamat: "; getline(cin, newUser.alamat);
        cout << "RT/RW: "; getline(cin, newUser.rt_rw);
        cout << "Kelurahan/Desa: "; getline(cin, newUser.kelurahan);
        cout << "Kecamatan: "; getline(cin, newUser.kecamatan);
        cout << "Agama: "; getline(cin, newUser.agama);
        cout << "Status Perkawinan: "; getline(cin, newUser.status_perkawinan);
        cout << "Pekerjaan: "; getline(cin, newUser.pekerjaan);
        cout << "Kewarganegaraan: "; getline(cin, newUser.kewarganegaraan);
        manager.saveToFile("KTP/output_ktp.txt", newUser);
        cout << "\nAkun berhasil dibuat! Silakan login.\n";
        return 4;
    }

    if (pilihan != 1) {
        cout << "\nPilihan tidak valid!\n";
        return 4;
    }

    cout << "\nMasukkan username (Nama): "; getline(cin, nama);
    cout << "Masukkan password (NIK): "; getline(cin, nik);
    nama = trim(nama);
    nik = trim(nik);
    string filePath = "KTP/output_ktp.txt";
    cout << "[INFO] Mulai proses login, membaca file KTP/output_ktp.txt..." << endl;
    manager.loadFromFile("KTP/output_ktp.txt");
    cout << "[INFO] Jumlah user terbaca: " << manager.userCount << endl;
    int idx = -1;
    for (int i = 0; i < manager.userCount; i++) {
        string namaFile = trim(manager.users[i].nama);
        string nikFile = trim(manager.users[i].nik);
        if (toLower(namaFile) == toLower(nama) && nikFile == nik) {
            idx = i;
            break;
        }
    }
    if (idx != -1) {
        namaLogin = manager.users[idx].nama;
        string pekerjaan = trim(manager.users[idx].pekerjaan);
        string pekerjaanLower = toLower(pekerjaan);
        if (pekerjaanLower == "admin") roleLogin = "admin";
        else roleLogin = "karyawan";
        Karyawan** dataK = mk.getDataKaryawan();
        int jumlahK = mk.getUkuran();
        int foundId = -1;
        for (int k = 0; k < jumlahK; k++) {
            if (dataK[k] && toLower(trim(dataK[k]->getNama())) == toLower(trim(namaLogin))) {
                foundId = dataK[k]->getId();
                break;
            }
        }
        if (foundId != -1) idKaryawan = foundId;
        else idKaryawan = idx + 1;
        cout << "\nLogin berhasil! Selamat datang, " << namaLogin << " (" << roleLogin << ")\n";
        return 2;
    } else {
        cout << "\nUsername atau password salah!\n";
        return 0;
    }
}



int main() {
    // Inisialisasi ManajemenKaryawan
    ManajemenKaryawan mk;

mk.tambahKaryawanStatis(4,  "Dewi",     "Manager", "Management");
mk.tambahKaryawanStatis(9,  "Eko",      "Manager", "Management");
mk.tambahKaryawanStatis(14, "Sinta15",  "Manager", "Management");
mk.tambahKaryawanStatis(19, "Reza20",   "Manager", "Management");
mk.tambahKaryawanStatis(24, "Ulia25",   "Manager", "Management");
mk.tambahKaryawanStatis(29, "Tono30",   "Manager", "Management");
mk.tambahKaryawanStatis(34, "Sari35",   "Manager", "Management");
mk.tambahKaryawanStatis(39, "Isna40",   "Manager", "Management");
mk.tambahKaryawanStatis(44, "Ilham45",  "Manager", "Management");
mk.tambahKaryawanStatis(49, "Nanda50",  "Manager", "Management");


mk.tambahKaryawanStatis(2,  "Siti",     "Analyst", "Analysis");
mk.tambahKaryawanStatis(7,  "Tono",     "Analyst", "Analysis");
mk.tambahKaryawanStatis(12, "Dina13",   "Analyst", "Analysis");
mk.tambahKaryawanStatis(17, "Fajar18",  "Analyst", "Analysis");
mk.tambahKaryawanStatis(22, "Putri23",  "Analyst", "Analysis");
mk.tambahKaryawanStatis(27, "Eko28",    "Analyst", "Analysis");
mk.tambahKaryawanStatis(32, "Maya33",   "Analyst", "Analysis");
mk.tambahKaryawanStatis(37, "Rudi38",   "Analyst", "Analysis");
mk.tambahKaryawanStatis(42, "Wawan43",  "Analyst", "Analysis");
mk.tambahKaryawanStatis(47, "Siska48",  "Analyst", "Analysis");


mk.tambahKaryawanStatis(3,  "Andi",     "Officer", "Operations");
mk.tambahKaryawanStatis(8,  "Rina",     "Officer", "Operations");
mk.tambahKaryawanStatis(13, "Riza14",   "Officer", "Operations");
mk.tambahKaryawanStatis(18, "Yuli19",   "Officer", "Operations");
mk.tambahKaryawanStatis(23, "Hari24",   "Officer", "Operations");
mk.tambahKaryawanStatis(28, "Wati29",   "Officer", "Operations");
mk.tambahKaryawanStatis(33, "Yudi34",   "Officer", "Operations");
mk.tambahKaryawanStatis(38, "Fitri39",  "Officer", "Operations");
mk.tambahKaryawanStatis(43, "Lusi44",   "Officer", "Operations");
mk.tambahKaryawanStatis(48, "Iman49",   "Officer", "Operations");


mk.tambahKaryawanStatis(5,  "Rudi",     "Developer", "IT");
mk.tambahKaryawanStatis(10, "Wati",     "Developer", "IT");
mk.tambahKaryawanStatis(15, "Agung16",  "Developer", "IT");
mk.tambahKaryawanStatis(20, "Nia21",    "Developer", "IT");
mk.tambahKaryawanStatis(25, "Dedi26",   "Developer", "IT");
mk.tambahKaryawanStatis(30, "Lina31",   "Developer", "IT");
mk.tambahKaryawanStatis(35, "Udin36",   "Developer", "IT");
mk.tambahKaryawanStatis(40, "Agus41",   "Developer", "IT");
mk.tambahKaryawanStatis(45, "Rita46",   "Developer", "IT");
mk.tambahKaryawanStatis(50, "Yosef51",  "Developer", "IT");


mk.tambahKaryawanStatis(1,  "Budi",     "Direktur", "Design"); 

mk.tambahKaryawanStatis(6,  "Lina",     "Designer", "Design");
mk.tambahKaryawanStatis(11, "Farhan12", "Designer", "Design");
mk.tambahKaryawanStatis(16, "Mila17",   "Designer", "Design");
mk.tambahKaryawanStatis(21, "Bambang22","Designer", "Design");
mk.tambahKaryawanStatis(26, "Rini27",   "Designer", "Design");
mk.tambahKaryawanStatis(31, "Andi32",   "Designer", "Design");
mk.tambahKaryawanStatis(36, "Lani37",   "Designer", "Design");
mk.tambahKaryawanStatis(41, "Rina42",   "Designer", "Design");
mk.tambahKaryawanStatis(46, "Bayu47",   "Designer", "Design");


    // Inisialisasi fitur baru
    Laporan laporan;
    Payroll payroll;
    Jadwal jadwal;
    Evaluasi evaluasi;
    Cuti cuti;

    KTPManager manager;
    bool keluar = false;
    while (!keluar) {
        string namaLogin, roleLogin;
        int idKaryawan;
        int statusLogin = login(manager, namaLogin, roleLogin, idKaryawan, mk);
        if (statusLogin == 2 || statusLogin == 1) {
            if (roleLogin == "admin") {
                menuAdminExtended(mk, laporan, payroll, jadwal, evaluasi, cuti, namaLogin);
            } else {
                menuKaryawan(namaLogin, idKaryawan, mk, cuti);
            }
        } else if (statusLogin == 3) {
            cout << "\nProgram selesai. Terima kasih.\n";
            keluar = true;
        } else if (statusLogin == 4) {
            continue;
        } else if (statusLogin == 0) {
            cout << "\nUsername atau password salah! Silakan coba lagi.\n\n";
        }
    }
    return 0;
}
