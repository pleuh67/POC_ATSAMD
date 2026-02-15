//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
//      _____        __ _              _    
//     |  __ \      / _(_)            | |   
//     | |  | | ___| |_ _ _ __   ___  | |__ 
//     | |  | |/ _ \  _| | '_ \ / _ \ | '_ \
//     | |__| |  __/ | | | | | |  __/_| | | |
//     |_____/ \___|_| |_|_| |_|\___(_)_| |_|
//                                          
// ---------------------------------------------------------------------------*
// ===== INCLUDES =====
#include <Wire.h>
#include <RTClib.h>
#include <ArduinoLowPower.h>
#include <stdlib.h>   // pour atoi()

#define NIBBLE_TO_HEX_CHAR(i) ((i <= 9) ? ('0' + i) : ('A' - 10 + i))
#define HIGH_NIBBLE(i) ((i >> 4) & 0x0F)
#define LOW_NIBBLE(i) (i & 0x0F)

// ===== CONSTANTES PROJET =====
#define PROJECT_NAME "POC IRQ_Payload IRQ_1s LOW_POWER OLED RN2483 DHT22 KEY5"  // len = 55
#define VERSION "1.1.1-PL"


#define LOG_ERROR(msg)   debugSerial.print("[ERROR] "); debugSerial.println(msg)
#define LOG_WARNING(msg) debugSerial.print("[WARN]  "); debugSerial.println(msg)
#define LOG_INFO(msg)    debugSerial.print("[INFO]  "); debugSerial.println(msg)
#define LOG_DEBUG(msg)   debugSerial.print("[DEBUG] "); debugSerial.println(msg)



#define NB_RUCHES 4 // 3 Protos Atsamd, Fixe à 4 pesées

// ===== TIMING CONSTANTS =====
#define RED_LED_DURATION        100   // Durée d'allumage LED rouge 300 ms
#define GREEN_LED_DURATION      100   // Durée d'allumage LED verte 300 ms
#define BLUE_LED_DURATION       100   // Durée d'allumage LED bleue 300 ms
#define BUILTIN_LED_DURATION    100   // Durée d'allumage LED builtin 100 ms
#define WAKEUP_INTERVAL_PAYLOAD 5     // Intervalle de réveil en minutes 
#define INTERVAL_1SEC           1000  // Intervalle 1 seconde en ms
#define DEFAULT_SF              12    // Spread Factor par defaut
#define TIMEOUT_SAISIE          20000    // Timeout saisies écrans (ms)
//#define REFRESHSCREEN           1000

// I2C Addresses
#define DS3231_ADDRESS 0x68   // Adresse RTC Module DS3231
#define EEPROM_ADDRESS 0x57   // Adresse EEPROM Module DS3231
#define OLED_ADDRESS   0x3C   // Adresse écran OLED

// ===== Def de LISTES =====
#define LIST_SF       4
#define LIST_RUCHERS  12

// ===== MENU CONFIGURATION =====
#define MAX_MENU_DEPTH 5
// nombre d'options pour le menu
#define M0_ITEM   5 // Menu Demarrage
#define M01_ITEM  7 // Menu config. Système
#define M02_ITEM  8 // Menu config. LoRa
#define M03_ITEM  5 // menu Calb. Tensions
#define M033_ITEM 5
#define M04_ITEM  7 // Menu Calib. Balances
#define M04x_ITEM 5

// ===== EEPROM CONFIGURATION =====
#define CONFIG_VERSION 111   // Version 1.1.1
// ===== ADRESSE EEPROM CONFIGURATION =====
#define CONFIG_EEPROM_START 0x0000  // Adresse de début en EEPROM
#define CONFIG_MAGIC_NUMBER 0xFF03  // Nombre magique pour valider config

// defines pour raccourcir et clarifier les instructions
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// noms courts et explicites, parametrer (num: 0..3) par Macro
#define poidsBal_g(num)   HiveSensor_Data.HX711Weight[num]         // g 
#define poidsBal_kg(num)  abs((Contrainte_List[num]-pesonTare(num))/pesonScale(num)) // kg

#define pesonTare(num)    Jauge[Peson[config.materiel.Num_Carte][num]][0]
#define pesonScale(num)   Jauge[Peson[config.materiel.Num_Carte][num]][1]
#define TareTemp(num)     Jauge[Peson[config.materiel.Num_Carte][num]][2]  
#define CompTemp(num)     Jauge[Peson[config.materiel.Num_Carte][num]][3]
#define pesonNum(num)     Peson[config.materiel.Num_Carte][num]

#define EXPLORER

// Affectations Ports ANA
#define LUM_SENSOR A0     // ANALOG_LDR_PIN              
#define VBAT_MEASURE A1     // VBat/Pont diviseur  3,9 V donne 2,45 environ
#define VSOL_MEASURE A2     // VBat/Pont diviseur  3,9 V donne 2,45 environ
#define KBD_ANA     A3
//#define A4_VPIN   A4        // I2C SDA pour LCD/RTC/EEPROM et ATECC508A (CRYPTO) sur connecteur GROVE EXT
//#define A5_VPIN   A5        // I2C SCL pour LCD/RTC/EEPROM et ATECC508A (CRYPTO) sur connecteur GROVE EXT
//#define A6_VPIN   A6        // NC
//#define A7_VPIN   A7        // sur connecteur GROVE INT
//#define A8_VPIN   A8        // sur connecteur GROVE INT

// Affectations Ports NUM/PWM

