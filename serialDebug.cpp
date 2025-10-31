//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____           _       _ _____       _                                  
//      / ____|         (_)     | |  __ \     | |                                 
//     | (___   ___ _ __ _  __ _| | |  | | ___| |__  _   _  __ _   ___ _ __  _ __ 
//      \___ \ / _ \ '__| |/ _` | | |  | |/ _ \ '_ \| | | |/ _` | / __| '_ \| '_ \
//      ____) |  __/ |  | | (_| | | |__| |  __/ |_) | |_| | (_| || (__| |_) | |_) |
//     |_____/ \___|_|  |_|\__,_|_|_____/ \___|_.__/ \__,_|\__, (_)___| .__/| .__/
//                                                          __/ |     | |   | |   
//                                                         |___/      |_|   |_|    
// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// uint8_t currentMenuDepth = 0;
// typedef struct 
// {
//   const char** menuList;    // Pointeur vers le menu
//   uint8_t menuSize;         // Nombre d'éléments du menu
//   uint8_t selectedIndex;    // Index sélectionné dans ce menu
//   char title[21];           // Titre du menu (20 caractères + \0)
// } menuLevel_t;

// ---------------------------------------------------------------------------*
// @brief Affiche la structure du Menu
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintMenuStruct(menuLevel_t* menu)
{
  sprintf(serialbuf,"\ndebugSerialPrintMenuStruct(): currentMenuDepth %d / title  %s / menuSize %d /selectedIndex %d", currentMenuDepth, menu->title, menu->menuSize, menu->selectedIndex);
                        // /* menuList %s /16 // ---------------------------------------------------------------------------*=>  /*menu->menuList[selectedModeIndex],
  debugSerial.println(serialbuf); 
}

// ---------------------------------------------------------------------------*
// @brief Affiche la structure de saisir d'une Liste
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
// listInputContext_t listInputCtx = {LIST_INPUT_IDLE, 0, 6, false, 0, false, 0, "", NULL};
/*
typedef struct 
{
  listInputState_t state;     // État actuel
  uint8_t selectedIndex;      // Index de l'élément sélectionné
  uint8_t scrollOffset;       // Décalage de défilement pour l'affichage
  uint8_t maxItems;           // Nombre maximum d'éléments dans la liste
  uint8_t lastScrollOffset;   // Dernier décalage affiché (pour détecter changement)
  uint8_t lastSelectedIndex;  // Dernier index sélectionné (pour détecter changement)
  bool lastCursorBlink;       // Dernier état du clignotement
  bool displayRefresh;        // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;   // Dernier rafraîchissement affichage
  bool cursorBlink;           // État du clignotement curseur
  unsigned long lastBlink;    // Dernier clignotement
  unsigned long lastActivity;    // Dernière activité utilisateur
  unsigned long timeoutDuration;  // Durée du timeout en ms
  char title[21];             // Titre de la sélection (20 caractères + \0)
  const char** itemList;      // Pointeur vers la liste des éléments
} listInputContext_t;
*/
void debugSerialListStruct(void)
{ // cas d'une saisie dans Liste:
  sprintf(serialbuf,"debugSerialListStruct(): state %d / selectedIndex %d / scrollOffset %d/ maxItems %d / lastScrollOffset %d /\
                     \nlastSelectedIndex %d / lastCursorBlink %d / displayRefresh %d / lastUpdate %d\
                     \ncursorBlink %d / lastActivity %d / timeoutDuration %d / lastBlink %d / title %s / itemList[2] %s ",
                    listInputCtx.state,
                    listInputCtx.selectedIndex,
                    listInputCtx.scrollOffset,
                    listInputCtx.maxItems,
                    listInputCtx.lastScrollOffset,
                    listInputCtx.lastSelectedIndex,
                    listInputCtx.lastCursorBlink,                    
                    listInputCtx.displayRefresh,
                    listInputCtx.lastUpdate,
                    listInputCtx.cursorBlink,
                    listInputCtx.lastActivity,
                    listInputCtx.timeoutDuration,                    
                    listInputCtx.title,
                    listInputCtx.itemList[2]                    
                    );
  debugSerial.println(serialbuf); 
}


