#ifndef ORGANISASI_TREE_H
#define ORGANISASI_TREE_H

#include <iostream>
#include <string>
using namespace std;

struct OrgNode {
    string nama;
    string jabatan;
    OrgNode** bawahan;
    int jumlahBawahan;
    int kapasitasBawahan;
    OrgNode(const string& n, const string& j) : nama(n), jabatan(j), jumlahBawahan(0), kapasitasBawahan(10) {
        bawahan = new OrgNode*[kapasitasBawahan];
    }
    ~OrgNode() {
        for (int i = 0; i < jumlahBawahan; ++i) delete bawahan[i];
        delete[] bawahan;
    }
    void tambahBawahan(OrgNode* node) {
        if (jumlahBawahan >= kapasitasBawahan) {
            kapasitasBawahan *= 2;
            OrgNode** baru = new OrgNode*[kapasitasBawahan];
            for (int i = 0; i < jumlahBawahan; ++i) baru[i] = bawahan[i];
            delete[] bawahan;
            bawahan = baru;
        }
        bawahan[jumlahBawahan++] = node;
    }
};

class OrganisasiTree {
private:
    OrgNode* root;
public:
    OrganisasiTree() : root(nullptr) {}
    ~OrganisasiTree() { hapusTree(root); }

    void setRoot(const string& nama, const string& jabatan) {
        if (root) hapusTree(root);
        root = new OrgNode(nama, jabatan);
    }

    OrgNode* getRoot() { return root; }

    // Tambah bawahan ke node tertentu (cari berdasarkan nama)
    bool tambahBawahan(const string& namaAtasan, const string& namaBawahan, const string& jabatanBawahan) {
        OrgNode* atasan = cariNode(root, namaAtasan);
        if (!atasan) return false;
        atasan->tambahBawahan(new OrgNode(namaBawahan, jabatanBawahan));
        return true;
    }

    // Tampilkan tree secara hierarki (preorder)
    void tampilkanStruktur() {
        tampilkanStrukturRec(root, 0);
    }

private:
    void tampilkanStrukturRec(OrgNode* node, int level) {
        if (!node) return;
        for (int i = 0; i < level; ++i) cout << "  ";
        cout << "- " << node->jabatan << ": " << node->nama << endl;
        for (int i = 0; i < node->jumlahBawahan; ++i) {
            tampilkanStrukturRec(node->bawahan[i], level + 1);
        }
    }

    OrgNode* cariNode(OrgNode* node, const string& nama) {
        if (!node) return nullptr;
        if (node->nama == nama) return node;
        for (int i = 0; i < node->jumlahBawahan; ++i) {
            OrgNode* found = cariNode(node->bawahan[i], nama);
            if (found) return found;
        }
        return nullptr;
    }

    void hapusTree(OrgNode* node) {
        if (!node) return;
        delete node;
    }
};

#endif
