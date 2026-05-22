#include <iostream>
#include <stdio.h>
#include <cstring>
#include <iomanip>
using namespace std;


//fungsi buat ngecek apakah dua kata atau  kalimat itu sama persis atau engga, 
//kalau sama return 1 kalau beda return 0
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
    int tersedia; //1= masih bisa dipesen = = sudah penuh
    Kamar *next;
};

struct Pemesanan {
    char username[30];
    char namaLengkap[50];
    char noHp[20];
    char email[50];
    char namaKamar[50];
    int jumlahMalam;
    int tglMasuk, blnMasuk, thnMasuk;
    int tglKeluar, blnKeluar, thnKeluar;
    int jumlahTamu;
    char status[20];
    int total;
};

//variabel global yang dipakek di semua proggram
Kamar *headKamar = NULL; // pintu masuk ke linked list kamar
Pemesanan pesan[50]; //semua data disimpan disini
int jumlahPesan = 0;
char userLogin[30];

int login() {
    FILE *fp;
    char user[30], pass[30];
    char u[30], p[30];

    int kesempatan = 3; //dikasih 3 kali kesempatan, kalau dalam 3 kali gagal program berhenti

    while (kesempatan > 0) {

        fp = fopen("user.txt", "r");

        if (fp == NULL) {
            cout << "File user tidak ditemukan!\n";
            return 0;
        }

        cout << "\n========== StayEase Hotel Login ==========\n";
        cout << "Username : ";
        cin >> user;

        cout << "Password : ";
        cin >> pass;

        int userBenar = 0;
        int passBenar = 0;

        //cek satu persatu bari di file dan dicocokan sama usn dan pw
        while (fscanf(fp, "%s %s", u, p) != EOF) {

            if (sama(user, u)) {
                userBenar = 1;

                if (sama(pass, p)) {
                    passBenar = 1;

                    //login berhasil habis itu disimpen ke variabel global
                    strcpy(userLogin, user);

                    fclose(fp);

                    cout << "Login berhasil! Selamat datang, " << userLogin << "!\n";

                    system("pause"); //buat berhentiin program sementara sampai user pencet lanjut
                    system("cls"); //buat bersihin layar console, jadi nnti lanjut ke menu berikutnya

                    return 1;
                }

                break;
            }
        }

        fclose(fp);

        kesempatan--;

        if (userBenar && !passBenar) {
            cout << "Password salah! Sisa kesempatan: "
                 << kesempatan << endl;
        }
        else if (!userBenar) {
            cout << "Username salah! Sisa kesempatan: "
                 << kesempatan << endl;
        }

        system("pause");
        system("cls");
    }

    cout << "Anda gagal login 3 kali.\n";

    return 0;
}


void tambahKamar(const char nama[], const char bed[], int kapasitas, int harga) {
    Kamar *baru = new Kamar; //minta memori baru buat kamar ini

    //salin nma dan tipe bed karakter per karakter
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

    if (headKamar == NULL) headKamar = baru; //kalau list kosong kamar ini langsung jadi yang pertama
    else {
        Kamar *bantu = headKamar;
        while (bantu->next != NULL) bantu = bantu->next;
        bantu->next = baru;
    }
}

void tampilSorting(int asc) { // menampilkan urutan daftar kamar yg diuritin berdasarkan harga
    // linked list dipindahin ke array sementara supaya bisa disorting
    Kamar *arr[50];
    int n = 0;

    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        arr[n++] = bantu;
        bantu = bantu->next;
    }

    for (int i = 0; i < n-1; i++) { //membandingkan dua elemen terdekat kalau salah baru ditukar
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

Kamar* cariKamar(char nama[]) { // cari kamar berdasarkan nama, kalau ketemu kembali ke pointer kalau ga kembali ke null
    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        if (sama(bantu->nama, nama)) return bantu;
        bantu = bantu->next;
    }
    return NULL;
}