// ---------------------------------------------------------------------------*
// --- constantes des broches ---
// ---------------------------------------------------------------------------*
// I2C PINs USED for LCD, RTC,
#define SDA  PIN_WIRE_SDA    // Défini dans variant.h
#define SCL  PIN_WIRE_SCL    // Défini dans variant.h


#define PIN_PE BUTTON
#define RTC_INTERRUPT_PIN 2

#define TARE_BUTTON   9
// #define BUZZER     7 // non implanté


#define LIBRE0  49    // PB09
#define LIBRE1  50    // PA18 
#define LIBRE2  51    // PA19 sur SD_CS
#define LIBRE3  52    // PB22
#define LIBRE4  53    // PB23
#define LIBRE5  54    // PA27
#define LIBRE6  55    // PA28 sur RD_VBAT
#define LIBRE7  56    // PB12

#define SD_CS   LIBRE2    // PA19, testé OK
#define RD_VBAT LIBRE6    // PA28


#define TEMP_ERR 99   // Mesures en erreur

// ===== PINS ET INTERFACES =====
#define loraSerial     Serial2 
#define debugSerial    SerialUSB
#define SERIALBUFLEN   256
#define OLEDBUFLEN     128 * 21
#define DEBUG_BAUD     115200
#define SERIAL_TIMEOUT 5000

// ===== LED RGB CONFIGURATION =====
#define LED_OFF HIGH     // État pour éteindre les LEDs RGB (logique inverse)
#define LED_ON  LOW       // État pour allumer les LEDs RGB (logique inverse)

// ===== CLAVIER ANALOGIQUE =====
#define NB_KEYS           5
#define TOL               20      // Tolérance pour la détection
#define DEBOUNCE_COUNT    5       // Nombre de lectures identiques requises
#define DEBOUNCE_DELAY_MS 1       // Délai entre les lectures

// Alias pour les touches selon usage
#define MOINS     KEY_3   
#define PLUS      KEY_2
#define LEFT      KEY_1    
#define RIGHT     KEY_4
#define VALIDE    KEY_5
#define UP        KEY_2      
#define DOWN      KEY_3    
#define ANNULER_4 KEY_4   


// ===== OLED CONFIGURATION =====
//#define OLED096  // Sélection du type d'écran
#define OLED130  // Sélection du type d'écran
//#define OLED154  // Sélection du type d'écran
//#define OLED242  // Sélection du type d'écran

// Pour OLED selon type sélectionné
#ifdef OLED096                            // SSD1306
  #include <Adafruit_SSD1306.h>
#else                                     // OLED130, OLED154?, OLED242?
  #include <Adafruit_SH110X.h>
#endif

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      4
#define MAX_LIGNES      8  
#define TAILLE_LIGNE    8
#define pixelLine       57 // ligne 8

#ifdef OLED096
  #define OLEDTEXTSIZE  1
  #define OLED_Col      6
  #define OLED_ColOffset  0
  #define OLED_Max_Col  20
  #define OLED_L1       8
#else
  #define OLEDTEXTSIZE  1
  #define OLED_Col      6
  #define OLED_Col_Offset  0 // 2     // pour 2.42 pouces
  #define OLED_Max_Col  20
  #define OLED_L1       8
  #define WHITE         SH110X_WHITE
  #define BLACK         SH110X_BLACK
#endif


// RN2483A
#define PAYLOADSIZE     19
#define HEXPAYLOADSIZE  38
#define MAX_HWEUI_List 6 


// ---------------------------------------------------------------------------*
// LoRa 
// ---------------------------------------------------------------------------*
#include <Sodaq_RN2483.h>   // et pas OrangeForRN2483
#include <Sodaq_wdt.h>
//#define LORA_BAUD 57600   >> loraSerial.begin(LoRaBee.getDefaultBaudRate());
#define LORA_Start_SESSION      // à activer hors phase de developpement.
#define __SendLoRaInOperationMode
#define __SendLoRaInProgrammationMode

// ---------------------------------------------------------------------------*
// cayenne pour tests gateway
// ---------------------------------------------------------------------------*
#include <CayenneLPP.h>

// ---------------------------------------------------------------------------*
// HX711 Weight sensors - weastone bridge
// HX711 Calibration Guide : https://github.com/bogde/HX711
// ---------------------------------------------------------------------------*
#include "HX711.h"              // inclusion de la librairie HX711
#define HX711_SENSOR_SCK    3
#define HX711_ASENSOR_DOUT  4   // pull up sur Dout
#define HX711_BSENSOR_DOUT  6
#define HX711_CSENSOR_DOUT  8
#define HX711_DSENSOR_DOUT  10
#define debugSerialGetStrainGaugeAverage  // decommenter pour les messages debugSerial
#define TARE  1071         //  56200 // Bloc de 56.2 kg
#define AVR_1 1     // loop() ttes 10 Secondes
#define AVR_3 3     // EXPLOITATION au reveil pour TX LoRa
#define AVR_10 10   // SETUP()

// ---------------------------------------------------------------------------*
// DHTxx Temperature and Humidity sensors            DTHT22 CONFIGURATION ====*
// ---------------------------------------------------------------------------*
#include "DHT.h"
#define DHT_TYPE      DHT22   // DHT 22  (AM2302), AM2321
#define DHT_SENSOR    5       // Temp/Hum
#define DHT_T_ERR     99      // Mesures en erreur
#define DHT_H_ERR     99      // Mesures en erreur


#include ".\Struct.h"
#include ".\Var.h"
#include ".\Prototypes.h"
