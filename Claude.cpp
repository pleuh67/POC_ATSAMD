// https://claude.ai/chat/9883784a-b682-4936-8d53-0885169eba94

/*

 
On va reprendre un projet en cours, voici ou j'en suis:

utilisation d'une carte SODAQ EXPLORER a base de atsamd21j18 sur arduino avec DS3231 avec la library DS3231 de andrew wickert, 
CONTRAINTES OBLIGATOIRES :
- Les prototypes de fonction sont en début de fichier
- Style d'accolades : alignement vertical 
- indentation 2 caractères
- Noms : fonctions et variables en camelCase anglais, #define en MAJUSCULES_ANGLAIS
- je souhaite le prototype des fonctions
- Commentaires : français avec accents
- Textes affichés : français SANS accents
- Documentation complète : toutes fonctions commentées avec description, paramètres, codes retour
- Debug : #define debugSerial SerialUSB et #define DEBUG_BAUD 115200
- Ajouter #define EXPLORER, #define PIN_PE BUTTON, #define RTC_INTERRUPT_PIN 2
dans setup() on mets:
// Port Série Debug ----- initialisation ----- infos projet -----
  debugSerial.begin(DEBUG_BAUD);    // SETUP Serial
  while ((!debugSerial) && (millis() < SERIAL_TIMEOUT)){}     // Wait 5 seconds 
  debugSerial.println( "Compiled: "DATE", "TIME", "VERSION );  //", "BOARD);
  debugSerial.println(PROJECT_NAME);

J'utilise : 
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_SH110X.h>
#include <Sodaq_RN2483.h>
#include <ArduinoLowPower.h>
#include <stdint.h>

utilisation d'un clavier 5 touches analogique

L'écran OLED utilise la taille de police 1 donc dispose de 8 lignes de 20 caractères, on ecrit dessus avec
OLEDDrawText()
param 1 : taille = 1
Param 2 : Ligne de 0 à 7
param 3 : Colone de 0 à 19
param 4 : chaine à afficher


en continuant sur cette base de code:






On ne s'occupe que de la saisie non bloquante, le reste est déja opérationnel.



Supprime sur ce code la partie saisie dans la liste


 
 
 
 
 
 
 
 */
