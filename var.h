//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//     __      __        _    
//     \ \    / /       | |   
//      \ \  / /_ _ _ __| |__ 
//       \ \/ / _` | '__| '_ \
//        \  / (_| | |_ | | | |
//         \/ \__,_|_(_)|_| |_|
//
// ---------------------------------------------------------------------------*
#include ".\Menu.h"
#ifdef __MAIN__

// pas de const dans les globales => erreur ce compile !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Cas typiques nécessitant volatile

// Variables modifiées par interruptions : controler rtc,nextPayload passé volatile pour test compile pas
// Variables liées au hardware (registres) : pas modifiés par moi (lib)
// Variables partagées entre threads : pas de threads
// Flags de communication ISR ↔ loop() : controler wakeup1Sec, alarm1_enabled, alarm2_enabled, wakeupPayload

// ===== FLAGS DEBUG =====
//bool DEBUG_INTERVAL_1SEC = true;       // Activer IRQ1 réveil 1 seconde
bool DEBUG_INTERVAL_1SEC = false;     // désactiver IRQ1 réveil 1 seconde

bool DEBUG_WAKEUP_PAYLOAD = true;      // Activer IRQ2 réveil payload
//bool DEBUG_WAKEUP_PAYLOAD = false;    // désactiver IRQ2 réveil payload

//bool DEBUG_LOW_POWER = true;         // Activer/désactiver basse consommation
bool DEBUG_LOW_POWER = false;        // Activer/désactiver basse consommation

unsigned long loopWDT = 0;


// ===== VARIABLES GLOBALES =====
RTC_DS3231 rtc;
DateTime nextPayload;

// variables clavier
clavier_context_t clavierContext = {KEY_NONE, KEY_NONE, 0, 0, false};
key_code_t touche; 

// ===== VARIABLES GLOBALES MACHINE A ETAT MENUS=====
menuLevel_t menuStack[MAX_MENU_DEPTH];
uint8_t currentMenuDepth = 0;


// ===== VARIABLES GLOBALES MACHINE A ETAT SAISIES=====
char saisieActive=0;

// Contexte de Listes et menus
bool startupListActivated = false;    // Flag pour activer la liste au démarrage
char stringSaisie[OLEDBUFLEN]="azerty";   // 128/retour de toutes les fonctions de saisie non bloquantes
bool displayListDebug = false;
listInputContext_t listInputCtx = {LIST_INPUT_IDLE, 0, 0, 10, 0, 0, false, false, 0, false, 0, 0, 0, "", NULL};

// Contexte de saisie Numérique
numInputContext_t numInputCtx = {NUM_INPUT_IDLE, 0, 0xFF, 0, 0xFF, 10, "", "", false, 0, false, false, 0, 0, 0xFF, 0xFF, 16, 0, TIMEOUT_SAISIE, false, false, 0, 9999, true, 0xFF, ""};
bool displayNumberDebug = false;

// Contexte de saisie Alphanumérique
stringInputContext_t stringInputCtx = {STRING_INPUT_IDLE, 0, 0xFF, 20, "", "", false, 0, false, false, 0, 0, TIMEOUT_SAISIE, true, 0xFF, ""};
bool displayStringDebug = false;

// Contexte de saisie HEXA
//hexInputContext_t hexInputCtx = {HEX_INPUT_IDLE, 0, 0xFF, "", "", false, 0, false, false, 0, 0, 0xFF, 0xFF, 16, 0, TIMEOUT_SAISIE, false, true, 0xFF, ""};
hexInputContext_t hexInputCtx = {HEX_INPUT_IDLE, 0, 0xFF, 40, "", "", "", false, 0, false, false, 0, 0, 0xFF, 0xFF, 16, 0, TIMEOUT_SAISIE, false, true, 0xFF};

// Contexte de saisie d'heure
timeInputContext_t timeInputCtx = {TIME_INPUT_IDLE, 0, 0xFF, "", "", false, 0, false, false, 0, 0, TIMEOUT_SAISIE, false, true, ""};

// Contexte de saisie de date
dateInputContext_t dateInputCtx = {DATE_INPUT_IDLE, 0, 0xFF, "", "", false, 0, false, false, 0, 0, TIMEOUT_SAISIE, false, true, ""};

