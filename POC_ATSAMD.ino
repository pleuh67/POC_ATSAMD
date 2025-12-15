//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*
// https://claude.ai/chat/75e1ba53-8b90-4f09-97a5-cfc366e36a8e
// ---------------------------------------------------------------------------*
//      _____                     _                           _   _            
//     |  __ \               /\  | |                         | | (_)           
//     | |__) |__   ___     /  \ | |_ ___  __ _ _ __ ___   __| |  _ _ __   ___ 
//     |  ___/ _ \ / __|   / /\ \| __/ __|/ _` | '_ ` _ \ / _` | | | '_ \ / _ \
//     | |  | (_) | (__   / ____ \ |_\__ \ (_| | | | | | | (_| |_| | | | | (_) |
//     |_|   \___/ \___| /_/    \_\__|___/\__,_|_| |_| |_|\__,_(_)_|_| |_|\___/
//                   ______                                                    
//                  |______|                                                   
// ---------------------------------------------------------------------------*
// https://patorjk.com/software/taag/#p=display&f=Big&t=&x=cppComment&v=4&h=4&w=80&we=true
// Ascii Art Font : BIG
// C++ style comment
// ---------------------------------------------------------------------------*
//
// @file POC_ATSAMD.ino
// @brief Programme de démonstration pour SODAQ EXPLORER avec gestion basse consommation
// @author Votre nom
// @version 1.1.1-PL
// @date 2025
// 
// Description : Gestion de deux modes de fonctionnement avec interruptions RTC
// - MODE EXPLOITATION : réveil périodique pour LED rouge
// - MODE PROGRAMMATION : réveil périodique + clignotement LED builtin + interface utilisateur
// ---------------------------------------------------------------------------*

#define __MAIN__
// ===== INCLUDES PROJET =====
#include ".\define.h"

#define __SendLoRa
//#define __SerialDebugPoc      // decommenter pour afficher messages debug