void pesanKamar() {
    tampilSorting(1);

    char nama[50]; // status kamar diubah jadi penuh setelah berhasil dipesan
    cout << "\nMasukkan nama kamar: ";
    cin.ignore(); // membuang sisa newline dari input sblmnya
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
    strcpy(p.status, "Belum Bayar");

    //menyalin usn yg lagi login ke data pemesanan
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

    int validNama;  //nama  hanya boleh huruf sama spasi, gblh angka sama simbol nnti ditolak
    do {
        cout << "Nama Lengkap      : ";
        cin.getline(p.namaLengkap, 50);

        validNama = 1;

        if (p.namaLengkap[0] == '\0') validNama = 0;

        for (int i = 0; p.namaLengkap[i] != '\0'; i++) {
            if (!( (p.namaLengkap[i] >= 'A' && p.namaLengkap[i] <= 'Z') ||
                   (p.namaLengkap[i] >= 'a' && p.namaLengkap[i] <= 'z') ||
                   p.namaLengkap[i] == ' ' )) {
                validNama = 0;
                break;
            }
        }

        if (!validNama) {
            cout << "Nama hanya boleh huruf!\n";
        }

    } while (!validNama);

    int valid;
    do {
        cout << "No Handphone      : ";
        cin.getline(p.noHp, 20);

        valid = 1;

        if (p.noHp[0] == '\0') valid = 0;

        for (int i = 0; p.noHp[i] != '\0'; i++) {
            if (p.noHp[i] < '0' || p.noHp[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            cout << "No HP harus angka!\n";
        }

    } while (!valid);

    cout << "Email             : ";
    cin.getline(p.email, 50);

    do {
        cout << "Jumlah Tamu       : ";
        cin >> p.jumlahTamu;

        if (cin.fail()) { //dipakek buat nangkep user iseng input huruf
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input harus angka!\n";
            p.jumlahTamu = 0;
            continue;
        }

        if (p.jumlahTamu <= 0) {
            cout << "Jumlah tamu tidak valid!\n";
        }

    } while (p.jumlahTamu <= 0);
        int tglMasuk, blnMasuk, thnMasuk;
        int tglKeluar, blnKeluar, thnKeluar;

    do {
        cout << "Tanggal Check-in (dd mm yyyy)  : "; 
        cin >> tglMasuk >> blnMasuk >> thnMasuk;
        cout << "Tanggal Check-out (dd mm yyyy) : ";
        cin >> tglKeluar >> blnKeluar >> thnKeluar;

        if (thnKeluar < thnMasuk || (thnKeluar == thnMasuk && blnKeluar < blnMasuk) || (thnKeluar == thnMasuk && blnKeluar == blnMasuk && tglKeluar <= tglMasuk)) {
            cout << "Tanggal tidak valid!\n";
        }

    } while (thnKeluar < thnMasuk || (thnKeluar == thnMasuk && blnKeluar < blnMasuk) || (thnKeluar == thnMasuk && blnKeluar == blnMasuk && tglKeluar <= tglMasuk));

        int totalMasuk = thnMasuk * 365 + blnMasuk * 30 + tglMasuk; //pakek angka gede itu karena biar mudah diitung selisihnya
        int totalKeluar = thnKeluar * 365 + blnKeluar * 30 + tglKeluar;

    p.jumlahMalam = totalKeluar - totalMasuk;
    cout << "Jumlah malam      : " << p.jumlahMalam << endl;

    p.tglMasuk = tglMasuk;
    p.blnMasuk = blnMasuk;
    p.thnMasuk = thnMasuk;

    p.tglKeluar = tglKeluar;
    p.blnKeluar = blnKeluar;
    p.thnKeluar = thnKeluar;

    p.total = p.jumlahMalam * k->harga; //total biaya

    char y;
    cout << "\nKonfirmasi pemesanan? (y/t): ";
    cin >> y;

    if (y == 'y' || y == 'Y') {

        k->tersedia = 0;
        pesan[jumlahPesan++] = p; //nyimpen ke  daftar pemesanan

        cout << "Pemesanan berhasil!\n";

        char bayar;
        cout << "\nBayar sekarang? (y/t): ";
        cin >> bayar;

        if (bayar == 'y' || bayar == 'Y') {

            int idx = jumlahPesan - 1;

            cout << "\n===== DETAIL PEMBAYARAN =====\n";
            cout << "Nama Pemesan  : " << pesan[idx].namaLengkap << "\n";
            cout << "Kamar         : " << pesan[idx].namaKamar << "\n";
            cout << "Jumlah Malam  : " << pesan[idx].jumlahMalam << " malam\n";
            cout << "Total Bayar   : Rp " << pesan[idx].total << "\n";

            int uang;
            do {
                cout << "Masukkan uang : Rp ";
                cin >> uang;

                if (uang < pesan[idx].total) {
                    cout << "Uang kurang!\n";
                }

            } while (uang < pesan[idx].total);

            int kembali = uang - pesan[idx].total;
            strcpy(pesan[idx].status, "Lunas"); //update ststus jadi lunas

            cout << "\n===== PEMBAYARAN BERHASIL =====\n";
            cout << "Total     : Rp " << pesan[idx].total << "\n";
            cout << "Dibayar   : Rp " << uang << "\n";
            cout << "Kembali   : Rp " << kembali << "\n";

        } else {
            cout << "Bayar nanti lewat menu pembayaran.\n";
        }

    } else {
        cout << "Pemesanan dibatalkan\n";
    }
}

//hapus kamar berdasarkan nama 
void hapusKamar() {
    if(headKamar  == NULL) {
        cout << "Daftar kamar kosong\n";
        return;
    }

    char nama[50];
    cout << "Masukkan nama kamar yang ingin dihapus: ";
    cin.ignore();
    cin.getline(nama,50);

    if(strcmp(headKamar->nama, nama) == 0) { //kalau mau yg dihapus kamar pertama
        Kamar *hapus = headKamar;
        headKamar = headKamar->next;
        delete hapus;
        cout << "Daftar kamar berhasil dihapus\n";
        return;
}

    Kamar *temp = headKamar;
    while(temp->next != NULL && strcmp(temp->next->nama, nama) != 0) {
        temp = temp->next;
    }

    if (temp->next == NULL) {
        cout << "Daftar kamar tidak ditemukan\n";
        return;
    }

    Kamar *hapus = temp->next;
    temp->next = temp->next->next; //  sambung langsung lewati kammar yg  dihapus
    delete hapus;
    cout << "Daftar kamar berhasil dihapus\n";
    
}

void updateKamar() {
    if(headKamar == NULL) {
        cout << "Daftar kamar kosong\n";
        return;
    }

    char nama[50];
    cout << "Masukkan nama kamar yang ingin diupdate: ";
    cin.ignore();
    cin.getline(nama, 50);

    Kamar *temp = headKamar;
    while(temp != NULL && strcmp(temp->nama, nama) != 0) {
        temp = temp->next;
    }

    if(temp == NULL) {
        cout << "Kamar tidak ditemukan!\n";
        return; 
    }

    cout << "\nKamar ditemukan!\n";
    cout << "Nama Kamar       : " << temp->nama << endl;
    cout << "Tipe Bed         : " << temp->bed << endl;
    cout << "Kapasitas        : " << temp->kapasitas << endl;
    cout << "Harga/malam      : " << temp->harga << endl; 
    cout << "Status           : " << (temp->tersedia ? "Tersedia" :  "Penuh") << endl;

    int pilih;
    cout << "\nPilih data yang ingin diupdate:\n";
    cout << "1. Nama Kamar\n";
    cout << "2. Tipe Bed\n";
    cout << "3. Kapasitas\n";
    cout << "4. Harga/malam\n";
    cout << "5. Status\n";
    cout << "Pilih : ";
    cin >> pilih;

    if(pilih == 1) {
       cout << "Masukkan nama kamar baru : ";
       cin >> temp->nama; 
    }
    else if(pilih == 2) {
        cout << "Masukkan tipe bed baru : ";
        cin >> temp->bed;
    }
    else if(pilih == 3) {
        cout << "Masukkan kapasitas baru : ";
        cin >> temp->kapasitas;
    }
    else if(pilih == 4) {
		cout << "Masukkan harga baru : "; 
		cin >> temp->harga;
	}
	else if(pilih == 5) {
		cout << "Status (1 = tersedia, 0 = penuh): ";
        cin >> temp->tersedia;
	}
    else {
        cout << "Pilihan tidak valid!\n";
        return;
    }
    cout << "Data kamar berhasil diupdate!\n";
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
			cout << "Tanggal Check-in  : " << pesan[i].tglMasuk << "-" << pesan[i].blnMasuk << "-" << pesan[i].thnMasuk << endl;
            cout << "Tanggal Check-out : " << pesan[i].tglKeluar << "-" << pesan[i].blnKeluar << "-" << pesan[i].thnKeluar << endl;
            cout << "Total bayar       : Rp " << pesan[i].total << endl;
            cout << "Status            : " << pesan[i].status << endl;

            cout << "\n========== INFORMASI PEMESAN ==========\n";
            cout << left << setw(15) << "Username"     << ": " << pesan[i].username << endl;
            cout << left << setw(15) << "Nama Lengkap" << ": " << pesan[i].namaLengkap << endl;
            cout << left << setw(15) << "No Handphone" << ": " << pesan[i].noHp << endl;
            cout << left << setw(15) << "Email"        << ": " << pesan[i].email << endl;
            
            cout << "----------------------------------------------------\n";
        }
    }
    
    cout << "\n========== DOKUMEN YANG DIPERLUKAN ===============\n";
            cout << "Saat check-in, Anda wajib membawa Kartu Identitas.\n";
            cout << "Dokumen yang wajib dibawa dalam bentuk hard copy.\n";

            cout << "----------------------------------------------------\n";
}

