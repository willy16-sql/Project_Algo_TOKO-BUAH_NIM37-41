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

void simpanFile(Buah *b, int n){
    FILE *file = fopen("data_buah.txt","w");
    if(file==NULL){
        cout<<"File tidak ditemukan\n";
        return;
    }
    for(int i=0;i<n;i++){
        fprintf(file, "%s\n", b[i].nama.c_str());
        fprintf(file, "%d\n%d\n", b[i].stok, b[i].harga);
    }
    fclose(file);
}

int bacaFile(Buah *b){
    FILE *file = fopen("data_buah.txt","r");
    if(file==NULL){
        return 0;
    }
    int i = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        hapusNewline(buffer);
        b[i].nama = buffer;
        if (fscanf(file, "%d %d", &b[i].stok, &b[i].harga) != 2) break;
        fgetc(file);
        i++;
    }
    fclose(file);
    return i;
}

const int MAX = 100;
int jumlah = 0;

int inputAngka(string pesan) {
    int x;
    cout << pesan;
    cin >> x;
    while (cin.fail() || x < 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input tidak valid! Masukkan angka >= 0: ";
        cin >> x;
    }
    return x;
}

int inputStok(string pesan) {
    int stok;
    do {
        stok = inputAngka(pesan);
        if (stok > 100) {
            cout << "Stok maksimal 100! Silakan masukkan ulang.\n";
        }
    } while (stok > 100);
    return stok;
}