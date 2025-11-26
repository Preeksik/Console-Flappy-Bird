#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define FILAR_WIDTH 2 

struct filar {
    int x;
    int hole_y;
    int hole_size;
};

void ukryj_kursor() {
    HANDLE uchwyt = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; 
    SetConsoleCursorInfo(uchwyt, &info);
}

void ustaw_kursor_na_poczatek() {
    HANDLE uchwyt = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pozycja = {0, 0};
    SetConsoleCursorPosition(uchwyt, pozycja);
}

void generowanie_mapy(char **mapa, int wysokosc, int szerokosc) {
    for (int i = 0; i < wysokosc; i++) {
        for (int j = 0; j < szerokosc; j++) {
            if (i == 0 || i == wysokosc - 1) mapa[i][j] = '=';
            else if (j == 0 || j == szerokosc - 1) mapa[i][j] = '|';
            else mapa[i][j] = ' ';
        }
    }
}

void rysowanie_mapy(char **mapa, int wysokosc, int szerokosc) {
    for (int i = 0; i < wysokosc; i++) {
        for (int j = 0; j < szerokosc; j++) {
            putchar(mapa[i][j]);
        }
        putchar('\n');
    }
}

void rysuj_filar(char **mapa, int wysokosc, int szerokosc, struct filar f) {
    for (int i = 1; i < wysokosc - 1; i++) {
        if (i >= f.hole_y && i < f.hole_y + f.hole_size) {
        } else {
            for (int w = 0; w < FILAR_WIDTH; w++) {
                int col = f.x + w;
                if (col > 0 && col < szerokosc - 1) {
                    mapa[i][col] = (char)219; // 219 to kod ASCII na pelny blok
                }
            }
        }
    }
}

