//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//       _____ _                   _     _    
//      / ____| |                 | |   | |   
//     | (___ | |_ _ __ _   _  ___| |_  | |__ 
//      \___ \| __| '__| | | |/ __| __| | '_ \
//      ____) | |_| |  | |_| | (__| |_ _| | | |
//     |_____/ \__|_|   \__,_|\___|\__(_)_| |_|
//                                             
// ---------------------------------------------------------------------------*


// ===== ÉNUMÉRATIONS =====
typedef enum 
{
    KEY_NONE = 0,        // Pas de touche pressée (état normal)
    KEY_1 = 1,           // Touche 1
    KEY_2 = 2,           // Touche 2  
    KEY_3 = 3,           // Touche 3
    KEY_4 = 4,           // Touche 4
    KEY_5 = 5,           // Touche 5
    KEY_INVALID = 255    // Valeur analogique invalide/hors plages
} key_code_t;

// ---------------------------------------------------------------------------*
// États possibles de la sélection dans une liste
// ---------------------------------------------------------------------------*
typedef enum 
{
    LIST_INPUT_IDLE,        // 0 : Pas de sélection en cours
    LIST_INPUT_ACTIVE,      // 1 : Sélection en cours
    LIST_INPUT_COMPLETED,   // 2 : Sélection terminée avec succès
    LIST_INPUT_CANCELLED    // 3 : Sélection annulée
} listInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie numérique
// ---------------------------------------------------------------------------*
typedef enum 
{
  NUM_INPUT_IDLE,         // Pas de saisie en cours
  NUM_INPUT_ACTIVE,       // Saisie en cours
  NUM_INPUT_COMPLETED,    // Saisie terminée avec succès
  NUM_INPUT_CANCELLED     // Saisie annulée
} numInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie alphanumérique
// ---------------------------------------------------------------------------*
typedef enum 
{
    STRING_INPUT_IDLE,      // 0 : Pas de saisie en cours
    STRING_INPUT_ACTIVE,    // 1 : Saisie en cours
    STRING_INPUT_COMPLETED, // 2 : Saisie terminée avec succès
    STRING_INPUT_CANCELLED  // 3 : Saisie annulée
} stringInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie hexadécimale
// ---------------------------------------------------------------------------*
typedef enum 
{
  HEX_INPUT_IDLE,         // Pas de saisie en cours
  HEX_INPUT_ACTIVE,       // Saisie en cours
  HEX_INPUT_COMPLETED,    // Saisie terminée avec succès
  HEX_INPUT_CANCELLED     // Saisie annulée
} hexInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie d'heure
// ---------------------------------------------------------------------------*
typedef enum 
{
  TIME_INPUT_IDLE,        // Pas de saisie en cours
  TIME_INPUT_ACTIVE,      // Saisie en cours
  TIME_INPUT_COMPLETED,   // Saisie terminée avec succès
  TIME_INPUT_CANCELLED    // Saisie annulée
} timeInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie de date
// ---------------------------------------------------------------------------*
typedef enum 
{
  DATE_INPUT_IDLE,        // Pas de saisie en cours
  DATE_INPUT_ACTIVE,      // Saisie en cours
  DATE_INPUT_COMPLETED,   // Saisie terminée avec succès
  DATE_INPUT_CANCELLED    // Saisie annulée
} dateInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie email
// ---------------------------------------------------------------------------*
typedef enum 
{
  EMAIL_INPUT_IDLE,         // Pas de saisie en cours
  EMAIL_INPUT_ACTIVE,       // Saisie en cours
  EMAIL_INPUT_COMPLETED,    // Saisie terminée avec succès
  EMAIL_INPUT_CANCELLED     // Saisie annulée
} emailInputState_t;

// ---------------------------------------------------------------------------*
// États possibles de la saisie IP
// ---------------------------------------------------------------------------*
typedef enum 
{
  IP_INPUT_IDLE,         // Pas de saisie en cours
  IP_INPUT_ACTIVE,       // Saisie en cours
  IP_INPUT_COMPLETED,    // Saisie terminée avec succès
  IP_INPUT_CANCELLED     // Saisie annulée
} ipInputState_t;


// ---------------------------------------------------------------------------*
// États possibles de l'affichage d'écran d'info
// ---------------------------------------------------------------------------*
 typedef enum 
{
  INFO_SCREEN_IDLE,       // Pas d'écran actif
  INFO_SCREEN_ACTIVE,     // Écran affiché
  INFO_SCREEN_CLOSED      // Écran fermé
} infoScreenState_t;


// ===== STRUCTURES =====

// Variables pour readKey() non-bloquant
typedef struct {
    key_code_t derniereTouche;
    key_code_t toucheStable;
    int compteStable;
    unsigned long derniereLecture;
    bool toucheDisponible;
} clavier_context_t;


