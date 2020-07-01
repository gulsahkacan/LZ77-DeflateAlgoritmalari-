#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define engelBoyut 0x80000
#define pencereBoyutu 60
#define yedekBoyut 40
#define diziBoyutu yedekBoyut + pencereBoyutu

typedef enum { false, true } bool;

typedef struct hufmanDugum {
	int agirlik;
	short lch, rch;
} hufman_a;

typedef struct hufmanDenetleme {
	int engellen;
	int kodlen;
	int agacboyutu;
} hufman_ust_a;

typedef struct hufmanKod {
	unsigned char kod[24];
	int kodlen;
} hufmanKod_a;

void mCompress(const char *sourcefilename,const char *targetfilename)
{
    FILE *fcompress,*k;
    char ch,temp,c,cha;
    int counter;

    k=fopen(sourcefilename,"w");
    fprintf(k,"Dal sarkar kartal kalkar; kartal kalkar dal sarkar.");
    fclose(k);

    k=fopen(sourcefilename,"r");
    fcompress=fopen(targetfilename,"w");

    if(k == NULL)
    {
        printf("error");
    }
    if(fcompress == NULL)
    {
        printf("error");
    }

    while((ch=fgetc(k)) != EOF)
    {
        temp = ch;

        if(temp == (cha = fgetc(k)))
        {
            fprintf(fcompress,"*");
            counter = 2;
            while( ((c = fgetc(k)) != EOF) && temp == c)
                counter++;

            fprintf(fcompress,"%d",counter);
            fputc(cha,fcompress);
            temp = c;
            fputc(temp,fcompress);
            continue;
        }
        fputc(temp,fcompress);
        fputc(cha,fcompress);
    }
    fclose(fcompress);
    fclose(k);
}

/*

void mCompress(const char *sourcefilename,const char *targetfilename)
{
    FILE *fcompress,*k;
    char ch,temp,c,cha;
    int counter;

    k=fopen(sourcefilename,"w");
    fprintf(k,"Dal sarkar kartal kalkar; kartal kalkar dal sarkar.");
    fclose(k);

    k=fopen(sourcefilename,"r");
    fcompress=fopen(targetfilename,"w");

    if(k == NULL)
    {
        printf("error");
    }
    if(fcompress == NULL)
    {
        printf("error");
    }

    while((ch=fgetc(k)) != EOF)
    {
        temp = ch;

        if(temp == (cha = fgetc(k)))
        {
            fprintf(fcompress,"*");
            counter = 2;
            while( ((c = fgetc(k)) != EOF) && temp == c)
                counter++;

            fprintf(fcompress,"%d",counter);
            fputc(cha,fcompress);
            temp = c;
            fputc(temp,fcompress);
            continue;
        }
        fputc(temp,fcompress);
        fputc(cha,fcompress);
    }
    fclose(fcompress);
    fclose(k);
}


char *decode(struct token *tokens, int numTokens, int *pcbDecoded)
{
    int cap = 1 << 3;

    *pcbDecoded = 0;

    char *decoded = malloc(cap);

    int i;
    for (i = 0; i < numTokens; i++)
    {
        int offset = GETOFFSET(tokens[i].offset_len);
        int len = GETLENGTH(tokens[i].offset_len);
        char c = tokens[i].c;


        if (*pcbDecoded + len + 1 > cap)
        {
            cap = cap << 1;
            decoded = realloc(decoded, cap);
        }

        if (len > 0)
        {
            lz77memcpy(&decoded[*pcbDecoded], &decoded[*pcbDecoded - offset], len);
        }

        *pcbDecoded += len;

        decoded[*pcbDecoded] = c;

        *pcbDecoded = *pcbDecoded + 1;
    }

    return decoded;
}
*/

