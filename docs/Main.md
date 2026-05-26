## PRÉSENTATION
Le code permet au module MIDI TOOLS de modifier, par NPRN, 59 des 156 paramètres du Volca FM sous firmware 1.09, réduit à 4-OP pour simplifier la maîtrise FM sur le modèle du Yamaha FB-01. Un contrôleur MIDI externe peut aussi être utilisé (CC1 à CC59).

## MODE D'EMPLOI
À la mise sous tension, l'écran affiche pendant quelques secondes un message d'information finissant par le numéro de version :

~~~~~~~
 SEND SYSEX TO   
 VOLCA FM 4-OP   
 BY CIYLAB       
 vx.y.z
~~~~~~~

Pour choisir les paramètres, il faut tourner l'encodeur PARAMETER au-dessus de l'écran.

~~~~~~~
 ALGO    FDBCK   
 ADSR    MINLVL  
 OUT    >LFO     
 PITCH   SCALING
~~~~~~~

Ce même encodeur PARAMETER permet (sauf pour le choix de l'algorithme et du feedback) d'afficher la liste des paramètres par simple pression et de sélectionner le paramètre qu'on souhaite modifier.

~~~~~~~
>SPEED   DELAY   
 DEPTH   WAVE    
 PMS

~~~~~~~

Une nouvelle pression permet de revenir au menu principal.

Une fois le paramètre sélectionné, l'encodeur VALUE en dessous de l'écran permet d'en modifier la valeur. À noter qu'une rotation d'un seul cran de l'encodeur VALUE affiche la valeur sans la modifier. Une pression sur cet encodeur affiche le nom du paramètre.

**Reboot** : une pression longue sur l'encodeur PARAMETER redémarre le module.

**Control Change** : le module réagit aux messages CC dont le numéro est celui de l'un des 45 paramètres. Par exemple le CC 2 gère la quantité de feedback.

## FDBCK

Le niveau de ré-insertion du signal. Provoque en général une saturation assimilable à du bruit.

