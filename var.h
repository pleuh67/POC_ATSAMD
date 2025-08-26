#ifdef __MAIN__

// ===== FLAGS DEBUG =====
bool DEBUG_WAKEUP_PAYLOAD = true;    // Activer/désactiver réveil payload
//bool DEBUG_WAKEUP_PAYLOAD = false;    // Activer/désactiver réveil payload

bool DEBUG_INTERVAL_1SEC = true;     // Activer/désactiver réveil 1 seconde
//bool DEBUG_INTERVAL_1SEC = false;     // Activer/désactiver réveil 1 seconde

//bool DEBUG_LOW_POWER = true;         // Activer/désactiver basse consommation
bool DEBUG_LOW_POWER = false;         // Activer/désactiver basse consommation
bool OLED = true;                    // Activer/désactiver OLED

// ===== VARIABLES GLOBALES =====
RTC_DS3231 rtc;

// variables clavier
clavier_context_t clavierContext = {KEY_NONE, KEY_NONE, 0, 0, false};



#ifdef OLED096
  Adafruit_SSD1306 display(OLED_RESET);
#else
  Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

char OLEDbuf[21] = "12345678901234567890";
char serialbuf[SERIALBUFLEN];
bool debugOLEDDrawText = false;

// Variables pour gestion des interruptions
volatile bool wakeupPayload = false;
volatile bool wakeup1Sec = false;
volatile bool modeExploitation = true;
int switchToProgrammingMode = true;
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
bool modeDebugActif = true;
unsigned long delaiAffichage = 1000;
char lignesDebug[MAX_LIGNES][21];

// Variables pour saisies
unsigned long lastBlink = 0;
bool blinkState = true;

// Variables config
ConfigGenerale_t config;

// Variables debug
bool COM_DebugSerial = true;

// Variable RN2483A et LoRa

uint8_t payloadSize = PAYLOADSIZE; 
uint8_t payload[PAYLOADSIZE];

int hexPayloadSize = HEXPAYLOADSIZE; // 45 characters counting from 0 + 2 for termination. 23 bytes is required by Kineis. Padding needs to be done if the payload is smaller.
char hexPayload[HEXPAYLOADSIZE];

// char hexPayload[PAYLOADSIZE * 2 + 1];
// int hexPayloadSize = PAYLOADSIZE * 2 + 1;

uint8_t *DevEUI;    // Orange : kit SodaQ RUCHE 0: 00 04 A3 0B 00 20 30 0A
uint8_t *AppEUI;    // Orange : kit SodaQ RUCHE 0 
uint8_t *AppKey;    // Orange : kit SodaQ RUCHE 0 

// liste des ID LoRa 
//  "414245494C4C4534", // Orange:HELTEC Wireless Stick => autre projet ESP32S3
char Module_ID[20] = "55AA55AA55AA55AA";               // A LIRE DANS MODULE
char HWEUI_List [6][20] = {
  "55AA55AA55AA55AA", // Module LoRa pas Lu
  "0004A30B0020300A", // Orange: Carte Explorer HS, puis proto PCB#1, récupérer Chip LoRa pour PCB#2
  "0004A30B0024BF45", // pas connecté chez Orange??? noeud non identifié
  "0004A30B00EEEE01", // Orange: carte Explorer, PCB#1 puis PCB#2 Support Loess
  "0004A30B00EEA5D5", // noeud non identifié
  "0004A30B00F547CF" // Orange: Proto CAVE PCB#2
 };

uint8_t SN2483_List [6][9] = {   // donne le DevEUI (Module_ID)
  {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // Module pas Lu
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0x20, 0x30, 0x0A, 0x00 },  // Proto PCB#1-1 RN à récuperer
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0x24, 0xBF, 0x45, 0x00 },  // SodaQ RUCHE 1 
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xEE, 0xEE, 0x01, 0x00 },  // Proto PCB#2-2 Ruches LOESS
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xEE, 0xA5, 0xD5, 0x00 },  // Proto PCB#2-3 Ruches Verger
  {  0x00, 0x04, 0xA3, 0x0B, 0x00, 0xF5, 0x47, 0xCF, 0x00 }   // Proto PCB#2-1 Cave
};

uint8_t AppEUI_List [6][9] ={ 
  {0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4B, 0x4F, 0x00},     // Module pas Lu
  {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00},     // PROTO SODAQ 1
  {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x32, 0x00},     // PROTO SODAQ 2
  {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x33, 0x00},     // PROTO PCB01
//  {0x70, 0xB3, 0xD5, 0x49, 0x96, 0xB9, 0x8D, 0x3C, 0x00},     // PROTO TTN
  {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x34, 0x00},
  {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x35, 0x00}     // ABEILLE5 - Verger
                                   };  // Orange : liste des AppEUI
                                    
//const 
uint8_t AppKey_List [56][17] = {  // 5048494C495050454C4F56454C414B4F != PHILIPPELOVEBEES
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4F, 0x4F, 0x00},  // 0x4F, 0x4F = 00 Pb
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4B, 0x4F, 0x00},  // 0x4B, 0x4F = 50 RN reconnu
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4B, 0x4F, 0x00},
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4B, 0x4F, 0x00},
//  {0x35, 0xBC, 0xE7, 0xA5, 0x7A, 0x4E, 0xCC, 0x04, 0xB3, 0xA4, 0x96, 0xF4, 0xCC, 0xFC, 0x0D, 0xB3, 0x00},  // proto TTN
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x4C, 0x41, 0x4B, 0x4F, 0x00},
  {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
                               };      // Orange : liste des AppKEY 