/*
struct token {
    uint8_t offset_len;
    char c;
};

inline int prefix_match_length(char *s1, char *s2, int limit)
{
    int len = 0;

    while (*s1++ == *s2++ && len < limit)
        len++;

    return len;
}

inline void lz77memcpy(char *s1, char *s2, int size)
{
    while (size--)
        *s1++ = *s2++;
}

char *decode(struct token *tokens, int numTokens, int *pcbDecoded)
{

    int cap = 1 << 3;

    *pcbDecoded = 0;

    char *decoded = malloc(cap);

    int i;
    for (i = 0; i < numTokens; i++)
    {

        int offset = GETOFFSET(tokens[i].offset_len);
        int len = GETLENGTH(tokens[i].offset_len);
        char c = tokens[i].c;

        if (*pcbDecoded + len + 1 > cap)
        {
            cap = cap << 1;
            decoded = realloc(decoded, cap);
        }


        if (len > 0)
        {
            lz77memcpy(&decoded[*pcbDecoded], &decoded[*pcbDecoded - offset], len);
        }


        *pcbDecoded += len;

        decoded[*pcbDecoded] = c;

        *pcbDecoded = *pcbDecoded + 1;
    }

    return decoded;
}

struct token *encode(char *text, int limit, int *numTokens)
{

    int cap = 1 << 3;


    int _numTokens = 0;


    struct token t;


    char *lookahead, *search;


    struct token *encoded = malloc(cap * sizeof(struct token));


    for (lookahead = text; lookahead < text + limit; lookahead++)
    {

        search = lookahead - OFFSETMASK;


        if (search < text)
            search = text;


        int max_len = 0;


        char *max_match = lookahead;


        for (; search < lookahead; search++)
        {
            int len = prefix_match_length(search, lookahead, LENGTHMASK);

            if (len > max_len)
            {
                max_len = len;
                max_match = search;
            }
        }


        if (lookahead + max_len >= text + limit)
        {
            max_len = text + limit - lookahead - 1;
        }



        t.offset_len = OFFSETLENGTH(lookahead - max_match, max_len);
        lookahead += max_len;
        t.c = *lookahead;


        if (_numTokens + 1 > cap)
        {
            cap = cap << 1;
            encoded = realloc(encoded, cap * sizeof(struct token));
        }


        encoded[_numTokens++] = t;
    }

    if (numTokens)
        *numTokens = _numTokens;

    return encoded;
}


char *file_read(FILE *f, int *size)
{
    char *content;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    content = malloc(*size);
    fseek(f, 0, SEEK_SET);
    fread(content, 1, *size, f);
    return content;
}

int main(void)
{
    FILE *f;
    int metin_boyutu = 8, token_sayisi, decode_boyutu;
    char *kaynak_metin = "aaaaaaaa", *decoded_metin = "";
    struct token *encoded_metin;

    if (f = fopen("source.txt", "rb"))
    {
        kaynak_metin = file_read(f, &metin_boyutu);
        fclose(f);
    }



    encoded_metin = encode(kaynak_metin, metin_boyutu, &token_sayisi);

    if (f = fopen("encoded.txt", "wb"))
    {
        fwrite(encoded_metin,sizeof(struct token),token_sayisi,f);
        fclose(f);
    }

    decoded_metin = decode(encoded_metin, token_sayisi, &decode_boyutu);

    if (f = fopen("decoded.txt", "wb"))
    {
        fwrite(decoded_metin,1, decode_boyutu, f);
        fclose(f);
    }

    printf("Orjinal Boyut: %d, Encode Boyutu: %d, Decode Boyutu: %d", metin_boyutu, token_sayisi * sizeof(struct token), decode_boyutu);

    return 0;
}
*/


/*** DEFLATE ALGORITMASI ***/
static int hufmanSonrakiDilimZaman(hufman_a *agac, int *boyut)
{
	int i;
	for(i = 0; i < 256; i++)
		if(agac[i].agirlik == -2)
			return i;
	return (*boyut)++;
}

static int hufmanAdim(hufman_a *agac, int *boyut)
{
	int min1 = -1, min2 = -1, t, i, n = 0, dilimZaman;
	for(i = 0; i < *boyut; i++) {
		if(agac[i].agirlik >= 0) {
			n++;
			if(min2 == -1 || agac[i].agirlik < agac[min2].agirlik)
				min2 = i;
			if(min1 == -1 || agac[min2].agirlik < agac[min1].agirlik)
				t = min1, min1 = min2, min2 = t;
		}
	}
	if(n < 2) return 0;
	dilimZaman = hufmanSonrakiDilimZaman(agac, boyut);
	agac[dilimZaman].agirlik = agac[min1].agirlik + agac[min2].agirlik;
	agac[dilimZaman].lch = min1;
	agac[dilimZaman].rch = min2;
	agac[min1].agirlik = -1;
	agac[min2].agirlik = -1;
	return n;
}

int hufmanBaslangicAgaci(unsigned char *engel, hufman_a *agac)
{
	int i;
	int n = 256;
	for(i = 0; i < 256; i++) {
		agac[i].agirlik = 0;
		agac[i].lch = agac[i].rch = -1;
	}
	for(i = 0; i < engelBoyut; i++)
		agac[engel[i]].agirlik++;
	for(i = 0; i < 256; i++)
		if(agac[i].agirlik == 0)
			agac[i].agirlik = -2;
	while(hufmanAdim(agac, &n));
	return n;
}

int hufmanKokBulma(hufman_a *agac)
{
	int i;
	for(i = 0; ; i++)
		if(agac[i].agirlik >= 0)
			return i;
}

