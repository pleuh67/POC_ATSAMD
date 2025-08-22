// ===== INCLUDES =====
#include <Wire.h>
#include <RTClib.h>
//#include <DS3231.h>
#include <ArduinoLowPower.h>


// defines pour raccourcir et clarifier les instructions
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// noms courts et explicites, parametrer (num: 0..3) par Macro
#define Poids_Peson(num)      Data_LoRa.HX711Weight[num]   //  Data_LoRa de type LoRa_Var (ligne 38)
#define Temp_Peson(num)       Data_LoRa.Temp_Peson[num]
#define Tare_Peson(num)       Jauge[Peson[Ruche.Num_Carte][num]][0]
#define Echelle_Peson(num)    Jauge[Peson[Ruche.Num_Carte][num]][1]
//#define BalPoids(num)  (Contrainte_List[num]-Jauge[Peson[Ruche.Num_Carte][num]][0])/Jauge[Peson[Ruche.Num_Carte][num]][1]/1000) //retourne float
#define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float

// ?? attention step(num) entre TareTemp et la correespondance
//         1..4                    0..3
#define TareTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][2]  // Ruche de type HW_equipement (ligne 21)
#define CompTemp(num)   Jauge[Peson[Ruche.Num_Carte][num]][3]
// passer de 0..3 dans l'appelant: fait


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

// ****************************************************************************
// --- constantes des broches ---
// ****************************************************************************
// I2C PINs USED for LCD, RTC,
#define SDA  PIN_WIRE_SDA    // Défini dans variant.h
#define SCL  PIN_WIRE_SCL    // Défini dans variant.h


#define PIN_PE BUTTON
#define RTC_INTERRUPT_PIN 2
#define HX711_SENSOR_SCK    3
#define HX711_ASENSOR_DOUT  4       // pull up sur Dout
#define DHT_SENSOR          5       // Temp/Hum
#define HX711_BSENSOR_DOUT  6
#define HX711_CSENSOR_DOUT  8
#define TARE_BUTTON         9
#define HX711_DSENSOR_DOUT  10
// #define BUZZER     7 // non implanté




// Pour OLED selon type sélectionné
#ifdef OLED096
  #include <Adafruit_SSD1306.h>
#else
  #include <Adafruit_SH110X.h>
#endif

// ===== CONSTANTES PROJET =====
#define PROJECT_NAME "POC IRQ LOW_POWER OLED KEY5 SAISIES EEPROM RN2483"
#define VERSION "1.1.1-PL"


#define BUFF_MAX 256
#define BUF_LEN 128  


// ===== PINS ET INTERFACES =====
#define loraSerial Serial2 
#define debugSerial SerialUSB
#define SERIALBUFLEN 128
#define DEBUG_BAUD 115200
#define SERIAL_TIMEOUT 5000

// ===== TIMING CONSTANTS =====
#define RED_LED_DURATION 5000     // Durée d'allumage LED rouge en millisecondes (5 secondes)
#define BUILTIN_LED_DURATION 100  // Durée d'allumage LED builtin en millisecondes
#define WAKEUP_INTERVAL_PAYLOAD 5 // Intervalle de réveil en minutes (2 minute pour test)
#define INTERVAL_1SEC 10000        // Intervalle 1 seconde en millisecondes

// ===== LED RGB CONFIGURATION =====
#define LED_OFF HIGH     // État pour éteindre les LEDs RGB (logique inverse)
#define LED_ON  LOW       // État pour allumer les LEDs RGB (logique inverse)

// ===== CLAVIER ANALOGIQUE =====
#define NB_KEYS 5
#define TOL 20                    // Tolérance pour la détection
#define DEBOUNCE_COUNT 5          // Nombre de lectures identiques requises
#define DEBOUNCE_DELAY_MS 1       // Délai entre les lectures

// ===== OLED CONFIGURATION =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS   0x3C
#define OLED_RESET 4
#define MAX_LIGNES     8  
#define TAILLE_LIGNE   8
#define pixelLine 57 // ligne 8

#define OLED130  // Sélection du type d'écran

#ifdef OLED096
  #define OLEDTEXTSIZE  1
  #define OLED_Col  6
  #define OLED_Max_Col 20
  #define OLED_L1 8
#else
  #define OLEDTEXTSIZE  1
  #define OLED_Col  6
  #define OLED_Max_Col 20
  #define OLED_L1 8
  #define WHITE SH110X_WHITE
  #define BLACK SH110X_BLACK
#endif

// ===== EEPROM CONFIGURATION =====
#define EEPROM_ADDRESS 0x57  // Adresse EEPROM du DS3231
#define CONFIG_VERSION 100   // Version 1.00 * 100

// Alias pour les touches selon usage
#define MOINS KEY_3 //1
#define PLUS KEY_2
#define LEFT KEY_1    //3
#define RIGHT KEY_4
#define VALIDE KEY_5
#define UP KEY_2    //3
#define DOWN KEY_3    //4
#define ANNULER_4 KEY_4   //

// RN2483A
#define PAYLOADSIZE 27
#define HEXPAYLOADSIZE 60



#include ".\struct.h"
#include ".\var.h"
#include ".\prototypes.h"


// ****************************************************************************
// LoRa
// ****************************************************************************
#include <Sodaq_RN2483.h>   // et pas OrangeForRN2483
#include <Sodaq_wdt.h>
//#define LORA_BAUD 57600  // >>>   loraSerial.begin(LoRaBee.getDefaultBaudRate());
#define LORA_Start_SESSION      // à activer hors phase de developpement.

// ****************************************************************************
// cayenne pour tests gateway
// ****************************************************************************
#include <CayenneLPP.h>
