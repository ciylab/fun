#ifndef VOLCA_H
#define VOLCA_H

#include <Arduino.h>

#define ALGORITHM 134 

/**
 * fonction assignant page_index et cursor_index en fonction du cc
 */
byte get_value(byte index);
byte get_max(byte index);
void set_value(byte index, byte value);
void change_algo(int8_t rotation);
#endif
