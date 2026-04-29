#include <iostream>
#include <stdio.h>
#include <iomanip>
using namespace std;

int sama(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

struct Kamar {
    char nama[50];
    char bed[30];
    int kapasitas;
    int harga;
    int tersedia;
    Kamar *next;
};

struct Pemesanan {
    char username[30];
    char namaLengkap[50];
    char noHp[20];
    char email[50];
    char namaKamar[50];
    int jumlahMalam;
    int jumlahTamu;
    char checkin[20];
    char checkout[20];
    int total;
};

Kamar *headKamar = NULL;
Pemesanan pesan[50];
int jumlahPesan = 0;
char userLogin[30];

int login() {
    FILE *fp = fopen("user.txt", "r");
    if (fp == NULL) return 0;

    char user[30], pass[30];
    cout << "\n========== StayEase Hotel Login ==========\n";
    cout << "Username : "; 
    cin >> user;
    cout << "Password : "; 
    cin >> pass;
    cout << "-----------------------------------------\n";

	cout << "Login berhasil! Selamat datang, " << user << "!\n";
	system("cls");

    char u[30], p[30];
    while (fscanf(fp, "%s %s", u, p) != EOF) {
        if (sama(user, u) && sama(pass, p)) {

            int i = 0;
            while (user[i] != '\0') {
                userLogin[i] = user[i];
                i++;
            }
            userLogin[i] = '\0';

            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void tambahKamar(const char nama[], const char bed[], int kapasitas, int harga) {
    Kamar *baru = new Kamar;

    int i = 0;
    while (nama[i] != '\0') {
        baru->nama[i] = nama[i];
        i++;
    }
    baru->nama[i] = '\0';

    i = 0;
    while (bed[i] != '\0') {
        baru->bed[i] = bed[i];
        i++;
    }
    baru->bed[i] = '\0';

    baru->kapasitas = kapasitas;
    baru->harga = harga;
    baru->tersedia = 1;
    baru->next = NULL;

    if (headKamar == NULL) headKamar = baru;
    else {
        Kamar *bantu = headKamar;
        while (bantu->next != NULL) bantu = bantu->next;
        bantu->next = baru;
    }
}

void tampilSorting(int asc) {
    Kamar *arr[50];
    int n = 0;

    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        arr[n++] = bantu;
        bantu = bantu->next;
    }

    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if ((asc && arr[j]->harga > arr[j+1]->harga) || (!asc && arr[j]->harga < arr[j+1]->harga)) {

                Kamar *temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    cout << "\n============================================================================\n";
    cout << "|                             PRICELIST KAMAR HOTEL                        |";
    cout << "\n============================================================================\n";
    cout << "| ";
	cout << left << setw(4)  << "No" << setw(20) << "Nama Kamar" << setw(18) << "Tipe Bed" << setw(6)  << "Kap" << setw(16) << "Harga/Malam" << setw(8) << "Status" << " |\n";
    cout << "----------------------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
		cout << "| ";
		cout << left << setw(4)  << i+1 << setw(20) << arr[i]->nama << setw(18) << arr[i]->bed << setw(6)  << arr[i]->kapasitas << "Rp " << setw(12) << arr[i]->harga  << setw(9) << (arr[i]->tersedia ? "Tersedia" : "Penuh") << " |\n";
}
    cout << "============================================================================\n";
}

Kamar* cariKamar(char nama[]) {
    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        if (sama(bantu->nama, nama)) return bantu;
        bantu = bantu->next;
    }
    return NULL;
}

void pesanKamar() {
    tampilSorting(1);

    char nama[50];
    cout << "\nMasukkan nama kamar: ";
    cin.ignore();
    cin.getline(nama, 50);

    Kamar *k = cariKamar(nama);

    if (k == NULL) {
        cout << "Kamar tidak ditemukan\n";
        return;
    }

    if (!k->tersedia) {
        cout << "Kamar penuh\n";
        return;
    }

    Pemesanan p;

    int i = 0;
    while (userLogin[i] != '\0') {
        p.username[i] = userLogin[i];
        i++;
    }
    p.username[i] = '\0';

    i = 0;
    while (k->nama[i] != '\0') {
        p.namaKamar[i] = k->nama[i];
        i++;
    }
    p.namaKamar[i] = '\0';
    
    cout << "\n========== DATA PEMESAN ==========\n";
    cout << "Nama Lengkap      : ";
    cin.getline(p.namaLengkap, 50);
    cout << "No Handphone      : "; 
    cin.getline(p.noHp, 20);
    cout << "Email             : "; 
    cin.getline(p.email, 50);
    cout << "Jumlah Malam      : ";
    cin >> p.jumlahMalam;
    cout << "Jumlah Tamu       : ";
	cin >> p.jumlahTamu;
	cin.ignore();

	cout << "Tanggal Check-in  : ";
	cin.getline(p.checkin, 20);
	cout << "Tanggal Check-out : ";
	cin.getline(p.checkout, 20);

    if (p.jumlahMalam <= 0) {
        cout << "Jumlah malam tidak valid\n";
        return;
    }
    p.total = p.jumlahMalam * k->harga;

    char y;
    cout << "\nKonfirmasi pemesanan? (y/t): ";
    cin >> y;

    if (y == 'y' || y == 'Y') {
        k->tersedia = 0;
        pesan[jumlahPesan++] = p;
        cout << "Pemesanan berhasil!\n";
    } else {
        cout << "Pemesanan dibatalkan\n";
    }
}

void riwayat() {
    cout << "\n========== RIWAYAT PEMESANAN ==========\n";

    if (jumlahPesan == 0) {
        cout << "Belum ada pesanan\n";
        return;
    }

    for (int i = 0; i < jumlahPesan; i++) {
        if (sama(pesan[i].username, userLogin)) {

            cout << "Kamar             : " << pesan[i].namaKamar << endl;
            cout << "Harga/malam       : Rp " << (pesan[i].total / pesan[i].jumlahMalam) << endl;
            cout << "Jumlah malam      : " << pesan[i].jumlahMalam << endl;
            cout << "Jumlah tamu 	  : " << pesan[i].jumlahTamu << endl;
			cout << "Tanggal Check-in  : " << pesan[i].checkin << endl;
			cout << "Tanggal Check-out : " << pesan[i].checkout << endl;
            cout << "Total bayar       : Rp " << pesan[i].total << endl;

            cout << "\n========== INFORMASI PEMESAN ==========\n";
            cout << left << setw(15) << "Username"     << ": " << pesan[i].username << endl;
            cout << left << setw(15) << "Nama Lengkap" << ": " << pesan[i].namaLengkap << endl;
            cout << left << setw(15) << "No Handphone" << ": " << pesan[i].noHp << endl;
            cout << left << setw(15) << "Email"        << ": " << pesan[i].email << endl;

            cout << "\n========== DOKUMEN YANG DIPERLUKAN ===============\n";
            cout << "Saat check-in, Anda wajib membawa Kartu Identitas.\n";
            cout << "Dokumen yang wajib dibawa dalam bentuk hard copy.\n";

            cout << "----------------------------------------------------\n";
        }
    }
}

void saveKamar() {
    FILE *fp = fopen("kamar.txt", "w");

    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        fprintf(fp, "%s;%s;%d;%d;%d\n",
            bantu->nama,
            bantu->bed,
            bantu->kapasitas,
            bantu->harga,
            bantu->tersedia
        );
        bantu = bantu->next;
    }

    fclose(fp);
}

void loadKamar() {
    FILE *fp = fopen("kamar.txt", "r");
    if (fp == NULL) return;

    char nama[50], bed[30];
    int kap, harga, status;

    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d\n",
        nama, bed, &kap, &harga, &status) != EOF) {

        tambahKamar(nama, bed, kap, harga);
    }

    fclose(fp);
}

