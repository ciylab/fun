/**
 * @file page.cpp
 * @brief Gestion des pages de l'écran.
 *
 * La page principale affiche les groupes de paramètres. Les pages secondaires
 * affichent une liste de paramètres.
 *
 * Contient les fonctions getter et setter évitant l'usage de extern.
 */

#include <Arduino.h>
#include "page.h"
#include "volca.h"


int page_index; /**< le rang de la page dans la liste */ 
int cursor_index; /**< le rang du paramètre dans la page */
int cursor_position; /**< la position du > parmi les 64 caractères */
int current_algo; /**< la numéro de 0 à 7 de l'algotithme FB-01 */

/**
 * @brief Le texte des pages.
 */
char menu_str[][65] = {
    //  modèle :
    //  "x111111 x1111111x111111 x1111111x111111 x1111111x111111 x1111111",
    // FB-01 (AfterTouch mag.)
    // CC :
    // 1      2
    " ALGO    FDBCK   ADSR    MINLVL  OUT     LFO     PITCH   SCALING",
    // ADSR
    // 3  4   5   6   7   8   9   10  11  12  13  14  15  16  17  18
    " A1  D1  S1  R1  A2  D2  S2  R2  A3  D3  S3  R3  A4  D4  S4  R4 ",
    // MIN LEVEL
    // 19             20              21              22
    " ML1             ML2             ML3             ML4            ",
    // OUTPUT LEVEL et KEYBOARD SENS.
    // 23     24      25      26      27      28      29      30
    " LVL1    KVS1    LVL2    KVS2    LVL3    KVS3    LVL4    KVS4   ",
    // LFO
    // 31     32      33      34      36
    " SPEED   DELAY   DEPTH   WAVE    MSP                            ",
    // OSCM = 00 (ratio)
    // 36             37              38              39
    " FREQ1           FREQ2           FREQ3           FREQ4          ",
    // LEVEL SCALE
    // 40     41      42      43      44      45
    " LDE     RDE     LCU     RCU     BRK     KRS                    ",
};

/**
 * @brief Pour chage les numéros de paramètres DX7.
 */
byte par_num[NUM_PAGE][16] = {
    {ALGORITHM, 135, NAI, NAI, NAI, NAI, NAI, NAI,       
        NAI, NAI, NAI, NAI, NAI, NAI, NAI, NAI},
    {63, 64, 68, 66, 42, 43, 47, 45, 21, 22, 26, 24, 0, 1, 5, 3},
    {70, 49, 28, 7},
    {79, 78, 58, 57, 37, 36, 16, 15,   
        NAI, NAI, NAI, NAI, NAI, NAI, NAI, NAI},
    {137, 138, 139, 142, 143, NAI, NAI, NAI, 
        NAI, NAI, NAI, NAI, NAI, NAI, NAI, NAI},
    {81, 60, 39, 18, NAI, NAI, NAI, NAI,   
        NAI, NAI, NAI, NAI, NAI, NAI, NAI, NAI},
    {72, 73, 74, 75, 71, 76, NAI, NAI,  
        NAI, NAI, NAI, NAI, NAI, NAI, NAI, NAI},
};

/**
 * @brief Les 8 algorithmes du FB-01 en ascii.
 */
char algo_str[][65] = {
    // algorithme 1
    " 1      4]            2-3             1               *         ",
    // algorithme 2
    " 2      4]            3-2               1               *       ",
    // algorithme 3
    " 3     4               3               1-2]            *        ",
    // algorithme 4
    " 4     4]              3               1-2             *        ",
    // algorithme 5
    " 5                    2 4]            1 3             * *       ",
    // algorithme 6
    " 6                     4]            1 2 3           * * *      ",
    // algorithme 7
    " 7                       4]          1 2 3           * * *      ",
    // algorithme 8
    " 8                                  1 2 3 4]        * * * *     ",
};

/**
 * @brief nombre de paramètres par page
 */
byte l[7] = {2, 16, 4, 8, 5, 4, 6};

