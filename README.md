# EN326_LINO_MAX
Projet Capteur pour l'embarqué.
Groupe ALBERTY Maxime & DELPECH Lino.

## 1 - Présentation globale
L'objectif de ce projet est de réaliser une station météo sans fil.
Elle sera composée d'une partie émission distante minue de différents capteurs, et d'une partie réception locale reliée à un ordinateur.
Le système devra être capable d'afficher sur l'écran de l'ordinateur mis à disposition les valeurs instantanées des capteurs embarqués.

Les deux modules sont composés :
 - D'une carte Arduino UNO
 - D'un shield Wireless Proto
 - D'un module xBee

## 2 - Capteurs mis à disposition
La partie émission est composé des capteurs suivant :
 - Catpeur de luminosité Vishay TEMT6000
 - Capteur de pression Freescale MPL115A2 (donnant également une mesure de température)
 - Capteur d'humidité/température Sensiron SHT15 ou RHT03

## 3 - Utilisation
Le projet Main_project correspond à la partie emission avec les capteurs.
La projet Main_recepteur correspond à la partie recepteur, interfacée avec Processing
L'interface processing est le projet Station_Meteo_Xbee_V2