// =====  PROGRAMME =====
// ===== SETUP =====
void setup() 
{  int z=0;

// INITIALISE LA STRUCTURE A ENVOYER
Data_LoRa.rucher_ID = 67;   // compris entre 0 et 99
sprintf(Data_LoRa.RucherName,"LPCZ");
Data_LoRa.DHT_Temp = 25;
Data_LoRa.DHT_Hum = 69;
Data_LoRa.Brightness = 91;
Data_LoRa.Bat_Voltage = 3.69;
Data_LoRa.Solar_Voltage = 4.69;
Data_LoRa.HX711Weight[0] = 12.34; // 
Data_LoRa.HX711Weight[1] = 23.45;
Data_LoRa.HX711Weight[2] = 34.56;
Data_LoRa.HX711Weight[3] = 45.67;
Data_LoRa.ProcessorTemp = 21.18;   // temp µC, ne sera pas conservé 

  
  initDebugSerial(); 
  // Initialisation des LEDs
  initLEDs();
    
  // Initialisation du mode selon PIN_PE
  pinMode(PIN_PE, INPUT_PULLUP);
  modeExploitation = digitalRead(PIN_PE);
    
  debugSerial.print("Mode détecté: ");
  debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
   
  // Initialisation I2C
  Wire.begin();
    
  // Initialisation OLED
  OLEDInit();
  OLEDDebugDisplay("OLEDInit OK"); // scrolling lors du setup(); 
  debugSerial.println("OLEDInit OK");
 
  // Initialisation configuration
  initConfig();
  OLEDDebugDisplay("initConfig OK");
delay(1000);



// initialiser les Structures, lecture EEPROM I2C@0x57,  si echec val par défaut
// Si echec tester autre adresse I2C si existe
// initialiser par lecture EEPROM I2C

// initialisation aux valeurs par défaut
debugSerial.println("Initialisation aux valeurs par défaut");


  ConfigMateriel.version = 3;       // version matérielle : 3 = PCB2
  ConfigMateriel.adresseRTC = DS3231_ADDRESS;     
  ConfigMateriel.adresseOLED = OLED_ADDRESS;   
  ConfigMateriel.adresseEEPROM = EEPROM_ADDRESS;  



  
  ConfigMateriel.Num_Carte = init2483A(ConfigMateriel.DevEUI);
debugSerial.print("ConfigMateriel.Num_Carte <= "); debugSerial.println(ConfigMateriel.Num_Carte);  // OK

//  ConfigMateriel.HWEUI [20] = {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // "0004A30B00EEEE01";         // ID RN2483: "0004A30B00EEEE01"
  
  
  
  ConfigMateriel.Rtc = true;        // True or False, autodetect?
  ConfigMateriel.KBD_Ana = true;    // True or False
  ConfigMateriel.Oled = 96;       // True or False OLED096 ou OLED130
  ConfigMateriel.SDHC = false;       // True or False
  ConfigMateriel.LiPo = true;       // True or False
  ConfigMateriel.Solaire = true;    // True or False Abandonné?

// /!\ la carte doit être identifiée => de ConfigMateriel.Num_Carte /!\
// #0
  ConfigMateriel.Peson_0 = Peson[ConfigMateriel.Num_Carte][0];  //  N° Peson
//                          N° de peson : num ruche        0 .. 3      
  ConfigMateriel.HX711Clk_0 = HX711_SENSOR_SCK;           
  ConfigMateriel.HX711Dta_0 = HX711_ASENSOR_DOUT;
// les données sont extraites de la base de données des pesons
  ConfigMateriel.HX711NoloadValue_0 = Jauge[ConfigMateriel.Peson_0][0];
  ConfigMateriel.HX711Tare_Temp_0 = Jauge[ConfigMateriel.Peson_0][2];
  ConfigMateriel.HX711Scaling_0 = Jauge[ConfigMateriel.Peson_0][1];
  ConfigMateriel.HX711Cor_Temp_0 = Jauge[ConfigMateriel.Peson_0][3];
// #1
  ConfigMateriel.Peson_1 = Peson[ConfigMateriel.Num_Carte][1];  //  N° Peson
  ConfigMateriel.HX711Clk_1 = HX711_SENSOR_SCK;           
  ConfigMateriel.HX711Dta_1 = HX711_BSENSOR_DOUT;
  ConfigMateriel.HX711NoloadValue_1 = Jauge[ConfigMateriel.Peson_1][0];
  ConfigMateriel.HX711Tare_Temp_1 = Jauge[ConfigMateriel.Peson_1][2];
  ConfigMateriel.HX711Scaling_1 = Jauge[ConfigMateriel.Peson_1][1];
  ConfigMateriel.HX711Cor_Temp_1 = Jauge[ConfigMateriel.Peson_1][3];
// #2
  ConfigMateriel.Peson_2 = Peson[ConfigMateriel.Num_Carte][2];  //  N° Peson
  ConfigMateriel.HX711Clk_2 = HX711_SENSOR_SCK;           
  ConfigMateriel.HX711Dta_2 = HX711_CSENSOR_DOUT;
  ConfigMateriel.HX711NoloadValue_2 = Jauge[ConfigMateriel.Peson_2][0];
  ConfigMateriel.HX711Tare_Temp_2 = Jauge[ConfigMateriel.Peson_2][2];
  ConfigMateriel.HX711Scaling_2 = Jauge[ConfigMateriel.Peson_2][1];
  ConfigMateriel.HX711Cor_Temp_2 = Jauge[ConfigMateriel.Peson_2][3];
// #3
  ConfigMateriel.Peson_3 = Peson[ConfigMateriel.Num_Carte][3];  //  N° Peson
  ConfigMateriel.HX711Clk_3 = HX711_SENSOR_SCK;           
  ConfigMateriel.HX711Dta_3 = HX711_DSENSOR_DOUT;
  ConfigMateriel.HX711NoloadValue_3 = Jauge[ConfigMateriel.Peson_3][0];
  ConfigMateriel.HX711Tare_Temp_3 = Jauge[ConfigMateriel.Peson_3][2];
  ConfigMateriel.HX711Scaling_3 = Jauge[ConfigMateriel.Peson_3][1];
  ConfigMateriel.HX711Cor_Temp_3 = Jauge[ConfigMateriel.Peson_3][3];

// Analog scaling  
  ConfigMateriel.LDRBrightnessScale;   // 
  ConfigMateriel.VSolScale;            //  
  ConfigMateriel.VBatScale;
// FIN initialisation aux valeurs par défaut

  sprintf(OLEDbuf,"ID: %s",ConfigMateriel.DevEUI);
  OLEDDebugDisplay(OLEDbuf);


// ConfigApplicatif: initialisation aux valeurs par défaut
  ConfigApplicatif.version = 1000101;  // version logicielle
// paramètres cosmétiques  
  ConfigApplicatif.redLedDuration = RED_LED_DURATION;      //  100
  ConfigApplicatif.greenLedDuration = GREEN_LED_DURATION;    //  100
  ConfigApplicatif.blueLedDuration = BLUE_LED_DURATION;     //  100   
  ConfigApplicatif.builtinLedDuration = BUILTIN_LED_DURATION;  //  100  
// paramètres Rucher
  ConfigApplicatif.RucherID = 91;         // ID Rucher           xx  uint8_t
  strcpy(ConfigApplicatif.RucherName, "FREUDENECK");    // Localisation Rucher (saisir direct ou liste + "autre") 
// paramètres LoRa
// HX :  uint8_t HWEUI [20] = "0004A30B00EEEE01";         // ID RN2483: "0004A30B00EEEE01"

  uint8_t AppEUI[] = {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00};
  memcpy(ConfigApplicatif.AppEUI, AppEUI, sizeof(AppEUI));
//  ConfigApplicatif.AppEUI /*[10]*/ = {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00};      

  uint8_t AppKey[] = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00};   
  memcpy(ConfigApplicatif.AppKey, AppKey, sizeof(AppKey)); 
