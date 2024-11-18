#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 100

// Evcil hayvan yapısı (struct)


struct EvcilHayvan {
    char tur[MAX_LENGTH];
    char ad[MAX_LENGTH];
    int enerji;
    int mutluluk;
    char duygu_durumu[MAX_LENGTH];
};

void hayvanlar_duygu_durumu(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    FILE *dosya = fopen("duygu.txt", "w");
    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    for (int i = 0; i < hayvanSayisi; i++) {
        if (hayvanlar[i].enerji > 25 && hayvanlar[i].mutluluk > 50) {
            fprintf(dosya, "%s  ,mutlu , :) , es>25 , ms>50 \n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Mutlu");
            
        } else if (hayvanlar[i].enerji < 20 && hayvanlar[i].mutluluk < 20) {
            fprintf(dosya, "%s  ,üzgün , :( , es<20 , ms<20 \n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Uzgun");
            
        } else if (hayvanlar[i].enerji < 10 && hayvanlar[i].mutluluk < 10) {
            fprintf(dosya, "%s  ,Agliyor , :( , es<10 , ms<10\n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Agliyor");
            
        } else if (hayvanlar[i].enerji < 5) {
            fprintf(dosya, "%s  ,Uyuyor , :( , es<5\n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Uyuyor");
            
        } else if (hayvanlar[i].enerji < 20) {
            fprintf(dosya, "%s  ,Ac , :( , es<20\n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Ac");
            
        } else if (hayvanlar[i].enerji > 40 || hayvanlar[i].mutluluk < 30) {
            fprintf(dosya, "%s  ,Oyun Istiyor , :( , es>20 , ms<30\n",hayvanlar[i].ad);
            strcpy(hayvanlar[i].duygu_durumu, "Oyun Istiyor");
        }
    }

    fclose(dosya);
}


// Evcil hayvanların duygu durumunu dosyadan okuyan ve ekrana yazan fonksiyon
void hayvanlar_duygu_durumu_oku(struct EvcilHayvan hayvanlar[], int *hayvanSayisi) {
    FILE *dosya = fopen("duygu.txt", "r");
    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    char ad[MAX_LENGTH];
    char duygu[10];

    printf("--- Evcil Hayvanların Duygu Durumu ---\n");

    while (fscanf(dosya, "%s: %s", ad, duygu) != EOF) {
        printf("%s: %s\n", ad, duygu);
    }

    fclose(dosya);
}

// Evcil hayvanları dosyadan okuyan fonksiyon
void evcilHayvanlariOku(struct EvcilHayvan hayvanlar[], int *hayvanSayisi) {
    FILE *dosya = fopen("evcil_veri.txt", "r");
    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    *hayvanSayisi = 0;
    while (fscanf(dosya, "%[^,],%[^,],%d,%d\n", hayvanlar[*hayvanSayisi].tur, hayvanlar[*hayvanSayisi].ad, &hayvanlar[*hayvanSayisi].enerji, &hayvanlar[*hayvanSayisi].mutluluk) == 4) {
        (*hayvanSayisi)++;

        
    }

    fclose(dosya);
}


// Evcil hayvanları ekrana yazan fonksiyon
void evcilHayvanlariYaz(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    FILE *dosya = fopen("evcil_veri.txt", "w");
    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    for (int i = 0; i < hayvanSayisi; i++) {
        fprintf(dosya, "%s,%s,%d,%d\n", hayvanlar[i].tur, hayvanlar[i].ad, hayvanlar[i].enerji, hayvanlar[i].mutluluk);
    }

    fclose(dosya);
}


// Evcil hayvanın bilgilerini dosyadan okuyan fonksiyon

int karsilastirString(char str1[], char str2[]) {
    int i = 0;

    while (str1[i] != '\0' || str2[i] != '\0') {
        if (str1[i] != str2[i])
            return 0;
        i++;
    }

    return 1;
}

