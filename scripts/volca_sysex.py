# fichier : volca_sysex.py
# auteur : Pierrick MEIGNEN
# date : 2026-5-28
# licence : GNU GPL version 3

"""
Ce module permet de construire un patch pour le volca FM au format sysex 
puis de l'envoyer dans le synthétiseur. Les 156 valeurs des paramètres de
ce patch sont stockées dans un hearder utilisé par le module pour définir
les valeurs par défaut. Ainsi la manipulation par l'interface affiche dès 
le démarrage les bonnes valeurs. Les données sont donc synchronisées.
"""

import time
import rtmidi
import rtmidi.midiutil

OUTPUT_PORT = 1

def test_io():
    rtmidi.midiutil.list_output_ports()

def create_default_patch():
    """
    Cette fonction retourne une liste d'octets constituant une configuration
    des paramètres pour une voix du volca FM.
    """
    begin = [0xF0, 0x43, 0x00, 0x00, 0x01, 0x1B]
    op = [
            99, 99, 0, 0, # RATE
            99, 99, 99, 99, # LEVEL
            0, 0, 0, 0, 0, 0, # SCALING
            0, 0, 50, # SENS., OUTPUT LEVEL
            0, 3, 0, 0] # OSC MODE, COARSE, FINE, DETUNE
    all = [
            50, 50, 50, 50, 50, 50, 50, 50, # PITCH EG
            0, 0, 1, # ALGO, FEEDBACK, OSC. SYNC
            0, 0, 0, 0, 0, 0, # LFO
            0, 0] # PMS, TRANSPOSE
    name = [ord(c) for c in 'MYPATCH   '] # VOICE NAME
    end = [63, 0xF7] # OP ON/OFF non pris en compte par le volca
    config = op * 4 + [0] * 2 * len(op) + all # OP 6 à 3 + OP 2 et 1
    config[16 + 3 * 21] = 99 # OUTPUT LEVEL OP 3 au max
    sysex = begin + config + name + end
    return bytes(sysex)

def export_header(sysex):
    '''
    Cette fonction prend en argument une voix au format sysex
    et la sauvegarde dans un header. Cet header sera utilisé
    comme paramètres par défaut à l'initialisation du module.
    '''
    f = open("../src/mypatch.h", "w")
    f.write("byte parameters_value[] = {\n")
    count = 0
    for val in sysex[6:-1]:
        f.write(str(val) + ", ")
        count += 1
        if count % 21 == 0:
            f.write("\n")
    f.write("};\n")
    f.close()

def save_patch(file_name):
    """
    Cette fonction crée le patch et le sauvegarde dans un fichier.
    """
    sysex = create_default_patch()    
    export_header(sysex)
    with open(file_name + ".syx", "wb") as f:
        f.write(sysex)

def make_sound(note, out):
    """
    Cette fonction envoie un son qui peut servir d'avertissement.
    """
    mess = [0x90, note, 100]
    out.send_message(mess)
    time.sleep(0.5)
    mess = [0x80, note, 0]
    out.send_message(mess)
    
def send_patch(file_name):
    """ 
    Cette fonction se substitue à la commande :
    $ amidi -p hw:1,0,0 -s file_name
    Un son avant et après est envoyé comme signal de chargement.
    """
    midi_out = rtmidi.MidiOut()
    midi_out.open_port(OUTPUT_PORT)
    # avertissement de début
    make_sound(60, midi_out)
    # send patch
    in_file = open(file_name + ".syx", "rb")
    mess = in_file.read()
    in_file.close()
    midi_out.send_message(mess)
    # avertissement de fin
    make_sound(72, midi_out)
    midi_out.close_port()
    del midi_out

if __name__ == '__main__':
    file_name = "../patchs/mypatch"
    save_patch(file_name)
    send_patch(file_name)
