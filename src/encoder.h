/**
 * @file encoder.h
 * @brief Gère les évènements sur les pots.
 */

#ifndef ENCODER_H
#define ENCODER_H
#define NUM_ENC 2
#include <Arduino.h>

void handleRotate0(int8_t rotation);
void handlePress0();
void handleLongPress0();
void handleRotate1(int8_t rotation);
void handlePress1();
#endif
