/**
 * @file Display.cpp
 * @brief Gestion de l'affichage.
 *
 * La variable screen représente ce qui est affiché.
 * La variable buffer est ce qui doit être affiché.
 * Pour des questions de latence, screen est mis à jour à partir
 * de buffer tant que les contenus sont différent, et un caractère
 * par boucle de loop() à la fois. 
 */


#include "Display.h"
#include <U8x8lib.h>
#include "font/my_u8x8_font_7x14_1x2_r.h"
#include "page.h"

/**
 * Oled SSD1306 ou SH1106.
 */

#ifdef SSD1306
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#elif SH1106
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#endif

/**
 * @brief Initialisation avec 64 espaces.
 */
Display::Display() {
    charIndex = 0;
    endPosition = PAGE_LEN;
    set_page_index(0);
    set_cursor_index(0);
    strcpy(buffer, get_page().str);
    for (int i = 0; i < PAGE_LEN; i++) {
        screen[i] = ' ';
    }
    screen[PAGE_LEN] = '\0';
    buffer[PAGE_LEN] = '\0';
    u8x8.setFont(my_u8x8_font_7x14_1x2_r);
    firstDisplay = true;
}

/**
 * @brief Initialisation du setup.
 */
void Display::begin() {
    u8x8.begin();
}
    
/**
 * @brief Affiche un caractère **c** à une position **position**.
 */
void Display::putChar(byte position, char c) {
    u8x8.drawGlyph(position % 16, 2 * (position / 16), c);
}

/**
 * @brief Affiche un message de bienvenue.
 *
 * Le numéro de version indique le niveau de mise à jour du contenu
 * du micro-processeur.
 *
 */

void Display::welcome() {
    sprintf(screen, "  SEND SYSEX TO   VOLCA FM 4-OP   BY CIYLAB       %s",
            VERSION);
    for (int i = 0; i < PAGE_LEN; i++) {
        putChar(i, screen[i]);
    }
}

/**
 * @brief Fonction de rafraîchissement.
 *
 * La variable **charIndex** est le numéro de caractère courant à afficher.
 * La variable **endPosition** est le numéro du dernier caractère du
 * buffer à afficher. Les valeurs de **charIndex** et **endPosition** 
 * dépendent de ce qui doit être affiché. Pour une page entière c'est 0 
 * et 63 par exemple. La fonction affiche au plus un nouveau caractère 
 * à chaque boucle de l'Arduino pour minimiser la latence des messages MIDI.
 */

void Display::display() {
    // without buffer : 100ms
    // with buffer : 2ms
    while (charIndex < endPosition && 
        screen[charIndex] == buffer[charIndex]) {
        if (charIndex == 0) {
            putChar(charIndex, CURSOR);
        }
        charIndex++;
    }
    if (charIndex == endPosition) {
        return;
    }
    putChar(charIndex, buffer[charIndex]);
    screen[charIndex] = buffer[charIndex];
    charIndex++;
}

/**
 * @brief Déplace le curseur.
 *
 * On met un espace là où on est puis un chevron là où on va.
 */
void Display::displaySelected(int8_t rotation) {
    if (6 < get_page_index()) {
        return;
    }
    putChar(get_cur_pos(), ' ');
    move_cursor(rotation);
    putChar(get_cur_pos(), CURSOR);
    firstDisplay = true;
}

/**
 * @brief Met le nom du paramètre dans le buffer.
 *
 * Initialise **charIndex** et **endPosition** pour afficher le nom
 * du paramètre au lieu de sa valeur.
 */
void Display::displayName() {
    firstDisplay = true;
    viewValue = false;
    charIndex = get_cur_pos();
    int i;
    for(i = charIndex; i < get_next_cur_pos(); i++) {
        buffer[i] = get_page().str[i];
    }
    endPosition = i;
}

/**
 * @brief Met la value du paramètre dans le buffer.
 *
 * Initialise **charIndex** et **endPosition** pour afficher la valeur
 * du paramètre au lieu de son nom.
 */

void Display::displayValue() {
    char temp[8];
    int i = 0;
    firstDisplay = false; 
    viewValue = true;
    get_string(temp);
    charIndex = get_cur_pos();
    while (i < 2) {
        buffer[charIndex + 1 + i] = temp[i];
        i++;
    }
    while(charIndex + 1 + i < get_next_cur_pos()) {
        buffer[charIndex + 1 + i] = ' ';
        i++;
    }
    endPosition = charIndex + i;
}
