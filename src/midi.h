#ifndef MIDI_H
#define MIDI_H

#define LED_CLOCK 2 /**<pin de la led indiquant le bpm */

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleClock();
void handleStart();
void handleStop();

#endif