// Contexte de saisie de l'Email
char emailCharSet[] = "abcdefghijklmnopqrstuvwxyz0123456789@.-_"; // Jeu de caractères pour email
uint8_t emailCharSetSize = 40; // 26 lettres + 10 chiffres + 4 symboles
emailInputContext_t emailInputCtx = {EMAIL_INPUT_IDLE, 0, 0xFF, 0, 0xFF, "", "", false, 0, false, false, 0, 0, 0xFF, 0xFF, 16, 0, TIMEOUT_SAISIE, false, 0xFF, true, 0, 0xFF, ""};

// Contexte de saisie de l'IP
ipInputContext_t ipInputCtx = {IP_INPUT_IDLE, 0, 0xFF, "192.168.001.001", "", false, 0, false, false, 0, 0, TIMEOUT_SAISIE, false, true, ""};

// État de l'écran d'information
infoScreenState_t infoScreenState = INFO_SCREEN_IDLE;


bool ScreenRefreshed = false;  // info generale de maj ecran info pour desactiver dans Handle.cpp

// Système
bool InfoScreenRefreshTime = false;         // rafraichissement heure ecran Info

// LoRa
bool LoRaScreenRefreshTime = false;         // rafraichissement heure ecran LoRa
bool LoraScreenRefreshNextPayload = false;
// Balances
bool InfoBalScreenRefreshTime = false;      // rafraichissement heure ecran Balances
bool WeightScreenRefreshTime = false;
bool WeightScreenRefreshWeights = false;
// Analogiques
bool InfoVBatScreenRefreshTime = false;     // rafraichissement heure ecran VBAT
bool InfoVBatScreenRefresh = false;         // rafraichissement mesures ecran VBAT
bool InfoVLumScreenRefreshTime = false;     // rafraichissement heure ecran VLUM
bool InfoVLumScreenRefresh= false;          // rafraichissement mesures ecran VLUM
bool InfoVSolScreenRefreshTime = false;     // rafraichissement heure ecran VSol
bool InfoVSolScreenRefresh = false;         // rafraichissement mesures ecran VSol
// 

//bool PvageInfosLoRaRefresh = false;  // utile?
//bool PvageInfosSystRefresh = false;  // utile?
//bool displayInfoScreenSystTimeRefresh = false;

#ifdef OLED096
  Adafruit_SSD1306 display(OLED_RESET);
#else
  Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

char OLEDbuf[OLEDBUFLEN] = "12345678901234567890";  // 128
char serialbuf[SERIALBUFLEN];                       // 256
bool debugOLEDDrawText = false;

// Variables pour gestion des interruptions
volatile bool alarm1_enabled = true;
volatile bool alarm2_enabled = true;
volatile bool wakeupPayload = false;
volatile bool displayNextPayload = false;
volatile bool wakeup1Sec = false;
volatile bool modeExploitation = true;
int switchToProgrammingMode= true;
int switchToOperationMode = true;

// gestion des leds non bloquantes
volatile bool redLedActive = false;           // LED rouge en cours
volatile unsigned long redLedStartTime = 0;   // Moment du démarrage
volatile bool greenLedActive = false;           // LED rouge en cours
volatile unsigned long greenLedStartTime = 0;   // Moment du démarrage
volatile bool blueLedActive = false;           // LED rouge en cours
volatile unsigned long blueLedStartTime = 0;   // Moment du démarrage
volatile bool builtinLedActive = false;         // LED builtin en cours  
volatile unsigned long builtinLedStartTime = 0; // Moment du démarrage


// Variables pour OLED scrolling
bool OLEDModeDebugActif = true;   // par defaut defilement messages au setup();
unsigned long delaiAffichage = 1000;
char lignesDebug[MAX_LIGNES][21];

// Variables pour saisies
unsigned long lastBlink = 0;
bool blinkState = true;

// Variables config
bool setupDone = false;
ConfigGenerale_t config;
//config.materiel_t config.materiel;
//config.applicatif_t config.applicatif;
HiveSensor_Data_t HiveSensor_Data;  // nommer en payload !!!! existe: uint8_t payload[PAYLOADSIZE];




// Variables debug
bool COM_DebugSerial = true;

// Variable RN2483A et LoRa

uint8_t payloadSize = PAYLOADSIZE; // 27
uint8_t payload[PAYLOADSIZE];

int hexPayloadSize = HEXPAYLOADSIZE; // 45 characters counting from 0 + 2 for termination. 23 bytes is required by Kineis. Padding needs to be done if the payload is smaller.
char hexPayload[HEXPAYLOADSIZE];

// liste des ID LoRa 
//  "414245494C4C4534", // Orange:HELTEC Wireless Stick => autre projet ESP32S3

