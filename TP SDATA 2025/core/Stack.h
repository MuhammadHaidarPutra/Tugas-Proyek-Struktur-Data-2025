#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>

using namespace std;

class StackActivity {
private:
    static const int MAX = 100;
    string stackData[MAX];
    int top;

public:
    StackActivity() { top = -1; }

    bool isFull() { return top == MAX - 1; }
    bool isEmpty() { return top == -1; }

    void push(const string &activity) {
        if (isFull()) {
            cout << "Stack penuh! Riwayat tidak bisa ditambahkan.\n";
            return;
        }
        top++;
        stackData[top] = activity;
    }

    void pop() {
        if (isEmpty()) {
            cout << "Tidak ada aktivitas untuk di-undo.\n";
            return;
        }
        cout << "Undo aktivitas: " << stackData[top] << endl;
        top--;
    }

    void tampilkanRiwayat() {
        if (isEmpty()) {
            cout << "Belum ada riwayat aktivitas.\n";
            return;
        }
        cout << "\n============================================\n";
        cout << "\n      RIWAYAT AKTIVITAS (STACK / LIFO)      \n";
        cout << "\n============================================\n";
        for (int i = top; i >= 0; i--) {
            cout << "- " << stackData[i] << endl;
        }
        cout << "============================================\n";
    }
};

#endif // STACK_H
