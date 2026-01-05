#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <string>

using namespace std;

class Validator {
public:
    // Basic validations
    static bool isNotEmpty(string data) {
        return !data.empty();
    }
    
    static bool isValidNama(string nama) {
        return nama.length() >= 3 && nama.length() <= 100;
    }
    
    static bool isValidEmail(string email) {
        return email.find("@") != string::npos && email.find(".") != string::npos;
    }
    
    static bool isValidNomor(int nomor) {
        return nomor > 0;
    }
    
    static bool isValidTanggal(string tanggal) {
        return tanggal.length() == 10; // DD-MM-YYYY
    }
    
    static bool isValidPassword(string password) {
        return password.length() >= 6 && password.length() <= 50;
    }
    
    static bool isValidJabatan(string jabatan) {
        return jabatan.length() >= 3 && jabatan.length() <= 50;
    }
    
    static bool isValidDepartemen(string dept) {
        return dept.length() >= 3 && dept.length() <= 50;
    }
    
    static bool isValidGaji(double gaji) {
        return gaji > 0;
    }
    
    static bool isValidPersen(double persen) {
        return persen >= 0 && persen <= 100;
    }
};

#endif