// #define MAX_HWEUI_List a maintenir dans define.h 
char HWEUI_List [MAX_HWEUI_List][21] = {                      // renommer en DeviceID_List
  "55AA55AA55AA55AA", // Module LoRa pas Lu
  "0004A30B0020300A", // Orange: Carte Explorer HS, puis proto PCB#1, récupérer Chip LoRa pour PCB#2
  "0004A30B0024BF45", // pas connecté chez Orange??? noeud non identifié
  "0004A30B00EEEE01", // Orange: carte Explorer, PCB#1 puis PCB#2 Support Loess
  "0004A30B00EEA5D5", // Orange: Proto PCB#2 Support Loess
  "0004A30B00F547CF" // Orange: Proto CAVE PCB#2
 };

uint8_t SN2483_List [6][9] = {   // donne le DevEUI (Module_ID)   // renommer en HexDeviceID_List
  {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // Module pas Lu
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0x20, 0x30, 0x0A, 0x00},  // Proto PCB#1-1 RN à récuperer
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0x24, 0xBF, 0x45, 0x00 },  // SodaQ RUCHE 1 
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xEE, 0xEE, 0x01, 0x00 },  // Proto PCB#2-2 Ruches LOESS
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xEE, 0xA5, 0xD5, 0x00 },  // Proto PCB#2-3 Ruches Verger
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xF5, 0x47, 0xCF, 0x00 }   // Proto PCB#2-1 Cave
};



// AppEUI (Application EUI, aussi appelé JoinEUI dans LoRaWAN 1.1+)
// Identifie l'application, pas le device individuel
// Peut être partagé par plusieurs devices de la même application
// Format : 64 bits (8 octets)
// Sur Orange Live Objects, vous pouvez utiliser le même AppEUI pour tous vos devices d'un même projet
uint8_t AppEUI_List[9] ={ 0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x53, 0x00}; //  ABEILLE3     0xAF},     // ABEILLES

// AppKey (Application Key)
// Doit être unique par device pour des raisons de sécurité
// C'est la clé secrète utilisée pour l'activation OTAA
// Format : 128 bits (16 octets)
// Ne doit jamais être réutilisée entre plusieurs devices
//  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
//                                                                      HONEYWEIGHT
uint8_t AppKey_List [6][17] = {  // 5048494C495050454C4F56454C414B4F != PHILIPPELOVEBEES
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00 },  // Module pas Lu
//   H     O     N     E     Y     W     E     I     G     H     T  (D)E     V     E     U     i
  { 0x48, 0x4F, 0x4E, 0x45, 0x59, 0x57, 0x45, 0x49, 0x47, 0x48, 0x54, 0x32, 0x30, 0x33, 0x30, 0x41 , 0x00},  // Proto PCB#1-1 RN à récuperer 300A
  { 0x48, 0x4F, 0x4E, 0x45, 0x59, 0x57, 0x45, 0x49, 0x47, 0x48, 0x54, 0x0B, 0x00, 0x24, 0xBF, 0x45 , 0x00},  // SodaQ RUCHE 1 BF45 HS?
   { 0x48, 0x4F, 0x4E, 0x45, 0x59, 0x57, 0x45, 0x49, 0x47, 0x48, 0x54, 0x0B, 0x00, 0xEE, 0xEE, 0x01, 0x00},  // Proto PCB#2-2 Ruches LOESSNEW EE01
   { 0x48, 0x4F, 0x4E, 0x45, 0x59, 0x57, 0x45, 0x49, 0x47, 0x48, 0x54, 0x0B, 0x00, 0xEE, 0xA5, 0xD5, 0x00},  // Proto PCB#2-3 Ruches Verger NEW A5D5  
   { 0x48, 0x4F, 0x4E, 0x45, 0x59, 0x57, 0x45, 0x49, 0x47, 0x48, 0x54, 0x0B, 0x00, 0xF5, 0x47, 0xCF, 0x00},  // Proto PCB#2-1 Cave NEW 47CF
 //  {0x35, 0xBC, 0xE7, 0xA5, 0x7A, 0x4E, 0xCC, 0x04, 0xB3, 0xA4, 0x96, 0xF4, 0xCC, 0xFC, 0x0D, 0xB3, 0x00},  // proto TTN
                               };      // Orange : liste des AppKEY 

uint8_t testPayload[7] = 
           { 0x52, 0x65, 0x73, 0x74, 0x61, 0x72, 0x74}; // Restart

