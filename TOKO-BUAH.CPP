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

// ==================== TAMPILAN ====================
void tampilkan(Buah *ptr) {
    if (jumlah == 0) {
        cout << "\n+----------------------------------+\n";
        cout << "|        DATA MASIH KOSONG        |\n";
        cout << "+----------------------------------+\n";
        return;
    }

    int pilihUrutan;
    cout << "\n+----------------------------------+\n";
    cout << "|   URUTAN BERDASARKAN HARGA      |\n";
    cout << "+----------------------------------+\n";
    cout << "| 1. Termurah -> Termahal (Asce)  |\n";
    cout << "| 2. Termahal -> Termurah (Desc)  |\n";
    cout << "+----------------------------------+\n";
    cout << "Pilihan urutan: ";
    cin >> pilihUrutan;

    bool ascending = (pilihUrutan == 1);

    int pilihMetode;
    cout << "\n+----------------------------------+\n";
    cout << "|        PILIH METODE SORTING      |\n";
    cout << "+----------------------------------+\n";
    cout << "| 1. Bubble Sort                   |\n";
    cout << "| 2. Selection Sort                |\n";
    cout << "| 3. Insertion Sort                |\n";
    cout << "| 4. Shell Sort                    |\n";
    cout << "| 5. Quick Sort                    |\n";
    cout << "| 6. Merge Sort                    |\n";
    cout << "+----------------------------------+\n";
    cout << "Pilihan metode: ";
    cin >> pilihMetode;

    switch(pilihMetode) {
        case 1: bubbleSort(ptr, jumlah, ascending); break;
        case 2: selectionSort(ptr, jumlah, ascending); break;
        case 3: insertionSort(ptr, jumlah, ascending); break;
        case 4: shellSort(ptr, jumlah, ascending); break;
        case 5: quickSort(ptr, 0, jumlah-1, ascending); break;
        case 6: mergeSort(ptr, 0, jumlah-1, ascending); break;
        default: cout << "Metode tidak valid, gunakan Bubble Sort.\n"; bubbleSort(ptr, jumlah, ascending);
    }

    cout << "\n+-----+--------------------------+--------+------------+\n";
    cout << "| No  | Nama Buah                | Stok   | Harga/kg   |\n";
    cout << "+-----+--------------------------+--------+------------+\n";

    for (int i = 0; i < jumlah; i++) {
        cout << "| " << left << setw(3) << i + 1 << " | "
             << left << setw(24) << (ptr + i)->nama << " | "
             << right << setw(6) << (ptr + i)->stok << " | "
             << right << setw(10) << (ptr + i)->harga << " |\n";
    }
    cout << "+-----+--------------------------+--------+------------+\n";
}

// Binary Search (harus terurut berdasarkan nama)
int binarySearch(Buah *ptr, string cari) {
    int kiri = 0, kanan = jumlah - 1;
    while (kiri <= kanan) {
        int mid = (kiri + kanan) / 2;
        if ((ptr + mid)->nama == cari)
            return mid;
        else if ((ptr + mid)->nama < cari)
            kiri = mid + 1;
        else
            kanan = mid - 1;
    }
    return -1;
}

// Menu 3
void cariBuah(Buah *ptr) {
    if (jumlah == 0) {
        cout << "\n+----------------------------------+\n";
        cout << "|        DATA MASIH KOSONG        |\n";
        cout << "+----------------------------------+\n";
        return;
    }

    cin.ignore();
    string cari;
    cout << "\n========== CARI BUAH ==========\n";
    cout << "Masukkan nama buah: ";
    getline(cin, cari);

    // Sequential search 
    int hasil = -1;
    for (int i = 0; i < jumlah; i++) {
        if ((ptr + i)->nama == cari) {
            hasil = i;
            break;
        }
    }

    if (hasil != -1) {
        cout << "\n+----------------------------------+\n";
        cout << "|         BUAH DITEMUKAN!          |\n";
        cout << "+----------------------------------+\n";
        cout << "| Nama  : " << left << setw(25) << (ptr + hasil)->nama <<"|\n";
        cout << "| Stok  : " << setw(25) << to_string((ptr + hasil)->stok) <<"|\n";
        cout << "| Harga : " << setw(25) << to_string((ptr + hasil)->harga) <<"|\n";
        cout << "+----------------------------------+\n";
    } else {
        cout << "\n+----------------------------------+\n";
        cout << "|       DATA TIDAK DITEMUKAN       |\n";
        cout << "+----------------------------------+\n";
    }
}

// UPDATE STOK
void updateStok(Buah *ptr) {
    cin.ignore();
    string cari;
    bool ketemu = false;

    cout << "\n========== UPDATE STOK ==========\n";
    cout << "Nama buah: ";
    getline(cin, cari);

    for (int i = 0; i < jumlah; i++) {
        if ((ptr + i)->nama == cari) {
            (ptr + i)->stok = inputStok("Stok baru (0-100): ");
            ketemu = true;
            simpanFile(ptr, jumlah);
            break;
        }
    }

    if (ketemu)
        cout << "Stok berhasil diupdate!\n";
    else
        cout << "Buah tidak ditemukan.\n";
}

// ==================== OPERASI LINKED LIST (BARU) ====================
struct NodeBuah {
    Buah data;
    NodeBuah *next;
};

NodeBuah *head = nullptr;

void buatListKosong() {
    NodeBuah *current = head;
    while (current != nullptr) {
        NodeBuah *temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    cout << "\nLinked list telah dikosongkan.\n";
}
void sisipNodeDiAkhir() {
    cin.ignore();
    Buah buahBaru;
    cout << "\n========== SISIP NODE DI AKHIR ==========\n";
    cout << "Nama buah: ";
    getline(cin, buahBaru.nama);
    buahBaru.stok = inputStok("Stok (0-100): ");
    buahBaru.harga = inputAngka("Harga per kg: ");

    NodeBuah *newNode = new NodeBuah;
    newNode->data = buahBaru;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    } else {
        NodeBuah *temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newNode;
    }
    cout << "Node berhasil ditambahkan di akhir.\n";
}
void sisipNodeDiDepan() {
    cin.ignore();
    Buah buahBaru;
    cout << "\n========== SISIP NODE DI DEPAN ==========\n";
    cout << "Nama buah: ";
    getline(cin, buahBaru.nama);
    buahBaru.stok = inputStok("Stok (0-100): ");
    buahBaru.harga = inputAngka("Harga per kg: ");

    NodeBuah *newNode = new NodeBuah;
    newNode->data = buahBaru;
    newNode->next = head;
    head = newNode;
    cout << "Node berhasil ditambahkan di depan.\n";
}
