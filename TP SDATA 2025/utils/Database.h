#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>

using namespace std;

struct KonfigurasiDatabase {
    string namaDatabase;
    string hostServer;
    string portServer;
    string usernameDB;
    string passwordDB;
};

class Database {
private:
    KonfigurasiDatabase config;
    bool isConnected;
    
public:
    Database() : isConnected(false) {
        config.namaDatabase = "HRManagement";
        config.hostServer = "localhost";
        config.portServer = "5432";
        config.usernameDB = "admin";
        config.passwordDB = "***";
    }
    
    bool connect() {
        isConnected = true;
        cout << " Database berhasil terhubung!\n";
        return true;
    }
    
    bool disconnect() {
        isConnected = false;
        cout << " Database berhasil terputus!\n";
        return true;
    }
    
    bool isConnectedStatus() {
        return isConnected;
    }
    
    void tampilkanKonfigurasi() {
        cout << "\n===== KONFIGURASI DATABASE =====\n";
        cout << "Nama DB: " << config.namaDatabase << "\n";
        cout << "Host: " << config.hostServer << "\n";
        cout << "Port: " << config.portServer << "\n";
        cout << "Username: " << config.usernameDB << "\n\n";
    }
};

#endif