// Variable définitions RUCHE

// jauges de contrainte de J01 à J15
int bal = 0;      // indiquer le contenu, Num de proto identifié par init lora & HWEUI_List ???????????????????????
float Jauge[22][4] = {                // Tare , Echelle , TareTemp , CompTemp
      {0,0,0,0},     // J00 => pas de peson connecté
      {178666,108.5,20,0},    // J01 20kg
      {30250,21.2,20,0},      // J02
      {21000,22000,20,0},     // J03 évolution valeurs en négatif. tester sur bornier
      {31000,32000,20,0},     // J04
      {41000,42000,20,0},     // J05
      {68971,19.56,19.7,0},     // J06  BAL_A  200kg  le 19/03/2021
      {61000,62000,20,0},     // J07MS 200kg
      {64003,21.19,20,0},     // J08SL proto1 SLB 200kg (OK à 5 et 50kg)
      {140680,19.39,20,0},    // J09MS proto1 SL 200kg (OK à 5 et 50kg)
      {374942,1145.58,20,0},  // J10 2kg
      {4798647,1053.71,20,0}, // J11 2kg
      {179568,1056.40,20,0},  // J12 2kg
      {20369,19.93,20,-2},   // J13MS proto1 Master 200kg (53kg ok 5kg => 3.12!!!)
      {53753,21.425,17.1,-1},    // J14 warré 200kg
      {139983,20.46,17.1,0.048341},    // J15SFX proto1 SLC 200kg (OK à 5 et 50kg)
      {123199.65,103.59,20,1},    // J16 proto1  20kg (OK à 1 et 5kg)
      {123199.65,103.59,20,2},    // J17 a refaire
      {34134.50,103.77,20,0},    // J18 proto1  20kg (OK à 1 et 5kg)
      {7929.70,97.49,20,0},    // J19 proto1  20kg (OK à 1 et 5kg) + DHT22
      {22005.70,97.49,20,0},    // J20
      {1,2,20,3}                // J21
    };

// paramètres et données des dispositif de pesée A,B,C,D
// Clk_PIN, Dta_PIN, Poids
int balance [4][2] = {    // défini en DUR!!!!!!!!!!!!!
      {HX711_ASENSOR_DOUT, HX711_SENSOR_SCK},   // PoidsA = -999}, // -999, Affiche "N/A"
      {HX711_BSENSOR_DOUT, HX711_SENSOR_SCK},   // PoidsB = -999}, // -999, Affiche "N/A"
      {HX711_CSENSOR_DOUT, HX711_SENSOR_SCK},   // PoidsC = -999}, // -999, Affiche "N/A"
      {HX711_DSENSOR_DOUT, HX711_SENSOR_SCK}    // PoidsD = -999}  // -999, Affiche "N/A"
    };


// N° des jauge montée sur le dispositif de pesée A,B,C,D
int Peson [10][4] = {
      {0,0,0,0},    // Module LoRa pas Lu; pas de Peson
      {0,0,0,17},    // 0004A30B0020300A carte 1 HS; sur Carte PROTO2 en service le 05/03/2021
      {13,8,9,0}, //15},    // 0004A30B0024BF45 carte 2; en service le 10/05/2020
      {0,16,0,0},    // 0004A30B00EEEE01 Carte PROTO1 mis en service Loess le 08/03/2021
      {0,18,0,0},    // 0004A30B00EEA5D5
      {19,21,14,17},  // 0004A30B00F547C
      {6,0,0,0},
      {7,0,0,0},
      {8,0,0,0},
      {9,0,0,0}  
    };


// Structures de données des configurations



