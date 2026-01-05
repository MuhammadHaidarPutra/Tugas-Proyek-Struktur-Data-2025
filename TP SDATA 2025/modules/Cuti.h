    #ifndef CUTI_H
    #define CUTI_H

    #include <iostream>
    #include <string>
    #include "../core/ManajemenKaryawan.h"

    using namespace std;

    // Saldo cuti per karyawan
    struct SaldoCuti {
        int idKaryawan;
        string namaKaryawan;
        double tahunan;
        double sakit;
        double pribadi;
        SaldoCuti() : idKaryawan(0), namaKaryawan(""), tahunan(12), sakit(10), pribadi(3) {}
    };

    struct SaldoCutiNode {
        SaldoCuti data;
        SaldoCutiNode* next;
        SaldoCutiNode(): next(NULL) {}
        SaldoCutiNode(const SaldoCuti &s): data(s), next(NULL) {}
    };

    // Permintaan cuti
    struct DataCuti {
        int idKaryawan;
        string namaKaryawan;
        string jenisCuti; // Tahunan/Sakit/Pribadi/Khusus
        string tanggalMulai;
        string tanggalSelesai;
        int jumlahHari;
        string alasan;
        string statusApproval; // Menunggu/Disetujui/Ditolak
    };

    struct NodeCuti {
        int requestId;
        DataCuti data;
        NodeCuti* next;
        NodeCuti(int reqId, const DataCuti &d) : requestId(reqId), data(d), next(NULL) {}
    };

    class Cuti {
    private:
        NodeCuti* headCuti; // front (depan antrian)
        NodeCuti* tailCuti; // rear (belakang antrian)
        int nextRequestId;
        SaldoCutiNode* headSaldo; // linked list of per-employee balances

        NodeCuti* findRequestNode(int requestId) {
            NodeCuti* cur = headCuti;
            while (cur != NULL) {
                if (cur->requestId == requestId) return cur;
                cur = cur->next;
            }
            return NULL;
        }

        SaldoCutiNode* findSaldoNodeById(int id) {
            SaldoCutiNode* cur = headSaldo;
            while (cur != NULL) {
                if (cur->data.idKaryawan == id) return cur;
                cur = cur->next;
            }
            return NULL;
        }

        SaldoCuti* findSaldoById(int id) {
            SaldoCutiNode* node = findSaldoNodeById(id);
            if (!node) return NULL;
            return &node->data;
        }

    public:
        Cuti() : headCuti(NULL), tailCuti(NULL), nextRequestId(1), headSaldo(NULL) {}

        ~Cuti() {
            // free cuti request queue
            NodeCuti* c = headCuti;
            while (c != NULL) {
                NodeCuti* toDelete = c;
                c = c->next;
                delete toDelete;
            }
            headCuti = NULL;
            tailCuti = NULL;
            // free saldo list
            SaldoCutiNode* s = headSaldo;
            while (s != NULL) {
                SaldoCutiNode* toDelete = s;
                s = s->next;
                delete toDelete;
            }
            headSaldo = NULL;
        }

        // Sinkronisasi saldo dengan data karyawan (panggil saat menu cuti dimulai)
        void syncSaldos(const ManajemenKaryawan &mk) {
            // Pastikan setiap karyawan memiliki entry saldo
            Karyawan** data = mk.getDataKaryawan();
            int jumlah = mk.getUkuran();

            // Tambahkan atau perbarui saldo untuk karyawan yang ada
            for (int i = 0; i < jumlah; i++) {
                if (!data[i]) continue;
                int id = data[i]->getId();
                string nama = data[i]->getNama();
                SaldoCutiNode* node = findSaldoNodeById(id);
                if (node) {
                    node->data.namaKaryawan = nama; // update nama jika berubah
                } else {
                    SaldoCuti newS;
                    newS.idKaryawan = id;
                    newS.namaKaryawan = nama;
                    SaldoCutiNode* nnode = new SaldoCutiNode(newS);
                    // prepend to list
                    nnode->next = headSaldo;
                    headSaldo = nnode;
                }
            }

            // Opsi: hapus saldo untuk karyawan yang sudah tidak ada dapat ditambahkan jika diperlukan
        }


        // Tambah permintaan cuti ke antrian (queue)
        void buatPermintaanCuti(int id, string nama, string jenis, string tglMulai, 
                                 string tglSelesai, int hari, string alasan) {
            DataCuti d;
            d.idKaryawan = id;
            d.namaKaryawan = nama;
            d.jenisCuti = jenis;
            d.tanggalMulai = tglMulai;
            d.tanggalSelesai = tglSelesai;
            d.jumlahHari = hari;
            d.alasan = alasan;
            d.statusApproval = "Menunggu";

            NodeCuti* node = new NodeCuti(nextRequestId++, d);
            // Tambah ke belakang antrian (queue)
            if (headCuti == NULL) {
                headCuti = node;
                tailCuti = node;
            } else {
                tailCuti->next = node;
                tailCuti = node;
            }

            cout << "\nPermintaan cuti berhasil diajukan! (Nomor Cuti: " << node->requestId << ")\n";
        }

        void tambahPermintaanCuti(int id, string nama, string jenis, string tglMulai, 
                                 string tglSelesai, int hari, string alasan) {
            buatPermintaanCuti(id, nama, jenis, tglMulai, tglSelesai, hari, alasan);
        }

        // Approve by request id; updates the relevant saldo if sufficient
        void setujuiCuti(int requestId, string catatan) {
            NodeCuti* node = findRequestNode(requestId);
            if (!node) { cout << "Nomor cuti tidak ditemukan!\n"; return; }
            if (node->data.statusApproval == "Disetujui") { cout << "Cuti sudah disetujui sebelumnya.\n"; return; }
            if (node->data.statusApproval == "Ditolak") { cout << "Cuti sebelumnya ditolak, tidak bisa disetujui.\n"; return; }

            SaldoCuti* saldo = findSaldoById(node->data.idKaryawan);
            if (!saldo) { cout << "Saldo karyawan tidak ditemukan. Pastikan data sinkron.\n"; return; }

            bool ok = false;
            if (node->data.jenisCuti == "Tahunan") {
                if (saldo->tahunan >= node->data.jumlahHari) { saldo->tahunan -= node->data.jumlahHari; ok = true; }
            } else if (node->data.jenisCuti == "Sakit") {
                if (saldo->sakit >= node->data.jumlahHari) { saldo->sakit -= node->data.jumlahHari; ok = true; }
            } else if (node->data.jenisCuti == "Pribadi") {
                if (saldo->pribadi >= node->data.jumlahHari) { saldo->pribadi -= node->data.jumlahHari; ok = true; }
            } else {
                // Khusus / other: do not affect automatic balances
                ok = true;
            }

            if (!ok) {
                cout << "Saldo tidak mencukupi untuk jenis cuti ini. Persetujuan dibatalkan.\n";
                return;
            }

            node->data.statusApproval = "Disetujui";
            cout << "Permintaan cuti (Nomor " << requestId << ") telah disetujui. Catatan: " << catatan << "\n";
        }

        void approvalCuti(int id, string status) {
            // backward-compatible wrapper (id = requestId)
            NodeCuti* node = findRequestNode(id);
            if (!node) { cout << "Nomor cuti tidak ditemukan!\n"; return; }
            node->data.statusApproval = status;
            cout << "Status permintaan cuti diubah menjadi: " << status << "\n";
        }

        void tolakCuti(int requestId, string alasan) {
            NodeCuti* node = findRequestNode(requestId);
            if (!node) { cout << "Nomor cuti tidak ditemukan!\n"; return; }
            if (node->data.statusApproval == "Disetujui") { cout << "Cuti sudah disetujui, tidak bisa ditolak.\n"; return; }
            node->data.statusApproval = "Ditolak";
            cout << "Permintaan cuti (Nomor " << requestId << ") ditolak. Alasan: " << alasan << "\n";
        }

        void rejectCuti(int id, string alasan) {
            tolakCuti(id, alasan);
        }

        void updateSaldoCuti(int id, double saldoTahunan, double saldoSakit, double saldoPribadi) {
            SaldoCuti* s = findSaldoById(id);
            if (!s) { cout << "Saldo karyawan tidak ditemukan.\n"; return; }
            s->tahunan = saldoTahunan;
            s->sakit = saldoSakit;
            s->pribadi = saldoPribadi;
            cout << "Saldo cuti berhasil diupdate untuk ID " << id << "\n";
        }

        void tampilkanRiwayatCutiKaryawan(int idKaryawan) {
            cout << "\n===== RIWAYAT CUTI (ID: " << idKaryawan << ") =====\n";
            NodeCuti* cur = headCuti;
            bool found = false;
            while (cur != NULL) {
                if (cur->data.idKaryawan == idKaryawan) {
                    cout << "Nomor: " << cur->requestId << " | " << cur->data.jenisCuti << " | " << cur->data.jumlahHari << " hari | " << cur->data.tanggalMulai << " - " << cur->data.tanggalSelesai << " | Status: " << cur->data.statusApproval << "\n";
                    found = true;
                }
                cur = cur->next;
            }
            if (!found) cout << "Belum ada permintaan cuti untuk karyawan ini.\n";
            cout << "\n";
        }

        void tampilkanRiwayatCuti(int idKaryawan) {
            tampilkanRiwayatCutiKaryawan(idKaryawan);
        }

        void tampilkanSemuaPermintaanCuti() {
            cout << "\n===== SEMUA PERMINTAAN CUTI =====\n";
            NodeCuti* cur = headCuti;
            if (!cur) { cout << "Belum ada permintaan cuti.\n\n"; return; }
            while (cur != NULL) {
                cout << "Nomor: " << cur->requestId << " | ID: " << cur->data.idKaryawan << " | " << cur->data.namaKaryawan << " | " << cur->data.jenisCuti << " | " << cur->data.jumlahHari << " hari | Status: " << cur->data.statusApproval << "\n";
                cur = cur->next;
            }
            cout << "\n";
        }

        void tampilkanCutiPending() {
            cout << "\n===== CUTI MENUNGGU APPROVAL =====\n";
            NodeCuti* cur = headCuti;
            bool found = false;
            while (cur != NULL) {
                if (cur->data.statusApproval == "Menunggu") {
                    cout << "Nomor: " << cur->requestId << " | ID: " << cur->data.idKaryawan << " | " << cur->data.namaKaryawan << " | " << cur->data.jenisCuti << " | " << cur->data.jumlahHari << " hari\n";
                    found = true;
                }
                cur = cur->next;
            }
            if (!found) cout << "Tidak ada permintaan cuti yang menunggu approval.\n";
            cout << "\n";
        }

        void tampilkanSaldoCutiKaryawan(int idKaryawan) {
            SaldoCuti* s = findSaldoById(idKaryawan);
            if (!s) { cout << "Saldo tidak ditemukan untuk ID " << idKaryawan << "\n"; return; }
            cout << "\n===== SALDO CUTI (ID: " << idKaryawan << ") =====\n";
            cout << "Cuti Tahunan: " << s->tahunan << " hari\n";
            cout << "Cuti Sakit: " << s->sakit << " hari\n";
            cout << "Cuti Pribadi: " << s->pribadi << " hari\n\n";
        }

        void laporanStatistikCuti() {
            int total = 0, disetujui = 0, ditolak = 0, pending = 0;
            NodeCuti* cur = headCuti;
            while (cur != NULL) {
                total++;
                if (cur->data.statusApproval == "Disetujui") disetujui++;
                else if (cur->data.statusApproval == "Ditolak") ditolak++;
                else pending++;
                cur = cur->next;
            }
            cout << "\n===== LAPORAN STATISTIK CUTI =====\n";
            cout << "Total permintaan: " << total << "\n";
            cout << "Disetujui: " << disetujui << " | Ditolak: " << ditolak << " | Menunggu: " << pending << "\n\n";
        }

        void tampilkanLaporanCuti() {
            laporanStatistikCuti();
        }
    };

    #endif