void evcilHayvanaBesle(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
		if(hayvanSayisi <3){
		printf("Hayvan sayisi 3  veya daha fazla olmalidir !");
		return;
	}
    printf("Yemek verilecek evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi) != 0) {
            if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz  gerekiyor.\n");
                return;
            }
            hayvanlar[i].enerji += 15;
            hayvanlar[i].mutluluk += 10;
            hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  // hayvanların duygu durumunu güncelle


            // Dosyayı güncelle
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvana yemek verildi \n");
            fclose(dosya);
			printf("Beslenen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}

// Evcil hayvana oyun oynatma işlemi karsilastirString(hayvanlar[i].ad, hayvanAdi   ////  
void evcilHayvanaOyunOynat(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
		if(hayvanSayisi <3){
		printf("Hayvan sayisi 3  veya daha fazla olmalidir !");
		return;
	}
    printf("Oyun oynatilacak evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi) != 0) {
            if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz  gerekiyor.\n");
                return;
            }
            hayvanlar[i].enerji -= 15;
            hayvanlar[i].mutluluk += 15;
            hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  // hayvanların duygu durumunu güncelle

            // Dosyayı güncelle
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvan oyun oynatildi \n");
            fclose(dosya);
            printf("oynatılan hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
            return;
        }
        

    }

    printf("Evcil hayvan bulunamadi.\n");
}


// Evcil hayvanı temizleme işlemi
void evcilHayvaniTemizle(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
    if (hayvanSayisi < 3) {
        printf("Hayvan sayisi 3 veya daha fazla olmalidir!\n");
        return;
    }

    printf("Temizlenecek evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi) != 0) {
            if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz gerekiyor.\n");
                return;
            }

            hayvanlar[i].enerji -= 10;
            hayvanlar[i].mutluluk -= 5;

            

            hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  // hayvanların duygu durumunu güncelle

            printf("Evcil hayvan temizlendi.\n");

            // Dosyayı güncelle
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvan temizlendi \n");
            fclose(dosya);
			printf("Temizlenen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}





// Evcil hayvana sevgi gösterme işlemi
void evcilHayvanaSevgiGoster(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
    if (hayvanSayisi < 3) {
        printf("Hayvan sayisi 3 veya daha fazla olmalidir!\n");
        return;
    }

    printf("Sevgi gösterilecek evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi) != 0) {
            if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz gerekiyor.\n");
                return;
            }

            hayvanlar[i].mutluluk += 5;

            

            hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  // hayvanların duygu durumunu güncelle

            printf("Evcil hayvana sevgi gösterildi.\n");

            // Dosyayı güncelle
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvana sevgi gosterildi \n");
            fclose(dosya);
            printf("Sevgi gosterilen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}


// Evcil hayvanı uyutma işlemi
void evcilHayvaniUyut(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
    if (hayvanSayisi < 3) {
        printf("Hayvan sayisi 3 veya daha fazla olmalidir!\n");
        return;
    }

    printf("Uyutulacak evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi)) {
          
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz gerekiyor.\n");
                return;
            }
            hayvanlar[i].enerji += 5;

            hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  // hayvanların duygu durumunu güncelle

            printf("Evcil hayvan uyutuldu.\n");

            // Dosyayı güncelle
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvan uyutuldu \n");
            fclose(dosya);
           	printf("Uyutulan hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}

// Evcil hayvana ödül verme işlemi
void evcilHayvanaOdulVer(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
    if (hayvanSayisi < 3) {
        printf("Hayvan sayisi 3 veya daha fazla olmalidir!\n");
        return;
    }

    printf("Ödül verilecek evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi)) {
            if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz gerekiyor.\n");
                return;
            }
            hayvanlar[i].mutluluk += 10;
			hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  
            // Dosya adını oluştur
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            // Dosyayı aç ve güncelle
            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvana odul verildi \n");
            fclose(dosya);

          	printf("Odul verilen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}



void kopyalaString(char hedef[], char kaynak[]) {
    int i = 0;

    while (kaynak[i] != '\0') {
        hedef[i] = kaynak[i];
        i++;
    }

    hedef[i] = '\0';
}
// Evcil hayvanın bilgilerini dosyaya yazan fonksiyon


// Evcil hayvan ekleme işlemi
void evcilHayvanEkle(struct EvcilHayvan hayvanlar[], int* hayvanSayisi) {
    if (*hayvanSayisi == MAX_LENGTH) {
        printf("Maksimum evcil hayvan sayisina ulasildi.\n");
        return;
    }

    printf("Tur: ");
    scanf("%s", hayvanlar[*hayvanSayisi].tur);
    printf("Ad: ");
    scanf("%s", hayvanlar[*hayvanSayisi].ad);

    // Başlangıç enerji ve mutluluk seviyeleri
    hayvanlar[*hayvanSayisi].enerji = 60;
    hayvanlar[*hayvanSayisi].mutluluk = 60;

    // Duygu durumunu belirleme
    if (hayvanlar[*hayvanSayisi].enerji > 25 && hayvanlar[*hayvanSayisi].mutluluk > 50) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Mutlu");
    } else if (hayvanlar[*hayvanSayisi].enerji < 20 && hayvanlar[*hayvanSayisi].mutluluk < 20) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Üzgün");
    } else if (hayvanlar[*hayvanSayisi].enerji < 10 && hayvanlar[*hayvanSayisi].mutluluk < 10) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Ağlıyor");
    } else if (hayvanlar[*hayvanSayisi].enerji < 5) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Uyuyor");
    } else if (hayvanlar[*hayvanSayisi].enerji < 20) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Aç");
    } else if (hayvanlar[*hayvanSayisi].enerji > 40 || hayvanlar[*hayvanSayisi].mutluluk < 30) {
        strcpy(hayvanlar[*hayvanSayisi].duygu_durumu, "Oyun");
    }

    // Ayrı çıktı dosyası oluşturma
    char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
    strcpy(dosyaAdi, hayvanlar[*hayvanSayisi].ad);
    strcat(dosyaAdi, "_cikti.txt");

    FILE* dosya = fopen(dosyaAdi, "a");
    if (dosya == NULL) {
        printf("Dosya acilamadi.\n");
        return;
    }

    fprintf(dosya, "Tur: %s\n", hayvanlar[*hayvanSayisi].tur);
    fprintf(dosya, "Ad: %s\n", hayvanlar[*hayvanSayisi].ad);
    fprintf(dosya, "Enerji: %d\n", hayvanlar[*hayvanSayisi].enerji);
    fprintf(dosya, "Mutluluk: %d\n", hayvanlar[*hayvanSayisi].mutluluk);
    fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[*hayvanSayisi].duygu_durumu);
    fprintf(dosya, "Hayvan eklendi \n");
	

    fclose(dosya);
    (*hayvanSayisi)++;
    return ;
}



// Evcil hayvan güncelleme işlemi
void evcilHayvanGuncelle(struct EvcilHayvan hayvanlar[], int* hayvanSayisi) {
	
    char eskiAd[MAX_LENGTH], yeniAd[MAX_LENGTH];
	
    printf("Guncellenecek evcil hayvanin adini girin: ");
    scanf("%s", eskiAd);

    printf("Yeni adi girin: ");
    scanf("%s", yeniAd);

    int bulundu = 0;  // Evcil hayvanın bulunup bulunmadığını kontrol etmek için bir bayrak

    for (int i = 0; i < *hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, eskiAd)) {
            // Eski dosya adını oluştur
            char eskiDosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(eskiDosyaAdi, hayvanlar[i].ad);
            strcat(eskiDosyaAdi, "_cikti.txt");

            // Yeni dosya adını oluştur
            char yeniDosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(yeniDosyaAdi, yeniAd);
            strcat(yeniDosyaAdi, "_cikti.txt");

            // Dosyanın adını ve içeriğini değiştir
            if (rename(eskiDosyaAdi, yeniDosyaAdi) != 0) {
                printf("Dosya adi degistirilirken bir hata olustu.\n");
                return;
            }

            // Evcil hayvanın adını güncelle
            kopyalaString(hayvanlar[i].ad, yeniAd);

            // Dosyayı güncelle
            FILE* dosya = fopen(yeniDosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
			fprintf(dosya, "Hayvan guncellendi \n");
            fclose(dosya);
			printf("Guncelenenen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            bulundu = 1;  // Evcil hayvan bulunduğunda bayrağı 1 olarak ayarla
            break;
        }
    }

    if (!bulundu) {
        printf("Evcil hayvan bulunamadi.\n");
    }
}





// Evcil hayvan silme işlemi
void evcilHayvanSil(struct EvcilHayvan hayvanlar[], int *hayvanSayisi) {
    char silinecekAd[MAX_LENGTH];
	
    printf("Silinecek evcil hayvanin adini girin: ");
    scanf("%s", silinecekAd);

    int bulundu = 0;

    for (int i = 0; i < *hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, silinecekAd)) {
            // Dosya adını oluştur
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            // Dosyayı sil
            if (remove(dosyaAdi) != 0) {
                printf("Dosya silinirken bir hata olustu.\n");
                return;
            }

            for (int j = i; j < *hayvanSayisi - 1; j++) {
                kopyalaString(hayvanlar[j].ad, hayvanlar[j + 1].ad);
                hayvanlar[j].enerji = hayvanlar[j + 1].enerji;
                hayvanlar[j].mutluluk = hayvanlar[j + 1].mutluluk;
                kopyalaString(hayvanlar[j].duygu_durumu, hayvanlar[j + 1].duygu_durumu);
            }

            (*hayvanSayisi)--;
            printf("Evcil hayvan basariyla silindi.\n");
            bulundu = 1;
            break;
        }
    }

    if (!bulundu) {
        printf("Evcil hayvan bulunamadi.\n");
    }
}




// Evcil hayvana su verme işlemi
void evcilHayvanaSuVer(struct EvcilHayvan hayvanlar[], int hayvanSayisi) {
    char hayvanAdi[MAX_LENGTH];
	if(hayvanSayisi <3){
		printf("Hayvan sayisi 3  veya daha fazla olmalidir !");
		return;
	}
    printf("Su verilecek evcil hayvanin adini girin: ");
    scanf("%s", hayvanAdi);

    // Evcil hayvanları kontrol et
    for (int i = 0; i < hayvanSayisi; i++) {
        if (karsilastirString(hayvanlar[i].ad, hayvanAdi)) {
        	if (hayvanlar[i].enerji < 5) {
                printf("Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.\n");
                return;
            }
            if (hayvanlar[i].mutluluk < 5) {
                printf("Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz  gerekiyor.\n");
                return;
            }
            hayvanlar[i].enerji += 5;
            hayvanlar[i].mutluluk += 5;
			hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);  

            // Dosya adını oluştur
            char dosyaAdi[MAX_LENGTH + 4]; // ".txt" eklemek için +4 kullanıyoruz
            strcpy(dosyaAdi, hayvanlar[i].ad);
            strcat(dosyaAdi, "_cikti.txt");

            // Dosyayı aç ve güncelle
            FILE* dosya = fopen(dosyaAdi, "a"); // "w" modunda açarak dosyayı yazma moduna geçiyoruz
            if (dosya == NULL) {
                printf("Dosya acilirken bir hata olustu.\n");
                return;
            }

            fprintf(dosya, "Tur: %s\n", hayvanlar[i].tur);
            fprintf(dosya, "Ad: %s\n", hayvanlar[i].ad);
            fprintf(dosya, "Enerji: %d\n", hayvanlar[i].enerji);
            fprintf(dosya, "Mutluluk: %d\n", hayvanlar[i].mutluluk);
            fprintf(dosya, "Duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);
            fprintf(dosya, "Hayvana su verildi \n");

            fclose(dosya);
	
            printf("Su verilen hayvanın son duygu Durumu: %s\n", hayvanlar[i].duygu_durumu);

            return;
        }
    }

    printf("Evcil hayvan bulunamadi.\n");
}



// Ana fonksiyon
int main() {
	struct EvcilHayvan hayvanlar[MAX_LENGTH];
    int hayvanSayisi = 0;
    
    
    evcilHayvanlariOku(hayvanlar, &hayvanSayisi);
    
    hayvanlar_duygu_durumu(hayvanlar, hayvanSayisi);
   /* evcilHayvanlariYaz(hayvanlar, hayvanSayisi);
    hayvanlar_duygu_durumu_oku();*/
    int hayvanIndex = 0;
    


    int secim;
    do {
        printf("\n--- Evcil Hayvan Yönetim Sistemi ---\n");
        printf("1. Yeni evcil hayvan ekle\n");
        printf("2. Evcil hayvanı güncelle\n");
        printf("3. Evcil hayvanı sil\n");
        printf("4. Evcil hayvanı su ver\n");
        printf("5. Evcil hayvana oyun oynat\n");
        printf("6. Evcil hayvanı temizle\n");
        printf("7. Evcil hayvana sevgi göster\n");
        printf("8. Evcil hayvanı uyut\n");
        printf("9. Evcil hayvana odul ver\n");
        printf("10. Evcil hayvana yemek ver\n");
        printf("0. Çıkış\n");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                evcilHayvanEkle(hayvanlar, &hayvanSayisi);
                break;
            case 2:
                evcilHayvanGuncelle(hayvanlar, &hayvanSayisi);
                break;
            case 3:
                evcilHayvanSil(hayvanlar, &hayvanSayisi);
                break;
            case 4:
                evcilHayvanaSuVer(hayvanlar, hayvanSayisi);
                break;
            case 5:
                evcilHayvanaOyunOynat(hayvanlar, hayvanSayisi);
                break;
            case 6:
                evcilHayvaniTemizle(hayvanlar, hayvanSayisi);
                break;
            case 7:
                evcilHayvanaSevgiGoster(hayvanlar, hayvanSayisi);
                break;
            case 8:
                evcilHayvaniUyut(hayvanlar, hayvanSayisi);
                break;
            case 9:
                evcilHayvanaOdulVer(hayvanlar, hayvanSayisi);
                break;
            case 10:
            	evcilHayvanaBesle(hayvanlar ,hayvanSayisi);
            	break;
            case 0:
                printf("Çıkış yapılıyor...\n");
                break;
            default:
                printf("Geçersiz seçim!\n");
                break;
        }

        // Evcil hayvanları dosyaya yaz
        evcilHayvanlariYaz(hayvanlar, hayvanSayisi);

    } while (secim != 0);

    return 0;
}