void pembayaran() { //nampilin daftar pesanan yg blm dibayar sama proses pembayaranhya
    int indexBelumBayar[50];
    int jumlah = 0;

    for (int i = 0; i < jumlahPesan; i++) { //ngumpulin daftar pesanan yg blm bayar
        if (sama(pesan[i].username, userLogin) && strcmp(pesan[i].status, "Belum Bayar") == 0) {
            indexBelumBayar[jumlah++] = i;
        }
    }

    if (jumlah == 0) {
        cout << "\nTidak ada pesanan yang belum dibayar.\n";
        return;
    }

    cout << "\n===== PESANAN BELUM DIBAYAR =====\n";
    for (int i = 0; i < jumlah; i++) {
        int idx = indexBelumBayar[i];
        cout << i+1 << ". " << pesan[idx].namaKamar << " | Rp " << pesan[idx].total << endl;
    }

    int pilih;
    cout << "Pilih nomor pesanan: ";
    cin >> pilih;

    if (pilih < 1 || pilih > jumlah) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    int idx = indexBelumBayar[pilih - 1]; // ambil index asli dari array pesan

    int uang;
    do {
        cout << "Masukkan uang : Rp ";
        cin >> uang;
        if (uang < pesan[idx].total)
            cout << "Uang kurang!\n";
    } while (uang < pesan[idx].total);

    int kembali = uang - pesan[idx].total;
    strcpy(pesan[idx].status, "Lunas");

    cout << "\nPembayaran berhasil!\n";
    cout << "Kembalian: Rp " << kembali << endl;
}

