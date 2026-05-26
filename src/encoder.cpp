/**
 * @file encoder.cpp
 * @brief Comportement général des deux encodeurs.
 * 
 * Les appels sont antérieurs aux autres appels.
 *
 */

#include "encoder.h"
#include "page.h"
#include "volca.h"

#include "Display.h"
extern Display oled;

/**
 * @brief Rotation de l'encodeur PARAMETER.
 *
 * Déplacement du curseur dans tous les cas
 * et affichage du paramètre précédent dans le cas d'un
 * algorithme à la place d'une éventuelle valeur. 
 *
 * Chaque rotation indique un nouveau choix.
 *
 */

void handleRotate0(int8_t rotation) {
    oled.displaySelected(rotation);
}

/**
 * @brief Pression de l'encodeur PARAMETER.
 *
 * Alterne entre le menu principal et une page de paramètres FM.
 */


void handlePress0() {
    oled.putChar(get_cur_pos(), ' ');
    if (change_page()) {
        strcpy(oled.buffer, get_page().str);
        oled.charIndex = 0;
        oled.endPosition = PAGE_LEN;
    } else {
        oled.putChar(get_cur_pos(), CURSOR);
    }

}

/**
 * @brief Pression longue de l'encodeur PARAMETER.
 *
 * Reboot.
 */


void handleLongPress0() {
    asm volatile("jmp 0x00");
}

/**
 * @brief Rotation de l'encodeur VALUE.
 *
 * La première utilisation ne fait qu'afficher la valeur.
 * Les suivantes modifient la valeur. 
 */

void handleRotate1(int8_t rotation) {
    if (6 < get_page_index()) {
        change_algo(rotation);
        strcpy(oled.buffer, get_page().str);
        oled.charIndex = 0;
        oled.endPosition = PAGE_LEN;
        return;
    }
    byte index = get_par_index();
    if (index == NAI || index == ALGORITHM) {
        return;
    }
    byte value = get_value(index);
    if(!oled.firstDisplay) {
        int step = 1;
        if (get_max(index) == 99) {
            step = 5;
        }
        if (rotation > 0) {
            if(value + step < get_max(index)) {
                set_value(index, value + step);
            } else {
                set_value(index, get_max(index));
            }
        } else if (rotation < 0) {
            if(step < value) {
                set_value(index, value - step);
            } else {
                set_value(index, 0);
            }
        }
    }
    oled.displayValue();
}

/**
 * @brief une pression affiche le nom du paramètre
 */

void handlePress1() {
    oled.displayName();
}