/**
 * Pour chaque page :
 * - son texte
 * - les positions du curseur > dans la page
 * - le nombre de paramètres
 */
Page pages[NUM_PAGE] = {
    {menu_str[0], {0, 8, 16, 24, 32, 40, 48, 56}, 8},
    {menu_str[1], {0, 4, 8, 12, 16, 20, 24, 28, 
                  32, 36, 40, 44, 48, 52, 56, 60}, l[1]},
    {menu_str[2], {0, 16, 32, 48}, l[2]},
    {menu_str[3], {0, 8, 16, 24, 32, 40, 48, 56}, l[3]},
    {menu_str[4], {0, 8, 16, 24, 32, 40, 48, 56}, l[4]},
    {menu_str[5], {0, 16, 32, 48}, l[5]},
    {menu_str[6], {0, 8, 16, 24, 32, 40}, l[6]},
    {algo_str[0], {}, 0},
    {algo_str[1], {}, 0},
    {algo_str[2], {}, 0},
    {algo_str[3], {}, 0},
    {algo_str[4], {}, 0},
    {algo_str[5], {}, 0},
    {algo_str[6], {}, 0},
    {algo_str[7], {}, 0},
};

/**
 * @brief Fonction de gestion du changement page par pression sur le pot.
 *
 * @return le booléen valant le chanement de page
 */
int change_page(void) {
    int p[] = {0, 0, 1, 2, 3, 4, 5, 6};
    if (page_index == 0) {
        if (cursor_index == 0) { // algo
            page_index = current_algo + 7;
        } else if (cursor_index == 1) { // feedback
            return 0;
        } else {
            page_index = p[cursor_index];
        }
    } else {
        page_index = 0;
    }
    cursor_index = 0;
    return 1;
}

/**
 * @brief Fonction qui donne le numéro de paramètre FM en focntion
 * du CC. 
 *
 * Les numéros de cc sont 1, 2, 3,..., 59 i.e. jusqu'au dernier 
 * paramètre géré via les pots et l'écran.
 *
 * @return le rang du paramètre
 */

byte cc2index(byte cc) {
    int i; // numéro de page
    byte j = cc - 1; // rang du paramètre dans la page
    for(i = 0; i < 7; i++) {
        if (j < l[i]) {
            break;
        }
        j = j - l[i];
    }
    return par_num[i][j];
}

/**
 * @return le numéro de page courante
 */
byte get_page_index(void) {
    return page_index;
}

/**
 * @brief affectation du numéro de page 
 */
void set_page_index(byte index) {
    page_index = index;
}

/**
 * @brief affectation du numéro de curseur 
 */
void set_cursor_index(byte index) {
    cursor_index = index;
}

/**
 * @return le numéro de paramètre FM
 */
byte get_par_index(void) {
    return par_num[page_index][cursor_index];
}

/**
 * @return la page courante
 */
page get_page(void) {
    return pages[page_index];
}

/**
 * @return la position du curseur > parmi les 64 caractères.
 */
int get_cur_pos(void) {
    return pages[page_index].positions[cursor_index];
}

/**
 * @brief utile pour gérer la substitution d'affichage nom<->valeur
 * @return la position suivante du curseur > parmi les 64 caractères.
 */
int get_next_cur_pos(void) {
    if(cursor_index == pages[page_index].num_param - 1) {
        return PAGE_LEN; 
    }
    return pages[page_index].positions[cursor_index + 1];
}

/**
 * @brief change la valeur du numéro de paramètre
 */
void move_cursor(int8_t rotation) {
    if (6 < page_index) {
        return;
    }
    Page p = pages[page_index];
    if (rotation > 0) {
        cursor_index = (cursor_index + 1) % p.num_param;
    } else if (rotation < 0) {
        cursor_index = (p.num_param + cursor_index - 1) % p.num_param;
    } else if (rotation == 0) {
        cursor_index = 0;
    }
}

/**
 * @brief récupère la valeur du paramètre FM en ascii.
 */
void get_string(char str[8]) {
    byte index = get_par_index();
    sprintf(str, "%2d", get_value(index));
}
