#include <iostream>
#include <iomanip>
using namespace std;

struct Buah {
    string nama;
    int stok;
    int harga; // harga per kg
};

// Fungsi bantu hapus newline
void hapusNewline(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}