// maintenir table de valeurs de correction pesons
/*
typedef struct 
{
  uint16_t version;       // version matérielle : 3 = PCB2
  uint8_t adresseRTC;     // DS3231_ADDRESS 0x68
  uint8_t adresseOLED;    //
  uint8_t adresseEEPROM;  // EEPROM_ADDRESS 0x57

  uint8_t Num_Carte;  // Numéro de carte 
  uint8_t Noeud_LoRa;
  uint8_t Rtc;        // True or False, autodetect?
  uint8_t KBD_Ana;    // True or False
  uint8_t Oled;       // True or False OLED096 ou OLED130
  uint8_t SDHC;       // True or False
  uint8_t LiPo;       // True or False
  uint8_t Solaire;    // True or False Aabandonné?

// HX711#0 parameters    
  uint8_t Peson_0; //  N° Peson
  uint8_t HX711Clk_0;           
  uint8_t HX711Dta_0;
  float   HX711ZeroValue_00;
  float   HX711Scaling_0;
  float   HX711Cor_Temp_0;
// HX711#1 parameters  
  uint8_t Peson_1; //  N° Peson
  uint8_t HX711Clk_1;           
  uint8_t HX711Dta_1;
  float   HX711ZeroValue_1;
  float   HX711Scaling_1;
  float   HX711Cor_Temp_1;
// HX711#2 parameters
  uint8_t Peson_2; //  N° Peson
  uint8_t HX711Clk_2;           
  uint8_t HX711Dta_2;
  float   HX711ZeroValue_2;
  float   HX711Scaling_2;
  float   HX711Cor_Temp_2;
// HX711#3 parameters
  uint8_t Peson_3; //  N° Peson
  uint8_t HX711Clk_3;           
  uint8_t HX711Dta_3;
  float   HX711ZeroValue_3;
  float   HX711Scaling_3;
  float   HX711Cor_Temp_3;
// Analog scaling  
  float   LDRBrightnessScale;   // 
  float   VSolScale;            //  
  float   VBatScale;
} config.materiel_t;
*/

// n'est ce pas en doublon avec Data_LoRa.HX711Weight[num]
float Contrainte_List [4] = {
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999   // pas de balance, Affiche "N/A"
       };


// DHT22 => 3V3/DHT_SENSOR/GND
DHT dht(DHT_SENSOR, DHT_TYPE);

// lectures ANA tension Batterie
float VBat[11] = {0,0,0,0,0,0,0,0,0,0,0}; // 10 dernières lectures + moyenne
float VBatScale_List [10] = {
        0.0032252,    // Module LoRa pas Lu
        0.0032252, // masse/6k2/mesure/3k3/VSOL
        0.0032252,   //
        0.0032252,   0.0032252,  
        0.003222,  0.0032252,  0.0032252,  0.0032252,  0.0032252
      };

// lectures ANA tension panneau solaire
float VSol[11] = {0,0,0,0,0,0,0,0,0,0,0}; // 10 dernières lectures + moyenne
float VSolScale_List [10] = {
        0.0032555, // Module LoRa pas Lu
        0.0032555, // masse/5k6/mesure/3k3/VSOL
        0.0032555, //
        0.0032555,  0.0032555, 
        0.0032555,  0.0032555, 0.0032555, 0.0032555,  0.0032555
      };

// lectures ANA LDR
String readingL;
float VLumScale_List [10] = {
        1,  // Module LoRa pas Lu
        1, 1, 1, 1, 1, 1, 1, 1, 1
      };

// lectures Température interne µC
String readingT;

// pesée HX711
HX711 scale;    // parameter "gain" is ommited; the default value 128 is used 
int HX711_NbLect = 10;//float calibration_factor = 7050; //-7050 worked for my 440lb max scale setup

/* 
//float poidsmax, poidsmin, totare;
//float GetgramVal;
*/

#else


// ===== FLAGS DEBUG =====
extern bool DEBUG_WAKEUP_PAYLOAD;    // Activer/désactiver réveil payload
extern bool DEBUG_INTERVAL_1SEC;     // Activer/désactiver réveil 1 seconde
extern bool DEBUG_LOW_POWER;         // Activer/désactiver basse consommation
extern bool OLED;                    // Activer/désactiver OLED

extern unsigned long loopWDT;

// Variables pour gestion des interruptions
extern volatile bool alarm1_enabled;
extern volatile bool alarm2_enabled;
extern volatile bool wakeupPayload;
extern volatile bool displayNextPayload;
extern volatile bool wakeup1Sec;
extern volatile bool modeExploitation;
extern int switchToProgrammingMode;
extern int switchToOperationMode;

// variables clavier
extern clavier_context_t clavierContext;
extern key_code_t touche;


// gestion des leds non bloquantes
extern volatile bool redLedActive;           // LED rouge en cours
extern volatile unsigned long redLedStartTime;   // Moment du démarrage
extern volatile bool greenLedActive;           // LED rouge en cours
extern volatile unsigned long greenLedStartTime;   // Moment du démarrage
extern volatile bool blueLedActive;           // LED rouge en cours
extern volatile unsigned long blueLedStartTime;   // Moment du démarrage
extern volatile bool builtinLedActive;         // LED builtin en cours  
extern volatile unsigned long builtinLedStartTime; // Moment du démarrage


