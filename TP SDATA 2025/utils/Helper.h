#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// ============================================================================
// UTILITY FUNCTIONS - HELPER CLASS UNTUK BERBAGAI KEPERLUAN
// ============================================================================

// Struktur untuk data format tabel
struct FormatTabel {
    int lebarKolom;
    string pemisah;
    bool adaBatas;
    
    FormatTabel(int l = 20, string p = " | ", bool b = true) 
        : lebarKolom(l), pemisah(p), adaBatas(b) {}
};

// Class Helper untuk utility functions
class Helper {
public:
    // Fungsi untuk membersihkan input buffer
    static void clearBuffer() {
        cin.clear();
        cin.ignore(10000, '\n');
    }
    
    // Fungsi untuk membuat garis pemisah
    static void cetakGaris(int panjang = 80, char karakter = '=') {
        for (int i = 0; i < panjang; i++) {
            cout << karakter;
        }
        cout << "\n";
    }
    
    // Fungsi untuk membuat garis single
    static void cetakGarisSingle(int panjang = 80) {
        for (int i = 0; i < panjang; i++) {
            cout << "-";
        }
        cout << "\n";
    }
    
    // Fungsi untuk membuat garis double
    static void cetakGarisDouble(int panjang = 80) {
        for (int i = 0; i < panjang; i++) {
            cout << "=";
        }
        cout << "\n";
    }
    
    // Fungsi untuk membuat padding string (kanan)
    static string paddingKanan(string text, int lebar) {
        if (text.length() >= lebar) return text;
        return text + string(lebar - text.length(), ' ');
    }
    
    // Fungsi untuk membuat padding string (kiri)
    static string paddingKiri(string text, int lebar) {
        if (text.length() >= lebar) return text;
        return string(lebar - text.length(), ' ') + text;
    }
    
    // Fungsi untuk membuat padding center
    static string paddingCenter(string text, int lebar) {
        if (text.length() >= lebar) return text;
        int padding = (lebar - text.length()) / 2;
        return string(padding, ' ') + text + string(lebar - text.length() - padding, ' ');
    }
    
    // Fungsi untuk validasi email (simple)
    static bool validasiEmail(string email) {
        if (email.find('@') == string::npos) return false;
        if (email.find('.') == string::npos) return false;
        return true;
    }
    
    // Fungsi untuk validasi nomor telepon
    static bool validasiTelepon(string telp) {
        if (telp.length() < 10 || telp.length() > 13) return false;
        for (char c : telp) {
            if (c != '+' && c != '-' && !isdigit(c)) return false;
        }
        return true;
    }
    
    // Fungsi untuk convert string ke uppercase
    static string toUpperCase(string text) {
        string result = text;
        for (char &c : result) {
            c = toupper(c);
        }
        return result;
    }
    
    // Fungsi untuk convert string ke lowercase
    static string toLowerCase(string text) {
        string result = text;
        for (char &c : result) {
            c = tolower(c);
        }
        return result;
    }
    
    // Fungsi untuk trim string (hapus spasi awal dan akhir)
    static string trim(string text) {
        size_t start = text.find_first_not_of(" \n\r\t");
        size_t end = text.find_last_not_of(" \n\r\t");
        if (start == string::npos) return "";
        return text.substr(start, end - start + 1);
    }
    
    // Fungsi untuk split string berdasarkan delimiter
    static void splitString(string text, char delimiter, string hasil[]) {
        int index = 0;
        string current = "";
        for (char c : text) {
            if (c == delimiter) {
                hasil[index++] = current;
                current = "";
            } else {
                current += c;
            }
        }
        hasil[index] = current;
    }
    
    // Fungsi untuk mengganti karakter dalam string
    static string replaceString(string text, string old_str, string new_str) {
        size_t pos = 0;
        while ((pos = text.find(old_str, pos)) != string::npos) {
            text.replace(pos, old_str.length(), new_str);
            pos += new_str.length();
        }
        return text;
    }
    
    // Fungsi untuk menghitung jumlah karakter tertentu dalam string
    static int countCharacter(string text, char c) {
        int count = 0;
        for (char ch : text) {
            if (ch == c) count++;
        }
        return count;
    }
    
    // Fungsi untuk validasi panjang string
    static bool validasiPanjang(string text, int minLen, int maxLen) {
        return text.length() >= minLen && text.length() <= maxLen;
    }
    
    // Fungsi untuk format currency (Rupiah)
    static string formatRupiah(double nilai) {
        string result = "Rp ";
        string angka = to_string((long long)nilai);
        int count = 0;
        
        for (int i = angka.length() - 1; i >= 0; i--) {
            if (count == 3) {
                result = "." + result;
                count = 0;
            }
            result = angka[i] + result;
            count++;
        }
        
        return result;
    }
    