void saveKamar() { //nyimpen semua data kamardari linked list ke file txt
    FILE *fp = fopen("kamar.txt", "w");

    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        fprintf(fp, "%s;%s;%d;%d;%d\n", bantu->nama, bantu->bed, bantu->kapasitas, bantu->harga, bantu->tersedia);
        bantu = bantu->next;
    }

    fclose(fp);
}

void menukamar() {
	int pilih;
	
	do {
		cout << "\n==== MENU KELOLA KAMAR ===\n";	
		cout << "1. Tampilkan Kamar\n";
		cout << "2. Tambahkan Kamar\n";
		cout << "3. Update Kamar\n";
		cout << "4. Hapus Kamar\n";
		cout << "5. Keluar\n";
		cout << "Pilih: ";
		cin >> pilih;
		
		switch (pilih) {
            case 1: {
                int urut;
                cout << "1. Ascending\n2. Descending\nPilih: ";
                cin >> urut;
                if (urut == 1) tampilSorting(1);
                else tampilSorting(0);
                break;
            }
            case 2: {
                char nama[50], bed[30];
                int kap, harga;

                cin.ignore();
                cout << "Nama kamar: ";
                cin.getline(nama, 50);
                cout << "Tipe bed: ";
                cin.getline(bed, 30);
                cout << "Kapasitas: ";
                cin >> kap;
                cout << "Harga: ";
                cin >> harga;

                tambahKamar(nama, bed, kap, harga);
                saveKamar(); //berguna buat mnyimpen langsung ke file  setelah diubah
                cout << "Kamar berhasil ditambah!\n";
                break;
            }
            case 3:
                updateKamar();
                saveKamar();
                break;
            case 4:
                hapusKamar();
                saveKamar();
                break;
            case 5:
                cout << "Kembali ke menu utama...\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilih != 5);
}

void loadKamar() {
    FILE *fp = fopen("kamar.txt", "r");
    if (fp == NULL) return; //file  blm ketemu? lewatin aje

    char nama[50], bed[30];
    int kap, harga, status;

    while (fscanf(fp, "%[^;];%[^;];%d;%d;%d\n", nama, bed, &kap, &harga, &status) != EOF) {
        tambahKamar(nama, bed, kap, harga);
    }

    fclose(fp);
}

void tampilTidakUrut() { //menampilkan kamar dalam urtan yg agak  acak
    Kamar *arr[50];
    int n = 0;

    Kamar *bantu = headKamar;
    while (bantu != NULL) {
        arr[n++] = bantu;
        bantu = bantu->next;
    }
    
    //tukar elemen pasangan
    for (int i = 0; i < n-1; i += 2) {
        Kamar *temp = arr[i];
        arr[i] = arr[i+1];
        arr[i+1] = temp;
    }

    cout << "\n============================================================================\n";
    cout << "|                             PRICELIST KAMAR HOTEL                        |\n";
    cout << "============================================================================\n";
    cout << "| " << left << setw(4)  << "No" << setw(20) << "Nama Kamar" << setw(18) << "Tipe Bed" << setw(6)  << "Kap" << setw(16) << "Harga/Malam" << setw(8)  << "Status" << " |\n";
    cout << "----------------------------------------------------------------------------\n";
    for (int i = n-1; i >= 0; i--) {
        cout << "| " << setw(4) << (n - i) << setw(20) << arr[i]->nama << setw(18) << arr[i]->bed << setw(6) << arr[i]->kapasitas << "Rp " << setw(12) << arr[i]->harga << setw(9) << (arr[i]->tersedia ? "Tersedia" : "Penuh")  << " |\n";
    }
    cout << "============================================================================\n";
}

void menuPricelist() {
    int pilih;

    do {
        system("cls");
        tampilTidakUrut(); 

        cout << "\n=== MENU PRICELIST ===\n";
        cout << "1. Sorting\n";
        cout << "2. Kelola Kamar\n";
        cout << "3. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: {
                int urut;
                cout << "\n=== SORTING ===\n";
                cout << "1. Ascending\n";
                cout << "2. Descending\n";
                cout << "Pilih: ";
                cin >> urut;

                if (urut == 1) tampilSorting(1);
                else if (urut == 2) tampilSorting(0);
                else cout << "Pilihan tidak valid\n";

                system("pause");
                break;
            }
            case 2:
                menukamar();
                break;
            case 3:
                break;
            default:
                cout << "Pilihan tidak valid\n";
                system("pause");
        }

    } while (pilih != 3);
}

