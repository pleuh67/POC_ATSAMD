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





// Variables pour readKey() non-bloquant
typedef struct {
    key_code_t derniereTouche;
    key_code_t toucheStable;
    int compteStable;
    unsigned long derniereLecture;
    bool toucheDisponible;
} clavier_context_t;


// ===== STRUCTURES DE CONFIGURATION =====
typedef struct 
{
    uint16_t version;
    uint8_t adresseRTC;
    uint8_t adresseOLED;
    uint8_t adresseEEPROM;
    uint8_t reserve[5];
} ConfigMateriel_t;

typedef struct 
{
    uint16_t redLedDuration;
    uint16_t greenLedDuration;
    uint16_t blueLedDuration;        
    uint16_t builtinLedDuration;
    uint16_t wakeupIntervalPayload;
    uint16_t interval1Sec;
    uint8_t reserve[8];
} ConfigApplicatif_t;

typedef struct 
{
    ConfigMateriel_t materiel;
    ConfigApplicatif_t applicatif;
    uint16_t checksum;
} ConfigGenerale_t;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// redefinir toutes les structures dès que compile !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




// prefixer _no pour paramètres à éliminer (tranformer en dummy sur LoRa)
typedef struct 
{
  uint8_t Num_Carte;  // Numéro de carte
  uint8_t MediaCOM;       // True
  //uint8_t _noBlueTooth;  //False
  uint8_t Rtc;        // True or False, autodetect?
  uint8_t KBD_Ana;    // True or False
  uint8_t Oled;       // True or False
  uint8_t SDHC;       // True or False
  uint8_t LiPo;       // True or False
  uint8_t Solaire;    // True or False Aabandonné?
  uint8_t Balance[4];   // True or False or N° Peson
  uint8_t DS18B20[4][8];   // uint8_t XH711;      // True or False, autodetect
}  HW_equipement;






typedef struct   
{
  uint8_t rucher_ID;  // 0:non affecté, 1: Fleurs, 2: Acacia, 3: Tilleul, 4: Chataignier
                      // 5: Sapin 6: Jachère 7: Forêt  8: Phacélie 9: Pommes 
  float   DHT_Temp;         // Temp DHT en °C    xx,x  Float
  float   DHT_Hum;          // Hum DHT en %      xx,x  Float
  float   Brightness;       // %Lum en LUX      xxxxx  uint16_t
  float   Bat_Voltage;      // Tension BAT en V     xx,xx  Float (uint16_t)
  float   Solar_Voltage;    // Tension BAT en V     xx,xx  Float (uint16_t)
  float   HX711Weight[4];    // masse Ruche 1 en kg xxx,xx (précision affich. 10g)
  float   Temp_Peson[4];     // température peson 1 en °C xx,xx  Float
  float   VSol[11];
  float   VBat[11];   
  float   _noLux;              // temp µC, ne sera pas conservé 
  float   _noProcessorTemp;    // temp µC, ne sera pas conservé 
} LoRa_Var;

typedef struct  
{
  uint8_t SpreadingFactor;   // 7, 9 et 12 echec freudeneck
  uint8_t SendingPeriod;        // 15 minutes = 500 sans IT
} LoRa_configuration;

// Config LoRa
typedef struct 
{
  char AppEUI[11];    // Format "jj/mm/aaaa"
  char heure[9];    // Format "hh:mm:ss"
  char sendPayloadInterval;
  char spreadFactor;
} LoRaState;


typedef struct     // regroupe tous les paramètres de EEPROM
{
  uint8_t Balance_ID;           // ID Rucher           xx  uint8_t
  char    HWEUI_List [20];      // RN2483 HWEUI
  uint8_t HX711Clk_0;           // HX711#0 parameters
  uint8_t HX711Dta_0;
  float   HX711ZeroValue_0;
  float   HX711Scaling_0;
//  float   HX711Cor_Temp_0;
  uint8_t HX711Clk_1;           // HX711#1 parameters
  uint8_t HX711Dta_1;
  float   HX711ZeroValue_1;
  float   HX711Scaling_1;
//  float   HX711Cor_Temp_1;
  uint8_t HX711Clk_2;           // HX711#2 parameters
  uint8_t HX711Dta_2;
  float   HX711ZeroValue_2;
  float   HX711Scaling_2;
//  float   HX711Cor_Temp_2;
  uint8_t HX711Clk_3;           // HX711#3 parameters
  uint8_t HX711Dta_3;
  float   HX711ZeroValue_3;
  float   HX711Scaling_3;
//  float   HX711Cor_Temp_3;
  float   LDRBrightnessScale;   // 
  float   VSolScale;            //  Luxmetre abandonné
  float   VBatScale;
// DS18B20 ???  
} ConfigBalance;