//  ConfigApplicatif.AppKey /*[18]*/ = {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00};      
  ConfigApplicatif.SpreadingFactor = DEFAULT_SF;    // 7, 9 et 12 echec freudeneck
  ConfigApplicatif.SendingPeriod = WAKEUP_INTERVAL_PAYLOAD;      // WAKEUP_INTERVAL_PAYLOAD 5
  ConfigApplicatif.OLEDRefreshPeriod = INTERVAL_1SEC;  // INTERVAL_1SEC 1000 




/*

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
  uint8_t HWEUI [20];         // ID RN2483: "0004A30B00EEEE01"
  uint8_t AppEUI [10];        // AppEUI: {0x41, 0x42, 0x45, 0x49, 0x4C, 0x4C, 0x45, 0x31, 0x00}
  uint8_t AppKey [18];        // AppKEY: // 5048494C495050454C4F564542454553 - PHILIPPELOVEBEES
// {0x50, 0x48, 0x49, 0x4C, 0x49, 0x50, 0x50, 0x45, 0x4C, 0x4F, 0x56, 0x45, 0x42, 0x45, 0x45, 0x53, 0x00} 
  uint8_t SpreadingFactor;    // 7, 9 et 12 echec freudeneck
  uint8_t SendingPeriod;      // WAKEUP_INTERVAL_PAYLOAD 5
  uint8_t OLEDRefreshPeriod;  // INTERVAL_1SEC 1000 
} ConfigApplicatif_t;

  */




  