int main() {

    loadKamar(); // membaca data kamar dari file 

    //kalau file kosong diisi sama kamar default ini
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

    do {  //perulangan menu utama
        cout << "\n------- WELCOME TO STAYEASE HOTEL------\n";
        cout << "\n+---------------------------------+\n";
        cout << "|          StayEase Hotel         |\n";
        cout << "+---------------------------------+\n";
        cout << "|1. Lihat Pricelist Kamar         |\n";     
        cout << "|2. Pesan Kamar                   |\n";           
        cout << "|3. Riwayat Pemesanan             |\n";
        cout << "|4. Pembayaran                    |\n";
        cout << "|5. Keluar                        |\n";
        cout << "+---------------------------------+\n";
        cout << "Pilih menu (1-5): ";
        cin >> pilih;

        if (pilih < 1 || pilih > 5) {
            cout << "Pilihan tidak valid\n";
            continue;
        }


			if (pilih == 1) {
				menuPricelist(); 
			}
			else if (pilih == 2) {
				pesanKamar();
			}
			else if (pilih == 3) {
				riwayat();
			}
			else if (pilih == 4) {
				
			pembayaran();
			}

    } while (pilih != 5); 

    saveKamar(); // nyimpen kondisi akhir kamar blm program ditutup

    return 0;
}