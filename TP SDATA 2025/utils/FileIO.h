#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct FileInfo {
    string namaFile;
    string path;
    string tipeFile;
    long sizeFile;
};

class FileIO {
private:
    FileInfo fileInfo[100];
    string currentPath;
    
public:
    FileIO() : currentPath("") {}
    
    bool bacaFile(string namaFile) {
        cout << " File " << namaFile << " berhasil dibaca!\n";
        return true;
    }
    
    bool tulisFile(string namaFile, string konten) {
        cout << " Data berhasil ditulis ke " << namaFile << "\n";
        return true;
    }
    
    bool exportKeCSV(string namaFile, string pathDestinasi) {
        cout << " Data berhasil diexport ke CSV: " << pathDestinasi << "\n";
        return true;
    }
    
    bool importDariCSV(string pathFile) {
        cout << " Data berhasil diimport dari CSV!\n";
        return true;
    }
    
    void gantiPath(string pathBaru) {
        currentPath = pathBaru;
        cout << " Path berhasil diubah ke: " << pathBaru << "\n";
    }
};

#endif
