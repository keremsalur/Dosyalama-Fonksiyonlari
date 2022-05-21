#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct kayit{
int ogrNo;
int dersKodu;
int puan;
};

FILE *indexDosyasi,*veriDosyasi;
int dosyaBoyutu,kayitSayisi;

int main()
{
    dosyaBoyutunuOgren();
    int secim = 0;
    int anahtar = -1;
    printf("\tProlab 1. Proje: Dosyalama Islemleri\n");
    printf("----------------------------------------------------\n");
    printf("\t1 Index Dosyasi Olustur\n");
    printf("\t2 Kayit Ekle\n");
    printf("\t3 Veri Dosyasindan Kayit Bul\n");
    printf("\t4 Index Dosyasindan Kayit Bul\n");
    printf("\t5 Kayit Sil\n");
    printf("\t6 Kayit Guncelle\n");
    printf("\t7 Veri Dosyasini Goster\n");
    printf("\t8 Indeks Dosyasini Goster\n");
    printf("\t9 Indeks Dosyasini Sil\n");
    printf("\t0 Cikis\n");
    printf("----------------------------------------------------\n");
    while(secim != -1){
        printf("\nSecim yapiniz: ");
        scanf("%d",&secim);
        switch(secim){
            case 1 : printf("\tIndex Dosyasi Olustur\n"); indexDosyasiOlusturGuncelle(0);   break;//
            case 2 : printf("\tKayit Ekle\n"); kayitEkle(); break;//
            case 3 : printf("\tVeri Dosyasindan Kayit Bul\n\tAnahtar: "); scanf("%d",&anahtar); veriDosyasiKayitBul(anahtar); break;//
            case 4 : printf("\tIndex Dosyasindan Kayit Bul\n\tAnahtar: "); scanf("%d",&anahtar); indexDosyasiKayitBul(anahtar,0); break;//
            case 5 : printf("\tKayit Sil\n\tAnahtar: "); scanf("%d",&anahtar); indexDosyasiKayitBul(anahtar,1); break;//
            case 6 : printf("\tKayit Guncelle\n\tAnahtar: "); scanf("%d",&anahtar); indexDosyasiKayitBul(anahtar,2); break;//
            case 7 : printf("\tVeri Dosyasini Goster\n"); veriDosyasiniGoster(); break; //
            case 8 : printf("\tIndeks Dosyasini Goster\n"); indexDosyasiniGoster(); break;//
            case 9 : printf("\tIndeks Dosyasini Sil\n"); remove("indexDosyasi.txt"); printf("Index Dosyasi Silindi!\n"); break; //
            case 0 : printf("Cikis yapildi\n"); secim = -1; break; //
            default: printf("Lutfen gecerli bir secim yapiniz!\n");
        }
    }
    return 0;
}

void dosyaBoyutunuOgren(){
    veriDosyasi = fopen("veriDosyasi.bin","rb");
    fseek(veriDosyasi,0,SEEK_END);
    dosyaBoyutu = ftell(veriDosyasi);
    fclose(veriDosyasi);
    kayitSayisi = dosyaBoyutu / sizeof(struct kayit);
}

