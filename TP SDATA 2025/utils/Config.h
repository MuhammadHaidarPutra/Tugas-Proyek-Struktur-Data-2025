#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>

using namespace std;

struct SettingAplikasi {
    string namaAplikasi;
    string versi;
    string tahun;
    string bahasa;
    bool debugMode;
    bool logActivity;
};

struct SettingKeamanan {
    int maxLoginAttempt;
    int sessionTimeout;
    bool requirePassword;
    bool requireOTP;
};

struct SettingTampilan {
    bool showBorder;
    bool showColor;
    string tema;
    int maxDataPerPage;
};

class Config {
private:
    SettingAplikasi appSetting;
    SettingKeamanan keamananSetting;
    SettingTampilan tampilanSetting;
    
public:
    Config() {
        appSetting.namaAplikasi = "HR Management System";
        appSetting.versi = "1.0";
        appSetting.tahun = "2024";
        appSetting.bahasa = "Indonesian";
        appSetting.debugMode = false;
        appSetting.logActivity = true;
        
        keamananSetting.maxLoginAttempt = 3;
        keamananSetting.sessionTimeout = 1800;
        keamananSetting.requirePassword = true;
        keamananSetting.requireOTP = false;
        
        tampilanSetting.showBorder = true;
        tampilanSetting.showColor = true;
        tampilanSetting.tema = "Dark";
        tampilanSetting.maxDataPerPage = 10;
    }
    
    void tampilkanInfoSistem() {
        cout << "\n===== INFORMASI SISTEM =====\n";
        cout << "Aplikasi: " << appSetting.namaAplikasi << "\n";
        cout << "Versi: " << appSetting.versi << "\n";
        cout << "Tahun: " << appSetting.tahun << "\n";
        cout << "Bahasa: " << appSetting.bahasa << "\n\n";
    }
    
    void resetKeDefault() {
        appSetting.debugMode = false;
        keamananSetting.maxLoginAttempt = 3;
        tampilanSetting.tema = "Dark";
        cout << " Konfigurasi direset ke default!\n";
    }
};

#endif