// Variables pour OLED scrolling
extern bool OLEDModeDebugActif;
extern unsigned long delaiAffichage;
extern char lignesDebug[][21];

// Variables pour saisies
extern unsigned long lastBlink;
extern bool blinkState;


extern bool debugOLEDDrawText;

extern RTC_DS3231 rtc;
extern DateTime nextPayload;

// ===== VARIABLES GLOBALES MACHINE A ETAT MENUS=====
extern menuLevel_t menuStack[];
extern uint8_t currentMenuDepth;

// ===== VARIABLES GLOBALES MACHINE A ETAT SAISIES=====
extern char saisieActive;
extern bool startupListActivated;
extern char *stringSaisie;
extern listInputContext_t listInputCtx;
extern bool displayListDebug;
//extern numberInputContext_t numberInputCtx;
extern numInputContext_t numInputCtx;
extern bool displayNumberDebug;
extern stringInputContext_t stringInputCtx;
extern bool displayStringDebug;
extern hexInputContext_t hexInputCtx;
extern timeInputContext_t timeInputCtx;
// Contexte de saisie de date
extern dateInputContext_t dateInputCtx;    

// Contexte de saisie de l'Email
extern const char *emailCharSet; //[];  // /*const*/
extern const uint8_t emailCharSetSize; // 26 lettres + 10 chiffres + 4 symboles  /*const*/
extern emailInputContext_t emailInputCtx;
// Contexte de saisie de l'IP
extern ipInputContext_t ipInputCtx;

// État de l'écran d'information
extern infoScreenState_t infoScreenState;

extern bool PvageInfosLoRaRefresh;
extern bool PvageInfosSystRefresh;

extern bool ScreenRefreshed;  // info generale de maj ecran info pour desactiver dans Handle.cpp

// Système
extern bool InfoScreenRefreshTime;

// LoRa
extern bool LoRaScreenRefreshTime;
extern bool LoraScreenRefreshNextPayload;
// Balances
extern bool InfoBalScreenRefreshTime;
extern bool WeightScreenRefreshTime;
extern bool WeightScreenRefreshWeights;
// Analogiques
extern bool InfoVBatScreenRefreshTime;
extern bool InfoVBatScreenRefresh;
extern bool InfoVLumScreenRefreshTime;
extern bool InfoVLumScreenRefresh;
extern bool InfoVSolScreenRefreshTime;
extern bool InfoVSolScreenRefresh;

//bool displayInfoScreenSystTimeRefresh = false;

#ifdef OLED096
  extern Adafruit_SSD1306 display; //(OLED_RESET);
#else
  extern Adafruit_SH1106G display; // = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

extern clavier_context_t clavierContext;

extern char *OLEDbuf; //[];
extern char serialbuf[];

//extern char Module_ID_HWEUI[];   

extern uint8_t payloadSize; 
extern uint8_t payload[];
extern int hexPayloadSize;
extern char hexPayload[];
extern uint8_t testPayload[]; 
extern char HWEUI_List [][21];
extern uint8_t SN2483_List [][9];
extern uint8_t AppEUI_List [9];
extern uint8_t AppKey_List [][17];


// Variables config
extern bool setupDone;
// Structures de données des configurations
extern ConfigGenerale_t config;
//extern config.materiel_t config.materiel;
//extern config.applicatif_t config.applicatif;
extern HiveSensor_Data_t HiveSensor_Data;







// Variables debug
extern bool COM_DebugSerial;

// Variable définitions RUCHE
// jauges de contrainte de J01 à J15
extern int bal;   // Numéro de balance selectionnée dans m04_CalibBalances[]
extern float Jauge[][4];           // Tare , Echelle , TareTemp , CompTemp
// paramètres et données des dispositif de pesée A,B,C,D
// Clk_PIN, Dta_PIN, Poids
extern int balance [][2];
extern int Peson [][4];


extern float Contrainte_List [];

// DHT22 => 3V3/DHT_SENSOR/GND
extern DHT dht;

// lectures ANA tension Batterie
extern float VBat[]; // 10 dernières lectures + moyenne
extern float VBatScale_List[];

// lectures ANA tension panneau solaire
extern float VSol[]; // 10 dernières lectures + moyenne
extern float VSolScale_List[];

// lectures ANA LDR
extern String readingL;
extern float VLumScale_List [];

// lectures Température interne µC
extern String readingT;

// Lecture du poids 
extern HX711 scale;
extern int HX711_NbLect;

#endif