// config balance connue 10 lecture + moyenne
// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025

  if (Peson[ConfigMateriel.Num_Carte][z])
  {
    Poids_Peson(z) = GetPoids(z+1,10);
    sprintf(OLEDbuf,"getWeight %d Done",z);
    OLEDDebugDisplay(OLEDbuf);
  }
  else 
  {
    sprintf(OLEDbuf,"Peson %d: NONE ",z);
   OLEDDebugDisplay(OLEDbuf);
  }


    
 
  float temp = ((analogRead(TEMP_SENSOR) * 3300.0 / 1023.0) - 500.0) / 10.0; // Lecture temp µC  
            // put the ADC in sleep mode
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// temp : utiliser DHT22 si existe sinon temperature interne µC
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Rappel:
// #define Poids_Peson(num)      Data_LoRa.HX711Weight[num]   //  Data_LoRa de type LoRa_Var (ligne 38)
// #define Tare_Peson(num)       Jauge[Peson[ConfigMateriel.Num_Carte][num]][0]
// #define Echelle_Peson(num)    Jauge[Peson[ConfigMateriel.Num_Carte][num]][1]
// #define BalPoids(num) (Contrainte_List[num]-Tare_Peson(num))/Echelle_Peson(num)/1000 //retourne float
// float GetPoids(int num)  // ?????   
// lecture brute => poids en gr
int num = 0;
float pesonTare  = Jauge[Peson[ConfigMateriel.Num_Carte][num]][0];  // correction pour avoir valeur balance sans charge.
float pesonScale = Jauge[Peson[ConfigMateriel.Num_Carte][num]][1];  // Mise à l'Echelle de pesonValue vers peson Poids
float pesonValue = Poids_Peson(num) - pesonTare;           // valeur à convertir
float pesonPoids = pesonValue / pesonScale;                // poids correspondant
float pesee;                                               // 
  //  Poids_List [num] = pesee*(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  Contrainte_List [num] = pesee; // *(1-Jauge[Peson[carte][num]][3]*(Jauge[Peson[carte][num]][2]/temp));
  // (peson-tare)/echelle
  pesee = BalPoids(num);   // ( pesee - Jauge[Peson[ConfigMateriel.Num_Carte][num]][0] ) / Jauge[Peson[Ruche.Num_Carte][num]][1] / 1000; 
  sprintf(serialbuf,"temp %5.2f, lu %5.2f, tare %5.2f, echelle %5.2f,  tare en moins %5.2f, poids %5.2f ",
          temp,Poids_Peson(num) , pesonTare, pesonScale,  pesonValue, pesonPoids  );
  debugSerial.println(serialbuf);
/*
15:20:10.162 ->  7892  7936  7884  7852  7785  7839  7999  8066  8076  7969  M 7929 
15:20:10.162 -> temp 28.39, lu 7929.00, tare 22005.70, echelle 97.49,  tare en moins -14076.70, poids -144.39 
*/


// poids peson doit être de 0 à 3 et GetPoids de 1..4 confirmé 06/06/2025


  for ( z=1;z<4;z++)
  {
    if (Peson[ConfigMateriel.Num_Carte][z])
    {
      Poids_Peson(z) = GetPoids(z+1,10);
      sprintf(OLEDbuf,"getWeight %d Done",z);
      OLEDDebugDisplay(OLEDbuf);
    }
    else 
    {
      sprintf(OLEDbuf,"Peson %d: NONE ",z);
     OLEDDebugDisplay(OLEDbuf);
    }
  }

  
  initLoRa();

// void DS3231CompleteReset() si DS3231 out!

  
  initRTC();

//Affiche heure DS3231
static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix
char localbuf[21] = "00:00:00    00/00/00";  
  DateTime systemTime = rtc.now();

  snprintf(localbuf, 21, "%02d:%02d:%02d    %02d/%02d/%02d", 
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year()-2000);
debugSerial.println(localbuf);
// Affiche heure µC
  
  
// Désactiver TOUTES les interruptions temporairement
    noInterrupts();
  
// Configuration DS3231 AVANT interruption
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

// Configuration des alarmes RTC 1 et 2
if (DEBUG_INTERVAL_1SEC)
{
  debugSerial.println("Initialisation IRQ1");
  DS3231setRTCAlarm1();
}
if (DEBUG_WAKEUP_PAYLOAD)
{
  debugSerial.println("Initialisation IRQ2");
  DS3231setRTCAlarm2();
}
  OLEDDebugDisplay("Set RTC Alarms OK");
// Configuration interruption externe
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
// AJOUTÉ: Debug configuration
debugSerial.print("Configuration interruption sur pin ");
debugSerial.println(RTC_INTERRUPT_PIN);
  
  LowPower.attachInterruptWakeup(RTC_INTERRUPT_PIN, onRTCAlarm, FALLING);
debugSerial.println("Initialisation terminee");