// ---------------------------------------------------------------------------*
// Contexte de sélection dans une liste
// ---------------------------------------------------------------------------*
typedef struct 
{
  listInputState_t state;        // État actuel
  uint8_t selectedIndex;         // Index de l'élément sélectionné
  uint8_t scrollOffset;          // Décalage de défilement pour l'affichage
  uint8_t maxItems;              // Nombre maximum d'éléments dans la liste
  uint8_t lastScrollOffset;      // Dernier décalage affiché (pour détecter changement)
  uint8_t lastSelectedIndex;     // Dernier index sélectionné (pour détecter changement)
  bool lastCursorBlink;          // Dernier état du clignotement
  bool displayRefresh;           // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;      // Dernier rafraîchissement affichage
  bool cursorBlink;              // État du clignotement curseur
  unsigned long lastBlink;       // Dernier clignotement
  unsigned long lastActivity;    // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  char title[21];                // Titre de la sélection (20 caractères + \0)
  const char** itemList;         // Pointeur vers la liste des éléments
} listInputContext_t;

// ---------------------------------------------------------------------------*
// Structure pour gérer la pile de menus
// ---------------------------------------------------------------------------*
typedef struct 
{
  const char** menuList;    // Pointeur vers le menu
  uint8_t menuSize;         // Nombre d'éléments du menu
  uint8_t selectedIndex;    // Index sélectionné dans ce menu
  char title[21];           // Titre du menu (20 caractères + \0)
} menuLevel_t;


// ---------------------------------------------------------------------------*
// Contexte de saisie numérique
// ---------------------------------------------------------------------------*
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
  char title[21];             // Titre de la saisie
} numInputContext_t;

// ---------------------------------------------------------------------------*
// Contexte de saisie alphanumérique
// ---------------------------------------------------------------------------*
typedef struct 
{
  stringInputState_t state;      // État actuel
  uint8_t position;              // Position du curseur
  uint8_t lastPosition;          // Dernière position affichée
  uint8_t maxLength;             // Longueur maximale autorisée
  char workingString[21];        // Chaîne de travail (20 chars max + '\0')
  char lastDisplayedString[21];  // Dernière chaîne affichée
  bool displayRefresh;           // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;      // Dernier rafraîchissement affichage
  bool cursorBlink;              // État du clignotement curseur
  bool lastCursorBlink;          // Dernier état du clignotement
  unsigned long lastBlink;       // Dernier clignotement
  unsigned long lastActivity;    // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool firstDisplay;             // Premier affichage
  uint8_t lastTimeoutValue;      // Dernière valeur de timeout affichée
  char title[21];                // Titre de la saisie                
} stringInputContext_t;


// ---------------------------------------------------------------------------*
// Contexte de saisie hexadécimale
// ---------------------------------------------------------------------------*
typedef struct 
{
  hexInputState_t state;         // État actuel
  uint8_t position;              // Position du curseur
  uint8_t lastPosition;          // Dernière position affichée
  uint8_t maxLength;             // Longueur maximale autorisée (1-40)
  char workingHex[41];           // Chaîne hexa de travail (40 chars max + '\0')
  char lastDisplayedHex[17];     // Dernière portion affichée (16 chars + '\0')
  char title[21];                // Titre de la saisie
  bool displayRefresh;           // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;      // Dernier rafraîchissement affichage
  bool cursorBlink;              // État du clignotement curseur
  bool lastCursorBlink;          // Dernier état du clignotement
  unsigned long lastBlink;       // Dernier clignotement
  uint8_t displayOffset;         // Décalage pour affichage (scroll horizontal)
  uint8_t lastDisplayOffset;     // Dernier décalage affiché
  uint8_t lastCursorOffset;      // Dernier offset quand le curseur a été affiché
  uint8_t displayWidth;          // Largeur d'affichage (16 caractères visibles)
  unsigned long lastActivity;    // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool lastValidity;             // Dernière validité affichée
  bool firstDisplay;             // Premier affichage
  uint8_t lastTimeoutValue;      // Dernière valeur de timeout affichée
} hexInputContext_t;

// ---------------------------------------------------------------------------*
// Contexte de saisie d'heure
// ---------------------------------------------------------------------------*
typedef struct 
{
  timeInputState_t state;     // État actuel
  uint8_t position;           // Position du curseur (0-7)
  uint8_t lastPosition;       // Dernière position affichée
  char workingTime[9];        // Heure de travail "HH:MM:SS"
  char lastDisplayedTime[9];  // Dernière heure affichée
  bool displayRefresh;        // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;   // Dernier rafraîchissement affichage
  bool cursorBlink;           // État du clignotement curseur
  bool lastCursorBlink;       // Dernier état du clignotement
  unsigned long lastBlink;    // Dernier clignotement
  unsigned long lastActivity; // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool lastValidity;          // Dernière validité affichée
  bool firstDisplay;          // Premier affichage
  char title[21];             // Titre de la saisie
} timeInputContext_t;

