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