// ---------------------------------------------------------------------------*
// @brief Affiche la structure de saisir d'un Nombre
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
// numberInputContext_t numberInputCtx = {NUMBER_INPUT_IDLE, 0, "", 10, false, 0, false, 0, "", false};
/*
typedef struct 
{
  numInputState_t state;      // État actuel
  uint8_t position;           // Position du curseur
  uint8_t lastPosition;       // Dernière position affichée
  uint8_t length;             // Longueur actuelle du nombre
  uint8_t lastLength;         // Dernière longueur affichée
  uint8_t maxLength;          // Longueur maximale autorisée
  char workingNum[21];        // Nombre de travail (20 chars max + '\0')
  char lastDisplayedNum[17];  // Dernière portion affichée (16 chars + '\0')
  bool displayRefresh;        // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;   // Dernier rafraîchissement affichage
  bool cursorBlink;           // État du clignotement curseur
  bool lastCursorBlink;       // Dernier état du clignotement
  unsigned long lastBlink;    // Dernier clignotement
  uint8_t displayOffset;      // Décalage pour affichage (scroll horizontal)
  uint8_t lastDisplayOffset;  // Dernier décalage affiché
  uint8_t lastCursorOffset;   // Dernier offset quand le curseur a été affiché
  uint8_t displayWidth;       // Largeur d'affichage (nb de caractères visibles)
  unsigned long lastActivity;    // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool allowNegative;         // Autoriser les nombres négatifs
  bool allowDecimal;          // Autoriser les décimales
  long minValue;              // Valeur minimale autorisée
  long maxValue;              // Valeur maximale autorisée
  bool firstDisplay;          // Premier affichage
  uint8_t lastTimeoutValue;   // Dernière valeur de timeout affichée
} numInputContext_t;
*/
void debugSerialPrintNumberStruct(void)
{ // cas d'une saisie numérique:
sprintf(serialbuf,"state %d / position : %d / workingNumber %s / maxLength %d / displayRefresh %d\nlastUpdate %d / cursorBlink %d / lastBlink %d / /*title %s /*/ allowNegative %d ",
                    numInputCtx.state,
                    numInputCtx.position,
                    numInputCtx.workingNum,
                    numInputCtx.maxLength,
                    numInputCtx.displayRefresh,
                    numInputCtx.lastUpdate,
                    numInputCtx.cursorBlink,
                    numInputCtx.lastBlink,
//                    numInputCtx.title,                    
                    numInputCtx.allowNegative );
  debugSerial.println(serialbuf); 
}

// ---------------------------------------------------------------------------*
// @brief Affiche la structure de saisir d'un Alphnumerique
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
// stringInputContext_t stringInputCtx = {STRING_INPUT_IDLE, 0, "", 20, false, 0, false, 0, ""};
/* 
typedef struct 
{
    stringInputState_t state;   // État actuel
    uint8_t position;           // Position du curseur (0-maxLength-1)
    char workingString[21];     // Chaîne de travail (20 chars + '\0')
    uint8_t maxLength;          // Longueur maximum de la chaîne
    bool displayRefresh;        // Flag pour rafraîchir l'affichage
    unsigned long lastUpdate;   // Dernier rafraîchissement affichage
    bool cursorBlink;           // État du clignotement curseur
    unsigned long lastBlink;    // Dernier clignotement
    char title[21];             // Titre de la saisie
} stringInputContext_t;
*/
void debugSerialPrintStringStruct(void)
{ // cas d'une saisie numérique:
  sprintf(serialbuf,"state %d / position : %d / workingNumber %s / maxLength %d / displayRefresh %d\nlastUpdate %d / cursorBlink %d / lastBlink %d / title %s ",
                    stringInputCtx.state,
                    stringInputCtx.position,
                    stringInputCtx.workingString,
                    stringInputCtx.maxLength,
                    stringInputCtx.displayRefresh,
                    stringInputCtx.lastUpdate,
                    stringInputCtx.cursorBlink,
                    stringInputCtx.lastBlink,
                    stringInputCtx.title);
  debugSerial.println(serialbuf); 
}