// ---------------------------------------------------------------------------*
// Contexte de saisie de date
// ---------------------------------------------------------------------------*
typedef struct 
{
  dateInputState_t state;     // État actuel
  uint8_t position;           // Position du curseur (0-9)
  uint8_t lastPosition;       // Dernière position affichée
  char workingDate[11];       // Date de travail "JJ/MM/AAAA"
  char lastDisplayedDate[11]; // Dernière date affichée
  bool displayRefresh;        // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;   // Dernier rafraîchissement affichage
  bool cursorBlink;           // État du clignotement curseur
  bool lastCursorBlink;       // Dernier état du clignotement
  unsigned long lastBlink;    // Dernier clignotement
  unsigned long lastActivity; // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool lastValidity;          // Dernière validité affichée
  bool firstDisplay;          // Premier affichage
  char title[21];             // Titre de la saisie
} dateInputContext_t;

// ---------------------------------------------------------------------------*
// Contexte de saisie email
// ---------------------------------------------------------------------------*
typedef struct 
{
  emailInputState_t state;    // État actuel
  uint8_t position;           // Position du curseur (0-39)
  uint8_t lastPosition;       // Dernière position affichée
  uint8_t length;             // Longueur actuelle de l'email
  uint8_t lastLength;         // Dernière longueur affichée
  char workingEmail[41];      // Email de travail (40 chars max + '\0')
  char lastDisplayedEmail[17];// Dernière portion affichée (16 chars + '\0')
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
  bool lastValidity;          // Dernière validité affichée
  uint8_t lastValidityState;  // 0=vide, 1=valide, 2=invalide
  bool firstDisplay;          // Premier affichage
  uint8_t charSetIndex;       // Index dans le jeu de caractères
  uint8_t lastTimeoutValue;   // Dernière valeur de timeout affichée
  char title[21];             // Titre de la saisie
} emailInputContext_t;

// ---------------------------------------------------------------------------*
// Contexte de saisie IP
// ---------------------------------------------------------------------------*
typedef struct 
{
  ipInputState_t state;       // État actuel
  uint8_t position;           // Position du curseur (0-14)
  uint8_t lastPosition;       // Dernière position affichée
  char workingIP[16];         // IP de travail "XXX.XXX.XXX.XXX" + '\0'
  char lastDisplayedIP[16];   // Dernière IP affichée
  bool displayRefresh;        // Flag pour rafraîchir l'affichage
  unsigned long lastUpdate;   // Dernier rafraîchissement affichage
  bool cursorBlink;           // État du clignotement curseur
  bool lastCursorBlink;       // Dernier état du clignotement
  unsigned long lastBlink;    // Dernier clignotement
  unsigned long lastActivity; // Dernière activité utilisateur
  unsigned long timeoutDuration; // Durée du timeout en ms
  bool lastValidity;          // Dernière validité affichée
  bool firstDisplay;          // Premier affichage
  char title[21];             // Titre de la saisie
} ipInputContext_t;






// ===== STRUCTURES DE CONFIGURATION =====
typedef struct 
{
    uint16_t version;
    uint8_t adresseRTC;
    uint8_t adresseOLED;
    uint8_t adresseEEPROM;
    uint8_t reserve[5];
} nonConfigMateriel_t;

typedef struct 
{
    uint16_t redLedDuration;
    uint16_t greenLedDuration;
    uint16_t blueLedDuration;        
    uint16_t builtinLedDuration;
    uint16_t wakeupIntervalPayload;
    uint16_t interval1Sec;
    uint8_t reserve[8];
} nonConfigApplicatif_t;


typedef struct   
{
  uint8_t rucher_ID;  // 0:non affecté, 1: Fleurs, 2: Acacia, 3: Tilleul, 4: Chataignier
                      // 5: Sapin 6: Jachère 7: Forêt  8: Phacélie 9: Pommes 
  char    RucherName [20];      // Localisation Rucher (saisir direct ou liste
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight[4];    // masse 4 Ruche  en kg xxx,xx (précision affich. 10g)
  float   ProcessorTemp;    // temp µC, conservé en backup DHT22 
} LoRa_Var;   // Data_LoRa. devient Payload

// Config  Générale
typedef struct     // regroupe tous les paramètres de EEPROM
{
  uint8_t Balance_ID;           // ID Rucher           xx  uint8_t
  char    RucherName [20];      // Localisation Rucher (saisir direct ou liste + "autre")
} ConfigBalanceSW;