    // Fungsi untuk format jam (HH:MM:SS)
    static string formatJam(int jam, int menit, int detik) {
        string result = "";
        if (jam < 10) result += "0";
        result += to_string(jam) + ":";
        
        if (menit < 10) result += "0";
        result += to_string(menit) + ":";
        
        if (detik < 10) result += "0";
        result += to_string(detik);
        
        return result;
    }
    
    // Fungsi untuk format tanggal (DD-MM-YYYY)
    static string formatTanggal(int hari, int bulan, int tahun) {
        string result = "";
        if (hari < 10) result += "0";
        result += to_string(hari) + "-";
        
        if (bulan < 10) result += "0";
        result += to_string(bulan) + "-";
        
        result += to_string(tahun);
        return result;
    }
    
    // Fungsi untuk validasi tanggal
    static bool validasiTanggal(int hari, int bulan, int tahun) {
        if (bulan < 1 || bulan > 12) return false;
        if (hari < 1 || hari > 31) return false;
        if (bulan == 2 && hari > 29) return false;
        if ((bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) && hari > 30) return false;
        return true;
    }
    
    // Fungsi untuk menampilkan menu dengan border
    static void tampilkanMenuBorder(string judul, string options[], int jumlahOption) {
        cout << "\n+==========================================================================+\n";
        cout << "+" << Helper::paddingCenter(judul, 70) << " +\n";
        cout << "+============================================================================+\n";
        
        for (int i = 0; i < jumlahOption; i++) {
            cout << "|" << Helper::paddingKanan(options[i], 70) << " |\n";
        }
        
        cout << "+=========================================================================+\n";
    }
    
    // Fungsi untuk menampilkan pesan error dengan border
    static void tampilkanError(string pesan) {
        cout << "\n+==========================================================================+\n";
        cout << "| ✗ " << Helper::paddingKanan(pesan, 67) << " |\n";
        cout << "+==========================================================================+\n";
    }
    
    // Fungsi untuk menampilkan pesan sukses dengan border
    static void tampilkanSukses(string pesan) {
        cout << "\n+==========================================================================+ \n";
        cout << "| ✓ " << Helper::paddingKanan(pesan, 67) << " |\n";
        cout << "+==========================================================================+\n";
    }
    
    // Fungsi untuk menampilkan pesan info dengan border
    static void tampilkanInfo(string pesan) {
        cout << "\n+==========================================================================+\n";
        cout << "| i " << Helper::paddingKanan(pesan, 67) << " |\n";
        cout << "+==========================================================================+\n";
    }
    
    // Fungsi untuk validasi pilihan menu (1-n)
    static int inputMenu(int maxOption) {
        int pilihan;
        bool valid = false;
        
        while (!valid) {
            cout << "Pilih menu (1-" << maxOption << "): ";
            cin >> pilihan;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "✗ Input tidak valid. Masukkan angka!\n";
            } else if (pilihan < 1 || pilihan > maxOption) {
                cout << "✗ Pilihan harus antara 1-" << maxOption << "!\n";
            } else {
                valid = true;
            }
        }
        
        return pilihan;
    }
    
    // Fungsi untuk input string dengan validasi panjang
    static string inputString(string prompt, int minLen, int maxLen) {
        string input;
        bool valid = false;
        
        while (!valid) {
            cout << prompt;
            getline(cin, input);
            input = trim(input);
            
            if (input.length() < minLen || input.length() > maxLen) {
                cout << "✗ Panjang harus antara " << minLen << "-" << maxLen << " karakter!\n";
            } else {
                valid = true;
            }
        }
        
        return input;
    }
    
    // Fungsi untuk input integer dengan validasi range
    static int inputInteger(string prompt, int minVal, int maxVal) {
        int input;
        bool valid = false;
        
        while (!valid) {
            cout << prompt;
            cin >> input;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "✗ Input tidak valid. Masukkan angka!\n";
            } else if (input < minVal || input > maxVal) {
                cout << "✗ Nilai harus antara " << minVal << "-" << maxVal << "!\n";
            } else {
                valid = true;
            }
        }
        
        return input;
    }
    
    // Fungsi untuk input double dengan validasi range
    static double inputDouble(string prompt, double minVal, double maxVal) {
        double input;
        bool valid = false;
        
        while (!valid) {
            cout << prompt;
            cin >> input;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "✗ Input tidak valid. Masukkan angka!\n";
            } else if (input < minVal || input > maxVal) {
                cout << "✗ Nilai harus antara " << minVal << "-" << maxVal << "!\n";
            } else {
                valid = true;
            }
        }
        
        return input;
    }
};

#endif