void hufmanKodHesapla(hufman_a *agac, int dugum, hufmanKod_a *kods, int kodlen)
{
	static hufmanKod_a kod;
	if(dugum < 0) return;
	if(agac[dugum].lch == -1 && agac[dugum].rch == -1 && dugum < 256) {
		kod.kodlen = kodlen;
		memcpy(&kods[dugum], &kod, sizeof(kod));
		return;
	}
	kod.kod[kodlen] = 0;
	hufmanKodHesapla(agac, agac[dugum].lch, kods, kodlen + 1);
	kod.kod[kodlen] = 1;
	hufmanKodHesapla(agac, agac[dugum].rch, kods, kodlen + 1);
}

int hufmanDeflate(hufman_a *agac, const unsigned char *girdi, size_t uzunluk, unsigned char *cikti)
{
	unsigned char ch = 0;
	int i, j, k = 0, kok, ciktipozisyon = 0;
	static hufmanKod_a kod[256];
	kok = hufmanKokBulma(agac);
	hufmanKodHesapla(agac, kok, kod, 0);
	for(i = 0; i < uzunluk; i++) {
		for(j = 0; j < kod[girdi[i]].kodlen; j++) {
			ch = ch | ((kod[girdi[i]].kod[j] & 1) << k);
			if(++k == 8) {
				cikti[ciktipozisyon++] = ch;
				k = 0;
				ch = 0;
			}
		}
	}
	if(k > 0)
		cikti[ciktipozisyon++] = ch;
	return ciktipozisyon;
}
void deflate(FILE *girdidosyasi, FILE *cikisdosyasi)
{
	static unsigned char engel[engelBoyut];
	static unsigned char yedek[engelBoyut * 2];
	hufman_ust_a hdr;
	static hufman_a mainagac[512];
	size_t nokuma;
	while((nokuma = fread(engel, 1, engelBoyut, girdidosyasi)) > 0) {
		hdr.engellen = nokuma;
		hdr.agacboyutu = hufmanBaslangicAgaci(engel, mainagac);
		hdr.kodlen = hufmanDeflate(mainagac, engel, nokuma, yedek);
		fwrite(&hdr, sizeof(hdr), 1, cikisdosyasi);
		fwrite(mainagac, sizeof(hufman_a), hdr.agacboyutu, cikisdosyasi);
		fwrite(yedek, 1, hdr.kodlen, cikisdosyasi);
	}
}

void Deflate()
{
    FILE *giris1,*deflatee;
    giris1 = fopen("giris1.txt", "rb");
	deflatee = fopen("deflate.txt", "wb");

    printf("DEFLATE ALGORITMASI:\n\n");
    dosyaBoyutOkuma2();
    clock_t deflate_basla = clock();
	deflate(giris1,deflatee);
    clock_t deflate_bitir = clock();
    printf("Calisma zamani: ");
    printf("%f", ((double) (deflate_bitir - deflate_basla) / CLOCKS_PER_SEC));
    printf(" [seconds]\n");
    dosyaBoyutOkuma4();

    fclose(giris1);
	fclose(deflatee);
}

/*** GIRIS VE CIKIS DOSYALARININ BOYUTLARININ BELIRLENMESI ***/
/** 1024 byte = 1 Kilobyte (KB) eder **/
int dosyaBoyutOkuma1()
{
    FILE *dosya = fopen("giris.txt", "rb");
    fseek(dosya,0,SEEK_END);
    printf("LZ77 giris dosyasi %d byte boyutundadir.\n",ftell(dosya));
    fclose(dosya);
}
int dosyaBoyutOkuma2()
{
    FILE *dosya = fopen("giris1.txt", "rb");
    fseek(dosya,0,SEEK_END);
    printf("Deflate giris dosyasi %d byte boyutundadir.\n",ftell(dosya));
    fclose(dosya);
}
int dosyaBoyutOkuma3()
{
    FILE *dosya = fopen("lz77.txt", "rb");
    fseek(dosya,0,SEEK_END);
    printf("LZ77 cikis dosyasi %d byte boyutundadir.\n\n",ftell(dosya));
    fclose(dosya);
}
int dosyaBoyutOkuma4()
{
    FILE *dosya = fopen("deflate.txt", "rb");
    fseek(dosya,0,SEEK_END);
    printf("Deflate cikis dosyasi %d byte boyutundadir.\n",ftell(dosya));
    fclose(dosya);
}

