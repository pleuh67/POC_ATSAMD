/*
Page info => OK
config systeme => valisd pour retour
Config LoRa => menuLora = Dev eui attend HAX sans affichage page saisie, le reste retourne menuLoRa
Calib Tens => menu calib => vbat page Vbat, idem Vlum/ VSol
                             Reserve => ouvre menu reserve
                                0 affich + retour
                                1 se perd....


*/
//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
// ---------------------------------------------------------------------------*
//
//       _____ _                 _                       
//      / ____| |               | |                      
//     | |    | | __ _ _   _  __| | ___   ___ _ __  _ __ 
//     | |    | |/ _` | | | |/ _` |/ _ \ / __| '_ \| '_ \
//     | |____| | (_| | |_| | (_| |  __/| (__| |_) | |_) |
//      \_____|_|\__,_|\__,_|\__,_|\___(_)___| .__/| .__/
//                                           | |   | |   
//                                           |_|   |_|   
// ---------------------------------------------------------------------------*
// https://claude.ai/chat/9883784a-b682-4936-8d53-0885169eba94
// ---------------------------------------------------------------------------*
/*
On va reprendre un projet en cours, voici ou j'en suis:

utilisation d'une carte SODAQ EXPLORER a base de atsamd21j18 sur arduino avec DS3231 avec la library DS3231 de andrew wickert, 
CONTRAINTES OBLIGATOIRES :
- je veux le code en artefact
- Les prototypes de fonction sont en début de fichier
- Style d'accolades : alignement vertical 
- indentation 2 caractères
- Noms : fonctions et variables en camelCase anglais, #define en MAJUSCULES_ANGLAIS
- je souhaite le prototype des fonctions
- Commentaires : français avec accents
- Textes affichés : français SANS accents
- Documentation complète : toutes fonctions commentées avec description, paramètres, codes retour
- n'utilise pas
/ completer  /
pour les commentaires de fonctions mais //
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

utilisation d'un clavier 5 touches analogique:
// Alias pour les touches selon usage
MOINS, PLUS, LEFT , RIGHT, VALIDE , UP, DOWN

L'écran OLED utilise la taille de police 1 donc dispose de 8 lignes de 20 caractères, on ecrit dessus avec
OLEDDrawText(taille, ligne, colonne, texte)
param 1 : taille = 1
Param 2 : Ligne de 0 à 7
param 3 : Colone de 0 à 19
param 4 : texte à afficher


On ne s'occupe que de la saisie non bloquante, le reste est déja opérationnel.

Le titre de la fenêtre de saisie doit être paramètrable, remplace void startNumInput(const char* initialNum, uint8_t maxLen, bool allowNeg, bool allowDec, long minVal, long maxVal);
par void startNumInput(const char* title, const char* initialNum, uint8_t maxLen, bool allowNeg, bool allowDec, long minVal, long maxVal);

ajoute     char title[21];             // Titre de la saisie
a la fin de la struct numInputContext_t;
  
utilise  cette base de code:



//
// Exemple de sélection de Spreading Factor avec liste
//
void selectSpreadingFactorFromList(void)
{
  static char selectedSF[21] = "";
  
  // Liste des SF valides uniquement
  const char* sfList[] = {
    "SF7",
    "SF9",
    "SF12"
  };
  
  // Lancer la sélection sans timeout
  startListInput("SPREADING FACTOR:", sfList, 3, 1); // SF9 par défaut (index 1)
  
  // Dans loop()
  listInputState_t state = processListInput();
  
  if (state == LIST_INPUT_COMPLETED)
  {
    uint8_t index = finalizeListInput(selectedSF);
    
    debugSerial.print("SF selectionne: ");
    debugSerial.println(selectedSF);
    
    // Extraire la valeur numérique
    uint8_t sfValue = 0;
    if (strcmp(selectedSF, "SF7") == 0)
      sfValue = 7;
    else if (strcmp(selectedSF, "SF9") == 0)
      sfValue = 9;
    else if (strcmp(selectedSF, "SF12") == 0)
      sfValue = 12;
    
    if (sfValue > 0)
    {
      lorawan.setSpreadingFactor(sfValue);
      debugSerial.print("SF configure: ");
      debugSerial.println(sfValue);
    }
  }
}

*/


 
// ---------------------------------------------------------------------------*
