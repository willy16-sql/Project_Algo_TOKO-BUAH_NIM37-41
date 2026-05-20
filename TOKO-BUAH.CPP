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

// Menu 1
void tambahBuah(Buah *ptr) {
    cin.ignore();
    cout << "\n========== TAMBAH BUAH ==========\n";
    cout << "Nama buah: ";
    getline(cin, (ptr + jumlah)->nama);
    (ptr + jumlah)->stok = inputStok("Stok (0-100): ");
    (ptr + jumlah)->harga = inputAngka("Harga per kg: ");
    jumlah++;
    simpanFile(ptr, jumlah);
    cout << "Data berhasil ditambahkan!\n";
}

// Fungsi swap
void swapBuah(Buah *a, Buah *b) {
    Buah temp = *a;
    *a = *b;
    *b = temp;
}

// ==================== SORTING ALGORITHMS ====================
// 1. Bubble Sort
void bubbleSort(Buah *ptr, int n, bool ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (ascending) {
                if (ptr[j].harga > ptr[j+1].harga)
                    swapBuah(&ptr[j], &ptr[j+1]);
            } else {
                if (ptr[j].harga < ptr[j+1].harga)
                    swapBuah(&ptr[j], &ptr[j+1]);
            }
        }
    }
}

// 2. Selection Sort
void selectionSort(Buah *ptr, int n, bool ascending) {
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            if (ascending) {
                if (ptr[j].harga < ptr[idx].harga)
                    idx = j;
            } else {
                if (ptr[j].harga > ptr[idx].harga)
                    idx = j;
            }
        }
        if (idx != i)
            swapBuah(&ptr[i], &ptr[idx]);
    }
}

// 3. Insertion Sort
void insertionSort(Buah *ptr, int n, bool ascending) {
    for (int i = 1; i < n; i++) {
        Buah key = ptr[i];
        int j = i - 1;
        if (ascending) {
            while (j >= 0 && ptr[j].harga > key.harga) {
                ptr[j+1] = ptr[j];
                j--;
            }
        } else {
            while (j >= 0 && ptr[j].harga < key.harga) {
                ptr[j+1] = ptr[j];
                j--;
            }
        }
        ptr[j+1] = key;
    }
}

// 4. Shell Sort
void shellSort(Buah *ptr, int n, bool ascending) {
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Buah temp = ptr[i];
            int j;
            if (ascending) {
                for (j = i; j >= gap && ptr[j-gap].harga > temp.harga; j -= gap)
                    ptr[j] = ptr[j-gap];
            } else {
                for (j = i; j >= gap && ptr[j-gap].harga < temp.harga; j -= gap)
                    ptr[j] = ptr[j-gap];
            }
            ptr[j] = temp;
        }
    }
}

// 5. Quick Sort
int partition(Buah *ptr, int low, int high, bool ascending) {
    int pivot = ptr[high].harga;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (ascending) {
            if (ptr[j].harga <= pivot) {
                i++;
                swapBuah(&ptr[i], &ptr[j]);
            }
        } else {
            if (ptr[j].harga >= pivot) {
                i++;
                swapBuah(&ptr[i], &ptr[j]);
            }
        }
    }
    swapBuah(&ptr[i+1], &ptr[high]);
    return i+1;
}

void quickSort(Buah *ptr, int low, int high, bool ascending) {
    if (low < high) {
        int pi = partition(ptr, low, high, ascending);
        quickSort(ptr, low, pi-1, ascending);
        quickSort(ptr, pi+1, high, ascending);
    }
}

// 6. Merge Sort
void merge(Buah *ptr, int left, int mid, int right, bool ascending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Buah *L = new Buah[n1];
    Buah *R = new Buah[n2];
    for (int i = 0; i < n1; i++) L[i] = ptr[left + i];
    for (int j = 0; j < n2; j++) R[j] = ptr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    if (ascending) {
        while (i < n1 && j < n2) {
            if (L[i].harga <= R[j].harga) ptr[k++] = L[i++];
            else ptr[k++] = R[j++];
        }
    } else {
        while (i < n1 && j < n2) {
            if (L[i].harga >= R[j].harga) ptr[k++] = L[i++];
            else ptr[k++] = R[j++];
        }
    }
    while (i < n1) ptr[k++] = L[i++];
    while (j < n2) ptr[k++] = R[j++];
    delete[] L;
    delete[] R;
}

void mergeSort(Buah *ptr, int left, int right, bool ascending) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(ptr, left, mid, ascending);
        mergeSort(ptr, mid+1, right, ascending);
        merge(ptr, left, mid, right, ascending);
    }
}