/*** LZ77 ALGORITMASI ***/
void sikistirma(FILE *dosya_girdisi,FILE *dosya_ciktisi) {
    bool son = false;
    int girdiuzunluk = 0;
    int ciktiuzunluk = 0;
    int endOffset = 0;
    int pozisyon = -1;
    int i, boyut, degistir, c_in;
    size_t byte_okuma = (size_t) -1;
    unsigned char c;
    unsigned char dizi[diziBoyutu];
    unsigned char pencere[pencereBoyutu];
    unsigned char yedek[yedekBoyut];
    unsigned char loadyedek[yedekBoyut];
    unsigned char str[yedekBoyut];


    fseek(dosya_girdisi, 0, SEEK_END);
    girdiuzunluk = ftell(dosya_girdisi);
    fseek(dosya_girdisi, 0, SEEK_SET);


    if (girdiuzunluk == 0)
        return 3;


    if (girdiuzunluk < diziBoyutu)
        return 2;


    fread(dizi, 1, diziBoyutu, dosya_girdisi);


    fwrite(dizi, 1, pencereBoyutu, dosya_ciktisi);


    while (true) {
        if ((c_in = fgetc(dosya_girdisi)) == EOF)
            son = true;
        else
            c = (unsigned char) c_in;


        for (int k = 0; k < pencereBoyutu; k++)
            pencere[k] = dizi[k];


        for (int k = pencereBoyutu, j = 0; k < diziBoyutu; k++, j++) {
            yedek[j] = dizi[k];
            str[j] = dizi[k];
        }


        if (endOffset != 0) {
            boyut = yedekBoyut - endOffset;
            if (endOffset == yedekBoyut)
                break;
        }
        else {
            boyut = yedekBoyut;
        }

        pozisyon = -1;
        for (i = boyut; i > 0; i--) {
            pozisyon = eslesmeAra(pencere, str, i);
            if (pozisyon != -1)
                break;
        }


        if (pozisyon == -1) {
            fputc(255, dosya_ciktisi);
            fputc(yedek[0], dosya_ciktisi);
            degistir = 1;
        }

        else {
            fputc(pencereBoyutu - pozisyon, dosya_ciktisi);
            fputc(i, dosya_ciktisi);
            if (i == yedekBoyut) {
                degistir = yedekBoyut + 1;
                if (!son)
                    fputc(c, dosya_ciktisi);
                else
                    endOffset = 1;
            }
            else {
                if (i + endOffset != yedekBoyut)
                    fputc(yedek[i], dosya_ciktisi);
                else
                    break;
                degistir = i + 1;
            }
        }


        for (int j = 0; j < diziBoyutu - degistir; j++)
            dizi[j] = dizi[j + degistir];
        if (!son)
            dizi[diziBoyutu - degistir] = c;

        if (degistir == 1 && son)
            endOffset++;


        if (degistir != 1) {

            byte_okuma = fread(loadyedek, 1, (size_t) degistir - 1, dosya_girdisi);

            for (int k = 0, l = diziBoyutu - degistir + 1; k < degistir - 1; k++, l++)
                dizi[l] = loadyedek[k];

            if (son) {
                endOffset += degistir;
                continue;
            }

            if (byte_okuma < degistir - 1)
                endOffset = degistir - 1 - byte_okuma;
        }
    }


    fseek(dosya_ciktisi, 0, SEEK_END);
    ciktiuzunluk = ftell(dosya_ciktisi);
    fseek(dosya_ciktisi, 0, SEEK_SET);

    fclose(dosya_girdisi);
    fclose(dosya_ciktisi);

    return 1;
}
int eslesmeAra(unsigned char pencere[], unsigned char str[], int strLen) {
    int j, k, pozisyon = -1;

    for (int i = 0; i <= pencereBoyutu - strLen; i++) {
        pozisyon = k = i;

        for (j = 0; j < strLen; j++) {
            if (str[j] == pencere[k])
                k++;
            else
                break;
        }
        if (j == strLen)
            return pozisyon;
    }

    return -1;
}

void LZ77()
{
    FILE *giris,*LZ77e;
	giris = fopen("giris.txt", "rb");
    LZ77e = fopen("LZ77.txt", "wb");

    printf("LZ77 ALGORITMASI:\n\n");
    dosyaBoyutOkuma1();
    clock_t basla = clock();
    sikistirma(giris,LZ77e);
    clock_t bitir = clock();
    printf("Calisma zamani: ");
    printf("%f", ((double) (bitir - basla) / CLOCKS_PER_SEC));
    printf(" [seconds]\n");
    dosyaBoyutOkuma3();

	fclose(giris);
  	fclose(LZ77e);
}



int main()
{
    FILE *dosyaacici;
    dosyaacici = fopen("sikistirmamsi.txt","w+");
    fclose(dosyaacici);
    printf("**************************************************************\n");
    printf("LZ77 Algoritmasi ve Deflate Algoritmasi Karsilastirmasi:\n");
    printf("**************************************************************\n\n");
    LZ77();
    Deflate();
    return 0;
}