int main() {

    loadKamar();

    if (headKamar == NULL) {
        tambahKamar("Premium", "1 Double Bed", 2, 1215000);
        tambahKamar("The Level", "1 Double Bed", 2, 1566000);
        tambahKamar("Junior Suite", "1 King Bed", 2, 2694000);
        tambahKamar("Kamar Keluarga", "1 Double Bed", 4, 2800000);
        tambahKamar("The Level Suite", "1 King Bed", 2, 3320000);
        tambahKamar("Family Suite", "1 Double Bed", 4, 4700000);
        tambahKamar("Suite Presiden", "1 King Bed", 4, 7100000);
    }

    if (!login()) {
        cout << "Login gagal\n";
        return 0;
    }

    int pilih; 

    do {  

        cout << "\n+---------------------------------+\n";
        cout << "|          StayEase Hotel         |\n";
        cout << "+---------------------------------+\n";
        cout << "|1. Lihat Pricelist Kamar         |\n";     
        cout << "|2. Pesan Kamar                   |\n";           
        cout << "|3. Riwayat Pemesanan             |\n";
        cout << "|4. Keluar                        |\n";
        cout << "+---------------------------------+\n";
        cout << "Pilih menu (1-4): ";
        cin >> pilih;

        if (pilih < 1 || pilih > 4) {
            cout << "Pilihan tidak valid\n";
            continue;
        }

        if (pilih == 1) {
            int urut;
            cout << "\nLIHAT PRICELIST KAMAR SECARA:\n";
            cout << "1. Ascending\n";
            cout << "2. Descending\n";
            cout << "Pilih: ";
            cin >> urut;

            if (urut == 1) tampilSorting(1);
            else if (urut == 2) tampilSorting(0);
        }
        else if (pilih == 2) {
            pesanKamar();
        }
        else if (pilih == 3) {
            riwayat();
        }

    } while (pilih != 4); 

    saveKamar();

    return 0;
}
