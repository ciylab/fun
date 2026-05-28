/**
 * @file volca.cpp
 * @brief Gestion spécifique du volca.
 *
 */

#include <Arduino.h>
#include <MIDI.h>
#include "volca.h"
#include "page.h"

#define M 99

using namespace MIDI_NAMESPACE;

extern uint8_t parameters_value[];
extern int current_algo; 
extern MidiInterface<SerialMIDI<HardwareSerial>> MIDI; /**<interface MIDI*/

/**
 * @brief Le numéro de l'algorithme du volca FM en fonction de celui du 
 * Yamaha FB-01 en désactivant les opérateurs 1 et 2
 */

byte algorithm_num[8] = {0, 13, 7, 6, 4, 21, 30, 31};

/**
 * @brief Tableau des valeurs maximales de chacun des 156 paramètres FM.
 */
byte parameters_max[156] = {
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, M, M, M, 3, 3, 7, 3, 7, M, 1, 31, M, 14,  
    M, M, M, M, M, M, M, M, // PITCH EG
    31, 7, 1,
    M, M, M, M, 1, 5, // LFO
    7,
    48, // TRANSPOSE
    127, 127, 127, 127, 127, 127, 127, 127, // VOICE NAME
    63 // OP ON/OFF
};

/**
 * @return la valeur du paramètre FM
 */
byte get_value(byte index) {
    return parameters_value[index];
}


/**
 * @return la valeur max du paramètre FM
 */
byte get_max(byte index) {
    return parameters_max[index];
}

/**
 * @brief attribue la valeur au paramètre FM.
 *
 * Ici pas de sysex mais 3 NRPNs pour le firmware 1.09.
 */
void set_value(byte index, byte value) {
    if (index == ALGORITHM) {
        current_algo = value;
        value = algorithm_num[value];
    }
    parameters_value[index] = value;
    /*
     * Par exemple, pour définir l'ALGORITHME (numéro de paramètre 134) 
     * sur "22", vous enverrez CC99:1 (MSB NRPN#), CC98:6 (LSB NRPN#) et 
     * enfin CC6:22 (valeur NRPN) etc. ou 
     * B0 63 01 B0 62 06 B0 06 16 pour le canal MIDI 1.
     */
    byte NRPNsArray[9] = {
        0xB0, 0x63, 0x00, 0xB0, 0x62, 0x00, 0xB0, 0x06, 0x00
    };
    NRPNsArray[2] = index >> 7;
    NRPNsArray[5] = index & 127;
    NRPNsArray[8] = value;
    MIDI.sendSysEx(9, NRPNsArray, true);
    if (index == 68 || index == 47 || index == 26 || index == 5) {
        NRPNsArray[5]++;
        MIDI.sendSysEx(9, NRPNsArray, true); // L2 = L3
        NRPNsArray[5] = NRPNsArray[5] - 4;
        NRPNsArray[8] = 0;
        MIDI.sendSysEx(9, NRPNsArray, true); // R3 = 0
    }
}

/**
 * @brief gestion particulière de l'algorithme FM
 */
void change_algo(int8_t rotation) {
    if (rotation > 0) {
        current_algo = (current_algo + 1) % 8;
    } else if (rotation < 0) {
        current_algo = (current_algo + 7) % 8;
    }
    set_page_index(current_algo + 7);
    set_value(ALGORITHM, current_algo);
    /** 
    * messages NRPN pour mettre off les opérateurs 1 et 2
    */
    set_value(155, 15);
}