// ---------------------------------------------------------------------------*
// @brief Affiche heure et status Alarmes sur SerialDebug
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void debugSerialTestConnexionDS3231(void)
{
    debugSerial.println("=== TEST CONNEXION DS3231 ===");
    
    // Test lecture heure
    DateTime now = rtc.now();
    debugSerial.print("Heure lue: ");
    debugSerial.print(now.hour()); debugSerial.print(":");
    debugSerial.print(now.minute()); debugSerial.print(":");
    debugSerial.println(now.second());
    
    // Test état alarmes
    debugSerial.print("Alarme 1 fired: ");
    debugSerial.println(rtc.alarmFired(1) ? "OUI" : "NON");
    debugSerial.print("Alarme 2 fired: ");
    debugSerial.println(rtc.alarmFired(2) ? "OUI" : "NON");
    
    // Test pin interruption
    debugSerial.print("Pin interruption état: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    debugSerial.println("=== FIN TEST DS3231 ===");
}


// ---------------------------------------------------------------------------*
// @brief Affiche l'heure système et l'heure RTC côte à côte pour comparaison
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintTimeComparison(void)
{
    DateTime systemTime = rtc.now();
    DateTime rtcTime = rtc.now();
    debugSerial.println();
    debugSerial.println("=== COMPARAISON DES HORLOGES ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    debugSerial.print("RTC:     ");
    debugSerial.print(rtcTime.day()); debugSerial.print("/");
    debugSerial.print(rtcTime.month()); debugSerial.print("/");
    debugSerial.print(rtcTime.year()); debugSerial.print(" ");
    debugSerial.print(rtcTime.hour()); debugSerial.print(":");
    debugSerial.print(rtcTime.minute()); debugSerial.print(":");
    debugSerial.println(rtcTime.second());
    
    long diff = systemTime.unixtime() - rtcTime.unixtime();
    debugSerial.print("Différence: ");
    debugSerial.print(diff);
    debugSerial.println(" secondes");
    debugSerial.println("===============================");
}

// ---------------------------------------------------------------------------*
// @brief Affiche l'heure et la date système sur le port série
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintTime(void)
{
    DateTime systemTime = rtc.now();
    debugSerial.println();
    debugSerial.println("=== HEURE SYSTÈME ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    debugSerialPrint2digits(systemTime.hour());
    debugSerial.print(":");
    debugSerialPrint2digits(systemTime.minute());
    debugSerial.print(":");
    debugSerialPrint2digits(systemTime.second());
    debugSerial.print(" ");
    debugSerialPrint2digits(systemTime.day());
    debugSerial.print("/");
    debugSerialPrint2digits(systemTime.month());
    debugSerial.print("/");
    debugSerialPrint2digits(systemTime.year());
    debugSerial.println("");
}

// ---------------------------------------------------------------------------*
// @brief Affiche un nombre avec 2 chiffres minimum
// @param number Nombre à afficher
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrint2digits(int number) 
{
  if (number >= 0 && number < 10) 
  {
      debugSerial.write('0');
  }
  debugSerial.print(number);
}

// ---------------------------------------------------------------------------*
// @brief Construit et affiche le message HEXA sur le port série de debug
// @param payload Pointeur vers les données
// @param len Longueur des données
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintLoraPayload(uint8_t *payload, uint8_t len)
{ int i;
/*
13:58:16.925 -> hexPayload: 00AC266300000000000000000000000000000
13:58:16.925 -> hexPayload: 3030333033333330333333333333333033333 len : 19
*/
    for (i = 0; i < len; i++) 
    {
        sprintf(&hexPayload[i * 2], "%02X", payload[i]);  
    }
    hexPayload[hexPayloadSize - 1] = '\0';
    
    debugSerial.print("hexPayload: "); 
    debugSerial.print(hexPayload);
    debugSerial.print(" len : "); 
    debugSerial.println(len);
}

// ---------------------------------------------------------------------------*
// @brief Affiche des données au format hexadécimal sur le port série
// @param txt Pointeur vers les données
// @param len Longueur des données
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintHEXA(char *txt, char len)
{ int i;
  char buf[256];
    
    if (COM_DebugSerial)
    {
        memset(buf, 0, 256);
        for (i = 0; i < len; i++)  
        { 
            buf[2*i] = ((txt[i]>>4) < 10) ? (txt[i]>>4) + 48 : (txt[i]>>4) + 55;
            buf[2*i+1] = ((txt[i]&0x0F) < 10) ? (txt[i]&0x0F) + 48 : (txt[i]&0x0F) + 55;
        }
        debugSerial.print(" ");
        buf[2*i] = 0;
        debugSerial.println(buf);
    }
}


// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void debugSerialPrintReprogNextAlarm(int IRQ)  
{ DateTime systemTime = rtc.now();
  
  sprintf(serialbuf,"\nIRQ%d Reprogrammée à %02d:%02d:%02d",
          IRQ, systemTime.hour(),systemTime.minute(),systemTime.second());
  debugSerial.println(serialbuf); 
}


// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void debugSerialPrintNextAlarm(DateTime nextPayload, int IRQ)  
{   
  sprintf(serialbuf,"IRQ%d prévue pour: %02d:%02d:%02d",
          IRQ, nextPayload.hour(),nextPayload.minute(),nextPayload.second());
  debugSerial.println(serialbuf); 
}


// ---------------------------------------------------------------------------*
// ---------------------------------------------------------------------------*
void debugSerialPrintLoRaStatus()  
{
  debugSerial.println("\n=== STATUS LoRa ===");
  sprintf(serialbuf," => RN2483#%02d",Ruche.Num_Carte);        // affiche N° de carte
  debugSerial.println(serialbuf);
//debugSerial.print("DevEUI = ");debugSerial.println((char *)DevEUI,8);
//debugSerial.print("AppEUI = ");debugSerial.println((char *)AppEUI,8);
//debugSerial.print("AppKey = ");debugSerial.println((char *)AppKey,16); 
// print...()
  debugSerial.println("=====================");
}  

// ---------------------------------------------------------------------------*
// @brief Fonction de test du clavier analogique
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void debugSerialPrintKbdKey(void) 
{ key_code_t key = readKey();
    
  switch (key) 
  {
    case KEY_NONE:
        break;
          
    case KEY_1:
        debugSerial.println("Touche 1 pressée");
        break;
            
    case KEY_2:
        debugSerial.println("Touche 2 pressée");
        break;
            
    case KEY_3:
        debugSerial.println("Touche 3 pressée");
        break;
            
    case KEY_4:
        debugSerial.println("Touche 4 pressée");
        break;
            
    case KEY_5:
        debugSerial.println("Touche 5 pressée");
        break;
           
    case KEY_INVALID:
        debugSerial.print("Erreur de lecture clavier - valeur: ");
        debugSerial.println(analogRead(KBD_ANA));
        break;
            
    default:
        debugSerial.println("Code de touche inconnu");
        break;
  }
  delay(100);
}


// ---------------------------------------------------------------------------*
// @brief Affiche les informations système sur debugSerial
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void serialDebugPrintSystemInfo(void)
{
    debugSerial.println("=== INFORMATIONS SYSTEME ===");
    debugSerial.println("Projet: " PROJECT_NAME);
    debugSerial.println("Version: " VERSION);
    debugSerial.print("Mode actuel: ");
    debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
    debugSerial.print("Configuration version: ");
    debugSerial.print(config.materiel.version/100);
    debugSerial.print(".");
    debugSerial.println(config.materiel.version%100);
    debugSerial.print("Compilation: ");
    debugSerial.print(__DATE__);
    debugSerial.print(" ");
    debugSerial.println(__TIME__);
}