void veriDosyasiKayitBul(int anahtar){
    veriDosyasi = fopen("veriDosyasi.bin","rb");
    if(veriDosyasi != NULL){
        struct kayit okunanKayit;
        int ilk = 1;
        for(int i = 0; i < kayitSayisi; i++){
            fread(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
            if(okunanKayit.ogrNo == anahtar){
                if(ilk){
                    printf("-------------------------\nIlk Kayit\n\tOgrenci No: %d\n\tDers Kodu: %d\n\tPuan: %d\n-------------------------\n",okunanKayit.ogrNo,okunanKayit.dersKodu,okunanKayit.puan);
                    ilk = 0;
                }
                else{
                    printf("Kayit\n\tOgrenci No: %d\n\tDers Kodu: %d\n\tPuan: %d\n",okunanKayit.ogrNo,okunanKayit.dersKodu,okunanKayit.puan);
                }

            }
        }
        fclose(veriDosyasi);
    }
    else{
        printf("Veri dosyasi mevcut degil lutfen olusturun!\n");
        return;
    }
}

void indexDosyasiKayitBul(int anahtar,int islem){
    printf("\nIstenen kayit:%d\n",anahtar);

    int anahtarlar[kayitSayisi];
    int adresler[kayitSayisi];
    indexDosyasi = fopen("indexDosyasi.txt","r");
    if(indexDosyasi != NULL){
        char satir [50];
        for(int i = 0; i < kayitSayisi; i++){
            fgets(satir,50,indexDosyasi);
            char *sAnahtar = strtok(satir,";");
            anahtarlar[i] = atoi(sAnahtar);
            sAnahtar = strtok(NULL,";");
            adresler[i] = atoi(sAnahtar);
        }
    }
    else{
        printf("Index dosyasi mevcut degil lutfen olusturun!\n");
        return;
    }
    fclose(indexDosyasi);
    int index = binarySearch(anahtarlar,0,kayitSayisi-1,anahtar);
    if(index != -1){
        printf("\n\tIndex Dosyasindaki %d. Kayit\n",index);
        if(islem == 0){
            /// Bul
            struct kayit okunanKayit;
            veriDosyasi = fopen("veriDosyasi.bin","rb");
            if(veriDosyasi != NULL){
                fseek(veriDosyasi,adresler[index],SEEK_CUR);
                fread(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
                printf("Kayit\n\tOgrenci No: %d\n\tDers Kodu: %d\n\tPuan: %d\n",okunanKayit.ogrNo,okunanKayit.dersKodu,okunanKayit.puan);
            }
            fclose(veriDosyasi);
        }
        else if(islem == 1){
            /// Sil
            veriDosyasi = fopen("veriDosyasi.bin","rb");
            FILE *geciciDosya = fopen("geciciDosya.bin","wb");
            struct kayit okunanKayit;
            for(int i = 0; i < kayitSayisi; i++){
                fread(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
                if(i == (int)(adresler[index] / sizeof(struct kayit))){
                    continue;
                }else{
                    fwrite(&okunanKayit,sizeof(struct kayit),1,geciciDosya);
                }
            }
            fclose(veriDosyasi);
            fclose(geciciDosya);
            remove("veriDosyasi.bin");
            rename("geciciDosya.bin","veriDosyasi.bin");
            dosyaBoyutunuOgren();
            printf("\n\tKayit Silindi\n");
            indexDosyasiOlusturGuncelle(1);
        }
        else if(islem == 2){
            /// Guncelle
            int yeniPuan;
            printf("\n\t\tYeni Puani Girin: "); scanf("%d",&yeniPuan);
            veriDosyasi = fopen("veriDosyasi.bin","r+b");
            struct kayit okunanKayit;
            for(int i = 0; i < kayitSayisi; i++){
                fread(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
                if(i == (int)(adresler[index] / sizeof(struct kayit))){
                    okunanKayit.puan = yeniPuan;
                    fseek(veriDosyasi,-1 * sizeof(struct kayit),SEEK_CUR);
                    fwrite(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
                    break;
                }
            }
            fclose(veriDosyasi);
        }
    }
    else{
        printf("Aranan anahtar index dosyasinda bulunamadi.\n");
    }
}

int binarySearch(int dizi[], int alt, int ust, int anahtar)
{
    if (ust >= alt) {
        int ort = alt + (ust - alt) / 2;
        if (dizi[ort] == anahtar)
            return ort;
        if (dizi[ort] > anahtar)
            return binarySearch(dizi, alt, ort - 1, anahtar);
        return binarySearch(dizi, ort + 1, ust, anahtar);
    }
    return -1;
}

void kayitEkle(){
    veriDosyasi = fopen("veriDosyasi.bin","ab");
    struct kayit yeniKayit;
    printf("Ogrenci No: ");
    scanf("%d",&yeniKayit.ogrNo);
    printf("\nDers Kodu: ");
    scanf("%d",&yeniKayit.dersKodu);
    printf("\nPuan: ");
    scanf("%d",&yeniKayit.puan);
    fwrite(&yeniKayit,sizeof(struct kayit),1,veriDosyasi);
    fclose(veriDosyasi);
    dosyaBoyutunuOgren();
    indexDosyasiOlusturGuncelle(1);
}

void veriDosyasiniGoster(){
    veriDosyasi = fopen("veriDosyasi.bin","rb");
    struct kayit okunanKayit;
    for(int i = 0; i < kayitSayisi; i++){
        fread(&okunanKayit,sizeof(struct kayit),1,veriDosyasi);
        printf("%d. Kayit\n\tOgrenci No: %d\n\tDers Kodu: %d\n\tPuan: %d\n",i,okunanKayit.ogrNo,okunanKayit.dersKodu,okunanKayit.puan);
    }
    fclose(veriDosyasi);

}

void indexDosyasiOlusturGuncelle(int guncelle){
    indexDosyasi = fopen("indexDosyasi.txt","r");
    if(indexDosyasi == NULL || guncelle == 1){
        if(guncelle == 0)
            printf("Index dosyasi olusturuluyor.\n");
        else
            printf("Index dosyasi guncelleniyor.\n");

        indexDosyasi = fopen("indexDosyasi.txt","w");
        struct kayit kayitlar[kayitSayisi];
        int adresler[kayitSayisi];
        veriDosyasi = fopen("veriDosyasi.bin","rb");
        for(int i = 0; i < kayitSayisi; i++){
            fread(&(kayitlar[i]),sizeof(struct kayit),1,veriDosyasi);
            adresler[i] = i * sizeof(struct kayit);
        }
        fclose(veriDosyasi);

        for(int j = 0; j < kayitSayisi -1; j++){
            for(int k = 0; k < kayitSayisi - j -1; k++){
                if(kayitlar[k].ogrNo > kayitlar[k+1].ogrNo){
                    struct kayit temp = kayitlar[k];
                    kayitlar[k] = kayitlar[k+1];
                    kayitlar[k+1] = temp;

                    int tempAdres = adresler[k];
                    adresler[k] = adresler[k+1];
                    adresler[k+1] = tempAdres;

                }
            }
        }

        for(int n = 0; n < kayitSayisi; n++)
            fprintf(indexDosyasi,"%d;%d\n",kayitlar[n].ogrNo,adresler[n]);

        if(guncelle == 0)
            printf("Index dosyasi olusturuldu.\n");
        else
            printf("Index dosyasi guncellendi.\n");

    }
    else{
        printf("Index dosyasi zaten mevcut.\n");
    }
    fclose(indexDosyasi);
}

void indexDosyasiniGoster(){
    indexDosyasi = fopen("indexDosyasi.txt","r");
    if(indexDosyasi != NULL){
        char satir [50];
        printf("\n\tAnahtar\tAdres\n");
        for(int i = 0; i < kayitSayisi; i++){
            fgets(satir,50,indexDosyasi);
            char *anahtarString = strtok(satir,";");
            printf("%d ->\t%.4s\t%.4s",i,anahtarString,strtok(NULL,";"));
        }
    }
    else{
        printf("Index dosyasi mevcut degil lutfen olusturun!\n");
    }
    fclose(indexDosyasi);
}

