#ifdef __MAIN__

// ===== FLAGS DEBUG =====
bool DEBUG_WAKEUP_PAYLOAD = true;    // Activer/désactiver réveil payload
bool DEBUG_INTERVAL_1SEC = false;     // Activer/désactiver réveil 1 seconde
bool DEBUG_LOW_POWER = false;         // Activer/désactiver basse consommation
bool OLED = true;                    // Activer/désactiver OLED

// ===== VARIABLES GLOBALES =====
RTC_DS3231 rtc;
//DS3231 Clock;

// variables clavier
clavier_context_t clavierContext = {KEY_NONE, KEY_NONE, 0, 0, false};



#ifdef OLED096
  Adafruit_SSD1306 display(OLED_RESET);
#else
  Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

char OLEDbuf[21] = "12345678901234567890";
char serialbuf[SERIALBUFLEN];
bool debugdrawtext = false;

// Variables pour gestion des interruptions
volatile bool wakeupPayload = false;
volatile bool wakeup1Sec = false;
volatile bool modeExploitation = true;
int switchToProgrammingMode = true;
int switchToOperationMode = true;

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

// Variable RN2483A

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
HW_equipement Ruche;
LoRa_configuration LoRa_Config = {9,WAKEUP_INTERVAL_PAYLOAD};      /// => sous IT !!!!!
LoRa_Var Data_LoRa;

float Contrainte_List [4] = {
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999,  // pas de balance, Affiche "N/A"
         -999   // pas de balance, Affiche "N/A"
       };





/*
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
         0.0032252,  0.0032252,  0.0032252,  0.0032252,  0.0032252
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

// ****************************************************************************
// initilize Weight sensor connection pins
// ****************************************************************************
// pesée HX711
HX711 scale;    // parameter "gain" is ommited; the default value 128 is used 
int HX711_NbLect = 10;//float calibration_factor = 7050; //-7050 worked for my 440lb max scale setup
//float poidsmax, poidsmin, totare;
//float GetgramVal;
*/


#else


#ifdef OLED096
  extern Adafruit_SSD1306 display; //(OLED_RESET);
#else
  extern Adafruit_SH1106G display; // = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

extern clavier_context_t clavierContext;

extern char OLEDbuf[];
extern char serialBuf[];
extern char Module_ID[];

extern uint8_t payloadSize; 
extern uint8_t payload[];
extern int hexPayloadSize;
extern char hexPayload[];
extern uint8_t testPayload[];
//extern 
extern char HWEUI_List [][20];
extern uint8_t SN2483_List [][9];
extern uint8_t AppEUI_List [][9];
extern uint8_t AppKey_List [][17];
extern uint8_t *DevEUI;    // Orange : kit SodaQ RUCHE 0: 00 04 A3 0B 00 20 30 0A
extern uint8_t *AppEUI;    // Orange : kit SodaQ RUCHE 0 
extern uint8_t *AppKey;    // Orange : kit SodaQ RUCHE 0 



// Variable définitions RUCHE
extern HW_equipement Ruche;
extern LoRa_configuration LoRa_Config; 
extern LoRa_Var Data_LoRa;
extern float Contrainte_List [];


#endif
