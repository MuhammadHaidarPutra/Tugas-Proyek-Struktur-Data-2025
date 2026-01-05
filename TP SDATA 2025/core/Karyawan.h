#ifndef KARYAWAN_H
#define KARYAWAN_H

#include <string>

using namespace std;

class Karyawan {
public:
    int id;
    string nama;
    string jabatan;
    string departemen;
    
    Karyawan() : id(0), nama(""), jabatan(""), departemen("") {}
    
    Karyawan(int i, string n, string j, string d) 
        : id(i), nama(n), jabatan(j), departemen(d) {}
    
    int getId() { return id; }
    string getNama() { return nama; }
    string getJabatan() { return jabatan; }
    string getDepartemen() { return departemen; }
    
    void setNama(string n) { nama = n; }
    void setJabatan(string j) { jabatan = j; }
    void setDepartemen(string d) { departemen = d; }
};

#endif // KARYAWAN_H