uint8_t testPayload[8] = 
           { 0x52, 0x65, 0x73, 0x74, 0x61, 0x72, 0x74}; // Restart

// Variable définitions RUCHE

// jauges de contrainte de J01 à J15
float Jauge[21][4] = {                // Tare , Echelle , TareTemp , CompTemp
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
//      {128885,20.43,5.1,0},    // J15SFX proto1 SLC 200kg (OK à 5 et 50kg)
//      {125777,25.42,3.2,0},    // J15SFX proto1 SLC 200kg (OK à 5 et 50kg)
//      {65931,21.59,20,0} J15 première tare ???(-12 mois)

      {123199.65,103.59,20,1},    // J16 proto1  20kg (OK à 1 et 5kg)
      {123199.65,103.59,20,2},    // J17 a refaire
//      {123199.65,103.59,20,3},    // J18 proto1  20kg (OK à 1 et 5kg)
      {30804.50,103.77,20,0},    // J18 proto1  20kg (OK à 1 et 5kg)
//      {123199.65,103.59,20,0}     // J19 proto1  20kg (OK à 1 et 5kg)

     {22005.70,97.49,20,0},    // J18 proto1  20kg (OK à 1 et 5kg)
     {22005.70,97.49,20,0},    // J19
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
      {19,17,0,0},    // 0004A30B00EEEE01 Carte PROTO1 mis en service Loess le 08/03/2021
      {0,5,0,0},    // complter
      {5,0,0,0},
      {6,0,0,0},
      {7,0,0,0},
      {8,0,0,0},
      {9,0,0,0}  
    };

HW_equipement Ruche;
LoRa_configuration LoRa_Config = {9,WAKEUP_INTERVAL_PAYLOAD};      /// => sous IT !!!!!
LoRa_Var Data_LoRa;

float Contrainte_List [4] = {
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999   // pas de balance, Affiche "N/A"
       };


byte DS18B20[20][8]={
            {0x28, 0xFF, 0xDF, 0xE4, 0x64, 0x15, 0x01, 0x48}, // jauge n°:
            {0x28, 0xFF, 0xF4, 0xAA, 0x64, 0x15, 0x03, 0x89}, // jauge n°:
            {0x28, 0xFF, 0xB9, 0xBD, 0x64, 0x15, 0x03, 0x32}, // jauge n°:
            {0x28, 0xFF, 0x2D, 0xF6, 0x64, 0x15, 0x03, 0xEE}, // jauge n°:
            {0,0,0,0,0,0,0,0}, // jauge n°:
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
};



// DS1820
OneWire  ds(DS1820);  // on pin PA18 (a 4.7K pull up resistor is necessary)

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

// Variables pour gestion des interruptions
extern volatile bool wakeupPayload;
extern volatile bool wakeup1Sec;
extern volatile bool modeExploitation;
extern int switchToProgrammingMode;
extern int switchToOperationMode;

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
extern bool modeDebugActif;
extern unsigned long delaiAffichage;
extern char lignesDebug[][21];

// Variables pour saisies
extern unsigned long lastBlink;
extern bool blinkState;


extern bool debugOLEDDrawText;

extern RTC_DS3231 rtc;

#ifdef OLED096
  extern Adafruit_SSD1306 display; //(OLED_RESET);
#else
  extern Adafruit_SH1106G display; // = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

extern clavier_context_t clavierContext;

extern char OLEDbuf[];
extern char serialbuf[];
extern char Module_ID[];

extern uint8_t payloadSize; 
extern uint8_t payload[];
extern int hexPayloadSize;
extern char hexPayload[];
extern uint8_t testPayload[]; 
extern char HWEUI_List [][20];
extern uint8_t SN2483_List [][9];
extern uint8_t AppEUI_List [][9];
extern uint8_t AppKey_List [][17];
extern uint8_t *DevEUI;    // Orange : kit SodaQ RUCHE 0: 00 04 A3 0B 00 20 30 0A
extern uint8_t *AppEUI;    // Orange : kit SodaQ RUCHE 0 
extern uint8_t *AppKey;    // Orange : kit SodaQ RUCHE 0 

// Variables config
extern ConfigGenerale_t config;

// Variables debug
extern bool COM_DebugSerial;

// Variable définitions RUCHE
// jauges de contrainte de J01 à J15
extern float Jauge[][4];           // Tare , Echelle , TareTemp , CompTemp
// paramètres et données des dispositif de pesée A,B,C,D
// Clk_PIN, Dta_PIN, Poids
extern int balance [][2];
extern int Peson [][4];
extern HW_equipement Ruche;
extern LoRa_configuration LoRa_Config; 
extern LoRa_Var Data_LoRa;
extern float Contrainte_List [];

// DS1820
extern byte DS18B20[][8];
extern OneWire  ds;  // on pin PA18 (a 4.7K pull up resistor is necessary)

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