//  forcerSynchronisationDS3231();
debugSerial.println("Mise à l'heure");

 
  dht.begin();
  if  (!read_DHT(dht))  // 
  {
//  sprintf(serialbuf,"Humidité : %.2f %%\nTemperature : %.2f °C",Data_LoRa.DHT_Hum,Data_LoRa.DHT_Temp);
//  debugSerial.println(serialbuf); 
    OLEDDebugDisplay("DHT Done");
    debugSerial.println("DHT Done");
  }
  else
  {
    OLEDDebugDisplay("DHT: Error");
    debugSerial.println("DHT: Error"); 
  } 

  for ( z=0;z<10;z++)
  {
    Data_LoRa.Bat_Voltage=getVBatMoy();   // calcul moyenne de 10 lectures
    Data_LoRa.Solar_Voltage=getVSolMoy();   // calcul moyenne de 10 lectures
  }
  
  OLEDDebugDisplayReset();
debugSerial.println("Start loop(); =====================================");

buildLoraPayload();
sendLoRaPayload((uint8_t*)payload,sizeof(payload)); //19);   // hex
setupDone = true;

// Activer les interruptions
  rtc.writeSqwPinMode(DS3231_OFF); 
  
// Réactiver les interruptions
    interrupts();

}

// ---------------------------------------------------------------------------*
// ===== LOOP PRINCIPAL =====
// ---------------------------------------------------------------------------*
void loop() 
{  static int index=0; 
   static int counter1s=0;   

      loopWDT  = millis();
//debugSerial.println("M");  // MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  gererLEDsNonBloquant();        // *** APPEL OBLIGATOIRE À CHAQUE CYCLE ***

// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);  
  if (modeExploitation)         // OK, validé, GreenLED => couleur Red
  {
    delay(10);  // anti rebond ILS/Switch
    handleOperationMode();    // normalement rien à faire dans ce mode.
// Envoi du payLoad si ISR2 

// ---------------------------------------------------------------------------*
// Gestion Clignotement LED Rouge non bloquant
// ---------------------------------------------------------------------------*
    if (wakeup1Sec) 
    {  
      config.applicatif.redLedDuration = 100;  // Clignotement Rouge = 100 ms
      LEDStartRed();                           //Clignotement 100ms
//debugSerial.println("BlinkRED");   
      wakeup1Sec = false;  
    }  
// FIN Gestion Clignotement LED non bloquant
  } 
  else                          // OK, validé, BlueLED
  {
// seulement en PROG  
// ---------------------------------------------------------------------------*
// Gestion clavier non bloquant
// ---------------------------------------------------------------------------*
  processContinuousKeyboard();      // *** TRAITEMENT CLAVIER NON-BLOQUANT ***
  touche = readKeyNonBlocking();  // lecture du clavier
  if (touche != KEY_NONE)         // Affiche touche pressée (KEY1..5, KEY_INVALID)
  {
    // Traiter la touche
    sprintf(serialbuf,"Touche pressée: %s",keyToString(touche));
    debugSerial.println(serialbuf);
  }
#ifdef __SerialDebugPoc
debugSerial.print("2");   // 22222222222222222222222
#endif  
// seulement en PROG  sans envoi Payload, contrôle process
// Affiche l'heure du prochain PayLoad si displayNextPayload == true     
 if (displayNextPayload)
 {
   displayNextPayload = false;
   debugSerialPrintNextAlarm(nextPayload,2);
 }
#ifdef __SerialDebugPoc
debugSerial.print("3");   // 333333333333333333333333333
#endif  

// seulement en PROG  
// ---------------------------------------------------------------------------*
// ISR1 => Lecture Cyclique des paramètres répartis à chaque seconde
// ---------------------------------------------------------------------------*
  if (wakeup1Sec) // && !modeExploitation)            // màj heure, blink LED
  {    
    loopWDT  = millis();
//#ifdef __SerialDebugPoc     
//debugSerial.print("I1$ WDT:");   //   I1$ I1$ I1$ I1$ I1$ I1$ I1$ I1$
//debugSerial.println(loopWDT);
//#endif
    wakeup1Sec = false;   
    counter1s++;
    
// rappel toutes les minutes
    if (!(counter1s %60)) 
    {     
//      debugSerialPrintReprogNextAlarm(2);
      debugSerialPrintNextAlarm(nextPayload,2);
      debugSerial.print("menuDeep: ");    // MMMMMMMMMMMMMMMMMMMMMMMMMMM
      debugSerial.println(currentMenuDepth);
    }
    switch (counter1s %10)
    {
      case 0 :    // Rafraichir OLED
  //             OLEDDisplayHivesDatas();  // pas quand saisies en cours....
//debugSerial.println("Case0");
               break;
      case 1 :   //  read_DHT(dht);  
// Reading temperature or humidity takes about 250 milliseconds!
//debugSerial.println("Case1");
               read_DHT(dht); // init: Data_LoRa.DHT_Temp, Data_LoRa.DHT_Hum
               break;
     case 2 :
//debugSerial.println("Case2");
               Data_LoRa.Brightness = getLuminance();
//  Data_LoRa.Lux = ???();
//  Data_LoRa.ProcessorTemp = getTemperature(); // lecture Temp en String
/*               Data_LoRa.Bat_Voltage=getVBatMoy();
*/              Data_LoRa.Solar_Voltage=getVSolMoy();
               break;
     case 3 :
//debugSerial.println("Case3");
              if (Peson[ConfigMateriel.Num_Carte][0])
                Poids_Peson(0) = GetPoids(1,1);
// afficher poids bal(1) sur fenêtre OLEDdisplayWeightBal(void)
              break;
     case 4 :
//debugSerial.println("Case4");
              if (Peson[ConfigMateriel.Num_Carte][1])
                Poids_Peson(1) = GetPoids(2,1);
              break;
     case 5 :
//debugSerial.println("Case5");
              if (Peson[ConfigMateriel.Num_Carte][2])
                Poids_Peson(2) = GetPoids(3,1);
              break;
     case 6 :
//debugSerial.println("Case6");
              if (Peson[ConfigMateriel.Num_Carte][0])
                Poids_Peson(3) = GetPoids(4,1);
              break;
     case 7 :
//debugSerial.println("Case7");
              readingT=getTemperature();
               break;
      case 8 :
//debugSerial.println("Case8");
              break;
      case 9 :      // Alive: '.' sur OLED
              debugSerial.print(".");
              break;
      default : // WTF
               debugSerial.print("WTF");  
               break;
    }
#ifdef __SerialDebugPoc      
 debugSerial.print("5");    // 55555555555555555555555555555555555
#endif 
// FIN ISR1 => Lecture Cyclique des paramètres répartis à chaque seconde

// ---------------------------------------------------------------------------*
// Gestion Clignotement LED Bleue non bloquant
// ---------------------------------------------------------------------------*
    config.applicatif.blueLedDuration = 100;  // Clignotement Blue = 100 ms
    LEDStartBlue();                           //Clignotement 100ms
// FIN Gestion Clignotement LED non bloquant

// ---------------------------------------------------------------------------*
// Gestion rafraichissement écrans (System\INFOS, )
// ---------------------------------------------------------------------------*
    OLEDRefreshDisplay();
  }
// fin de ISR1
#ifdef __SerialDebugPoc  
debugSerial.print("6");   // 666666666666666666666666666666666666666666666
#endif

// ---------------------------------------------------------------------------*
// traite : 
// ---------------------------------------------------------------------------*
    handleProgrammingMode();  // faire gestion Clavier et actions associées
  }

#ifdef __SerialDebugPoc  
debugSerial.print("4");   // 444444444444444444444444444444
#endif

  if (!DEBUG_INTERVAL_1SEC)
  { static unsigned long noIRQSecLast=0;
    if (millis() - noIRQSecLast > 1000) // Clignotement toutes les 1s
    {
      noIRQSecLast = millis();
      wakeup1Sec = true;
    } 
//debugSerial.println("dans if (!DEBUG_INTERVAL_1SEC) /////////////////////////////////////////////////////////////////////////////////");    
  }
  
}
