/**
 * @file midi.cpp
 * @brief Gestion des messages MIDI.
 */

#include <Arduino.h> // micros()
#include <MIDI.h>
#include "midi.h" /**< pour LED_CLOCK */
#include "volca.h" /**< correspondance volcas FM<->FB-01 */
#include "page.h"


using namespace MIDI_NAMESPACE;

unsigned long currentTick = 0; /**< le temps est discrétisé en PPQN*/
bool clock = false; /**< la led n'affiche pas le tempo*/
extern MidiInterface<SerialMIDI<HardwareSerial>> MIDI; /**<interface MIDI*/

/**
 * @brief Gestion note on de la librairie
 * 
 * La led indique prioritairement le tempo.
 */

void handleNoteOn(byte channel, byte pitch, byte velocity) {
    MIDI.sendNoteOn(pitch, velocity, channel);
    if (!clock) {
        digitalWrite(LED_CLOCK, HIGH);
    }
}

/**
 * @brief Gestion note off de la librairie
 * 
 * La led indique prioritairement le tempo.
 */


void handleNoteOff(byte channel, byte pitch, byte velocity) {
    MIDI.sendNoteOff(pitch, velocity, channel);
    if (!clock) {
        digitalWrite(LED_CLOCK, LOW);
    }
}

/**
 * @brief Gestion CC de la librairie.
 * 
 * Chaque numéro de CC correspond à une page et un index (dans page.cpp)
 * affectés avec set_indexes. On en déduit le numéro de paramètre FM. On
 * modifie dans le tableau des paramètres.
 *
 */

void handleControlChange(byte channel, byte number, byte value) {
    number = cc2index(number);
    value = (byte) map(value, 0, 127, 0, (int) get_max(number));
    if (number == ALGORITHM) {
        value = value >> 2;
    }
    set_value(number, value);
}

/**
 * @brief La led affiche la clock.
 */

void handleClock() {
    switch (currentTick % 24) {
    case 0:
        digitalWrite(LED_CLOCK, HIGH);
        break;
    case 1:
        digitalWrite(LED_CLOCK, LOW);
        break;
    default:
        break;
    }
    currentTick++;    
    MIDI.sendRealTime(midi::Clock);
}

/**
 * @brief Gestion start de la librairie.
 *
 * La commande start initialise la clock. 
 */

void handleStart() {
    currentTick = 0;
    clock = true;
    MIDI.sendRealTime(midi::Start);
}

/**
 * @brief Gestion stop de la librairie.
 *
 * La commande arrête la clock et son affichage. 
 */

void handleStop() {
    MIDI.sendRealTime(midi::Stop);
    digitalWrite(LED_CLOCK, LOW);
    clock = false;
}