/*
// Config Hardware
typedef struct     // regroupe tous les paramètres de EEPROM
{
  uint8_t HX711Clk_0;           // HX711#0 parameters
  uint8_t HX711Dta_0;
  float   HX711ZeroValue_0;
  float   HX711Scaling_0;
  float   HX711Cor_Temp_0;
  uint8_t HX711Clk_1;           // HX711#1 parameters
  uint8_t HX711Dta_1;
  float   HX711ZeroValue_1;
  float   HX711Scaling_1;
  float   HX711Cor_Temp_1;
  uint8_t HX711Clk_2;           // HX711#2 parameters
  uint8_t HX711Dta_2;
  float   HX711ZeroValue_2;
  float   HX711Scaling_2;
  float   HX711Cor_Temp_2;
  uint8_t HX711Clk_3;           // HX711#3 parameters
  uint8_t HX711Dta_3;
  float   HX711ZeroValue_3;
  float   HX711Scaling_3;
  float   HX711Cor_Temp_3;
  float   LDRBrightnessScale;   // 
  float   VSolScale;            //  
  float   VBatScale;
} ConfigBalanceHW;
*/

// Définition structure: ConfigMateriel_t
typedef struct 
{
  uint16_t version;       // version matérielle : 3 = PCB2
  uint8_t adresseRTC;     // DS3231_ADDRESS 0x68
  uint8_t adresseOLED;    //
  uint8_t adresseEEPROM;  // EEPROM_ADDRESS 0x57

  uint8_t Num_Carte;  // Numéro de carte fct° de SN RN2482
  uint8_t DevEUI[9];         // ID RN2483: 0x0004A30B00EEEE01
  uint8_t Rtc;        // True or False, autodetect?
  uint8_t KBD_Ana;    // True or False
  uint8_t Oled;       // True or False OLED096 ou OLED130
  uint8_t SDHC;       // True or False
  uint8_t LiPo;       // True or False
  uint8_t Solaire;    // True or False Abandonné?

// HX711#0 parameters    
  uint8_t Peson_0;              //  N° Peson
  uint8_t HX711Clk_0;           
  uint8_t HX711Dta_0;
  float   HX711NoloadValue_0;   // valeur sans charge (relevée par opérateur )
  float   HX711Tare_Temp_0;     // Température lors de la tare (mesurée lors de la tare)  
  float   HX711Scaling_0;       // Facteur de mise à l'echelle
  float   HX711Cor_Temp_0;      // Facteur de compensation en température
// HX711#1 parameters  
  uint8_t Peson_1;              //  N° Peson 
  uint8_t HX711Clk_1;           
  uint8_t HX711Dta_1;
  float   HX711NoloadValue_1;
  float   HX711Tare_Temp_1;
  float   HX711Scaling_1;
  float   HX711Cor_Temp_1;
// HX711#2 parameters
  uint8_t Peson_2;              //  N° Peson
  uint8_t HX711Clk_2;           
  uint8_t HX711Dta_2;
  float   HX711NoloadValue_2;
  float   HX711Tare_Temp_2;  
  float   HX711Scaling_2;
  float   HX711Cor_Temp_2;
// HX711#3 parameters
  uint8_t Peson_3;              //  N° Peson
  uint8_t HX711Clk_3;           
  uint8_t HX711Dta_3;
  float   HX711NoloadValue_3;
  float   HX711Tare_Temp_3;  
  float   HX711Scaling_3;
  float   HX711Cor_Temp_3;
// Analog scaling  
  float   LDRBrightnessScale;   // 
  float   VSolScale;            //  
  float   VBatScale;
} ConfigMateriel_t;


// Définition structure: ConfigApplicatif_t;
typedef struct 
{
    uint16_t version;  // version logicielle
// paramètres cosmétiques    
    uint16_t redLedDuration;      // RED_LED_DURATION 100
    uint16_t greenLedDuration;    // GREEN_LED_DURATION 100
    uint16_t blueLedDuration;     // BLUE_LED_DURATION 100   
    uint16_t builtinLedDuration;  // BUILTIN_LED_DURATION 100  
// paramètres Rucher
  uint8_t RucherID;         // ID Rucher           xx  uint8_t
  char    RucherName [20];    // Localisation Rucher (saisir direct ou liste + "autre") 
// paramètres LoRa
  uint8_t AppEUI [10];        // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00}
  uint8_t AppKey [18];        // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
// {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} 
  uint8_t SpreadingFactor;    // 7, 9 et 12 echec freudeneck
  uint8_t SendingPeriod;      // WAKEUP_INTERVAL_PAYLOAD 5
  uint8_t OLEDRefreshPeriod;  // INTERVAL_1SEC 1000 
} ConfigApplicatif_t;

typedef struct 
{
    ConfigMateriel_t materiel;
    ConfigApplicatif_t applicatif;
    uint16_t checksum;
} ConfigGenerale_t;



// exemples struct et union
// dans Mesures.cpp
struct nombre
{
  unsigned entier : 1;
  unsigned flottant : 1;
  union
  {
    int e;
    double f;
  } u;
};