int main() {
    ukryj_kursor();
    SetConsoleTitle("Flappy Bird");
    srand((unsigned)time(NULL));

    int dziura;
    int ptk_za_klatke;
    char wybor;
    char gracz[50];

    int szerokosc;
    int wysokosc;
    int odstep;
    int ilosc_filarow;
    int predkosc_gry;

    printf("======= FLAPPY BIRD =======\n");
    printf("Podaj swoje imie: ");
    scanf(" %[^\n]", gracz);

    printf("\nWYBIERZ POZIOM TRUDNOSCI\n");
    printf("1. Easy (wpisz 'E')   (mala mapa, wolna gra, duze dziury, 3 pkt/klatka)\n");
    printf("2. Normal (wpisz 'N') (srednia mapa, szybka gra, srednie dziury, 5 pkt/klatka)\n");
    printf("3. Hard (wpisz 'H')   (duza mapa,bardzo szybka gra, male dziury, 12 pkt/klatka)\n");
    printf("Twoj wybor: ");
    scanf(" %c", &wybor);

    switch (wybor) {
        case 'E': case 'e':
            dziura = 7;
            ptk_za_klatke = 3;
            szerokosc = 45;
            wysokosc = 20;
            odstep = 15;
            ilosc_filarow = 3;
            predkosc_gry = 100;
            break;
        case 'N': case 'n':
            dziura = 5;
            ptk_za_klatke = 5;
            szerokosc = 60;
            wysokosc = 20;
            odstep = 12;
            ilosc_filarow = 4;
            predkosc_gry = 80;
            break;
        case 'H': case 'h':
            dziura = 4;
            ptk_za_klatke = 12;
            szerokosc = 50;
            wysokosc = 20;
            odstep = 10;
            ilosc_filarow = 5;
            predkosc_gry = 60;
            break;
        default:
            dziura = 5;
            ptk_za_klatke = 3;
            szerokosc = 60;
            wysokosc = 20;
            odstep = 15;
            ilosc_filarow = 4;
            predkosc_gry = 80;
            printf("Niepoprawny wybor - ustawiono poziom NORMAL.\n");
            Sleep(1000);
            break;
    }

    char **mapa = malloc(wysokosc * sizeof(char*));
    if (!mapa) { printf("Brak pamieci\n"); return 1; }
    for (int i = 0; i < wysokosc; i++) {
        mapa[i] = malloc(szerokosc * sizeof(char));
        if (!mapa[i]) return 1;
    }

    struct filar *filary = malloc(ilosc_filarow * sizeof(struct filar));
    if (!filary) return 1;

    int start_x = szerokosc - 1;
    for (int i = 0; i < ilosc_filarow; i++) {
        filary[i].hole_size = dziura;
        filary[i].x = start_x + i * odstep;
        filary[i].hole_y = 1 + rand() % (wysokosc - filary[i].hole_size - 2);
    }

    int punkty = 0;
    int x_pos = 5; 
    if (x_pos < FILAR_WIDTH + 1) x_pos = FILAR_WIDTH + 1;
    float y_pos = (float)(wysokosc / 2);
    float predkosc_skoku = 0.0f;
    float grawitacja = 0.5f;
    int flap = -2;
    char c;

    system("cls");
    printf("\n=== FLAPPY BIRD ===\n");
    printf("Sterowanie: Spacja = skok, Q = wyjscie\n");
    printf("Nacisnij SPACJE, aby zaczac...\n");

    while (1) {
        if (_kbhit()) {
            char zaczynamy = _getch();
            if (zaczynamy == ' ') break;
            else if (zaczynamy == 'q' || zaczynamy == 'Q') {
                for (int i = 0; i < wysokosc; i++) free(mapa[i]);
                free(mapa);
                free(filary);
                return 0;
            }
        }
    }
    
    system("cls");
    
    while (1) {
        ukryj_kursor();
        ustaw_kursor_na_poczatek();
        generowanie_mapy(mapa, wysokosc, szerokosc);

        for (int i = 0; i < ilosc_filarow; i++) {
            filary[i].x -= 1;
        }

        for (int i = 0; i < ilosc_filarow; i++) {
            if (filary[i].x + FILAR_WIDTH - 1 < 0) {
                int max_x = filary[0].x;
                for (int k = 1; k < ilosc_filarow; k++) if (filary[k].x > max_x) max_x = filary[k].x;

                filary[i].x = max_x + odstep;
                filary[i].hole_y = 1 + rand() % (wysokosc - filary[i].hole_size - 2);
            }
        }

        for (int i = 0; i < ilosc_filarow; i++) {
            rysuj_filar(mapa, wysokosc, szerokosc, filary[i]);
        }

        predkosc_skoku += grawitacja;
        y_pos += predkosc_skoku;
        int int_y = (int)y_pos;

        if (int_y <= 0 || int_y >= wysokosc - 1) {
            break;
        }

        int c0 = x_pos;
        int c1 = x_pos - 1;
        int c2 = x_pos - 2;
        if (c2 < 1) c2 = 1; 
        
        if (mapa[int_y][c0] != ' ' || mapa[int_y][c1] != ' ' || mapa[int_y][c2] != ' ') {
            break;
        }

        mapa[int_y][x_pos] = '>';
        mapa[int_y][x_pos - 1] = ')';
        mapa[int_y][x_pos - 2] = '(';

        rysowanie_mapy(mapa, wysokosc, szerokosc);
        printf("Punkty: %d   ", punkty);

        if (_kbhit()) {
            c = _getch();
            if (c == ' ') predkosc_skoku = flap;
            else if (c == 'q' || c == 'Q') break;
        }

        punkty += ptk_za_klatke;
        Sleep(predkosc_gry); 
    }

    system("cls");
    printf("============================\n");
    printf("KOLIZJA! KONIEC GRY\n");
    printf("Twoj wynik: %d\n", punkty);
    printf("============================\n");

    FILE *pFile = fopen("wyniki.txt", "a");
    if (pFile != NULL) {
        fprintf(pFile, "%s;%d\n", gracz, punkty);
        fclose(pFile);
    } else {
        printf("Blad: nie mozna otworzyc pliku do dopisania.\n");
    }

    FILE *readFile = fopen("wyniki.txt", "r");
    if (readFile == NULL) {
        printf("Blad: nie mozna otworzyc pliku do odczytu.\n");
    } else {
        typedef struct {
            char imie[50];
            int wynikk;
        } Wynik;

        Wynik tab[200];
        int ilosc = 0;
        
        while (ilosc < 200 && fscanf(readFile, " %49[^;];%d", tab[ilosc].imie, &tab[ilosc].wynikk) == 2) {
            ilosc++;
        }
        fclose(readFile);

        if (ilosc == 0) {
            printf("\nBrak wynikow w pliku.\n");
        } else {
            for (int i = 0; i < ilosc - 1; i++){
                for (int j = 0; j < ilosc - i - 1; j++) {
                    if (tab[j].wynikk < tab[j + 1].wynikk) {
                        Wynik tmp = tab[j];
                        tab[j] = tab[j + 1];
                        tab[j + 1] = tmp;
                    }
                }
            }
            int najlepsze_wyniki = (ilosc < 3) ? ilosc : 3;
            printf("\n======= NAJLEPSZE WYNIKI =======\n");
            for (int i = 0; i < najlepsze_wyniki; i++){
                printf("%d. %s - %d pkt\n", i + 1, tab[i].imie, tab[i].wynikk);
            }
        }
    }

    for (int i = 0; i < wysokosc; i++) free(mapa[i]);
    free(mapa);
    free(filary);

    return 0;
}