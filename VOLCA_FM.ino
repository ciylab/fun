/**
 * @file VOLCA_FM.ino
 * @brief Fichier principal du projet 
 * 
 * Le module envoie des messages NRPN activés par :
 * - envoie de CC extérieurs
 * - encodeurs 
 * Il est configuré pour simuler le Yamaha FB-01 de 1986
 * (4 opérateurs et 8 algorithmes) sur le Korg Volca FM.
 *
 * Une voix nommée MYPATCH est initialisée sur le volca avec
 * des paramètres par défaut qu'il reste à modifier en live.
 * Cette voix est générée par un script python.
 *
 * @author Pierrick MEIGNEN 
 * @version 0.0.0
 * @date 21/04/2026 
 */
 
#include <MIDI.h>
#include <U8x8lib.h>
#include <Versatile_RotaryEncoder.h>

/*** base ***/
#include "src/Display.h"
#include "src/encoder.h"
#include "src/midi.h"
#include "src/volca.h"

Display oled; /**<l'écran */
/**le tableau des deux encodeurs */
Versatile_RotaryEncoder *versatile_encoder[NUM_ENC];
/**instance MIDI*/

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

int sws[NUM_ENC]  = {10, 20}; /**<pin des switchs */
int dts[NUM_ENC]  = {11, 21}; /**<pin des datas */
int clks[NUM_ENC] = {12, 17}; /**<pin des clocks */

void setup() {
    /**************** l'écran **********************************/
    oled.begin();    
    oled.welcome();
    /**************** déclaration des in/out ************************/
    pinMode(LED_CLOCK, OUTPUT);
    for (int i = 0; i < NUM_ENC; i++) {
        pinMode(clks[i], INPUT_PULLUP);
        pinMode(dts[i], INPUT_PULLUP);
        pinMode(sws[i], INPUT_PULLUP);
    }
    for (int i = 0; i < NUM_ENC; i++) {
        versatile_encoder[i] =
            new Versatile_RotaryEncoder(clks[i], dts[i], sws[i]);
    }
    versatile_encoder[0]->setHandleRotate(handleRotate0);    
    versatile_encoder[0]->setHandlePress(handlePress0);
    versatile_encoder[0]->setHandleLongPress(handleLongPress0);
    versatile_encoder[1]->setHandleRotate(handleRotate1);
    versatile_encoder[1]->setHandlePress(handlePress1);
    /**************** partie MIDI **********************************/
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleStart(handleStart);
    MIDI.setHandleStop(handleStop);
    MIDI.setHandleNoteOn(handleNoteOn);   // Listen to note on
    MIDI.setHandleNoteOff(handleNoteOff); // Listen to note off
    MIDI.setHandleControlChange(handleControlChange);
    /** pause pour lire les informations affichées *****************/
    delay(2000);
}

/**
 * @brief La boucle principale
 */

void loop() {
    MIDI.read();
    for (int i = 0; i < NUM_ENC; i++) {
        versatile_encoder[i]->ReadEncoder();
    }
    oled.display();
}

