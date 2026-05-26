/**
 * @file Display.h
 * @brief Champs de la classe d'affichage.
 */
 
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h> // pour byte
#include "page.h"

#define CURSOR '>' /**< curseur de l'écran*/

class Display {
  public:
    char buffer[PAGE_LEN + 1]; /**<tampon de l'écran : ce qui doit être affiché */ 
    char screen[PAGE_LEN + 1]; /**<contenu de l'écran : ce qui est affiché */
    byte charIndex; /**<position dans le tampon du caractère à afficher */
    byte endPosition; /**<dernière position à afficher */
    bool firstDisplay; /**<première rotation ? */
    bool viewValue; /**<valeur affichée */
    Display();
    void begin();
    void putChar(byte position, char c);
    void welcome();
    void display();
    void displaySelected(int8_t rotation);
    void displayName();
    void displayValue();
};

#endif
