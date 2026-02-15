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

//#define __SerialDebugPoc      // decommenter pour afficher messages debug

// =====  PROGRAMME =====
// ===== SETUP =====
void setup() 
{ char msg[80];
// Initialisation de debugSerial et envoi infos compil.
  SETUPinitDebugSerial(); 
  LOG_INFO("debugSerial initialisé");
// Initialisation de LoraSerial
  SETUPSETUPinitLoRaSerial();
  LOG_INFO("LoraSerial initialisé");
// Initialisation des LEDs
  initLEDs();
  LOG_INFO("LED initialisé");    
// Initialisation du mode selon PIN_PE
  pinMode(PIN_PE, INPUT_PULLUP);
  modeExploitation = digitalRead(PIN_PE);
//
  snprintf(msg,80,modeExploitation ? "Mode détecté: EXPLOITATION" : "Mode détecté: PROGRAMMATION"); 
  LOG_INFO(msg); 
  // Initialisation I2C
  Wire.begin();
  LOG_INFO("I2C initialisé");
// scanI2C???
// Initialisation OLED
  debugOLEDDrawText = false;
  OLEDInit();
  LOG_INFO("OLED initialisé");
/*
// Après display.begin() si décallage de 2 lignes
display.ssd1306_command(0x21);  // Set column address
display.ssd1306_command(2);      // Column start address (2 au lieu de 0)
display.ssd1306_command(129);    // Column end address (128 + 2 - 1)
*/
// Initialisation configuration
  E24C32initConfig();     // appel E24C32loadConfig();
  OLEDDebugDisplay("initConfig        OK");
//OLEDDebugDisplay("01234567890123456789");
  LOG_INFO("Config EEPROM lue");
//  
  SETUP_HX711initWithWatchdog(1);  // initialise toutes les balances présentes et les mets en basse conso
  LOG_INFO("HX711 initialisé");      
  for ( int numGauge=0;numGauge<4;numGauge++) 
  {     
    if (Peson[config.materiel.Num_Carte][numGauge])
    { 
      switch (numGauge)
      { case 0 : Contrainte_List[numGauge]=MESURESHX711GetStrainGauge(numGauge,scaleA,AVR_10);
                 break;
        case 1 : Contrainte_List[numGauge]=MESURESHX711GetStrainGauge(numGauge,scaleB,AVR_10);
                 break;
        case 2 : Contrainte_List[numGauge]=MESURESHX711GetStrainGauge(numGauge,scaleC,AVR_10);
                 break;
        case 3 : Contrainte_List[numGauge]=MESURESHX711GetStrainGauge(numGauge,scaleD,AVR_10);
                 break;            
      } 
      HiveSensor_Data.HX711Weight[numGauge] = poidsBal_kg(numGauge); //calculePoids(numGauge);
      snprintf(OLEDbuf, 21,"Bal. %c:  %8.2f kg",numGauge+65,poidsBal_kg(numGauge));  //calculePoids(numGauge));
      OLEDDebugDisplay(OLEDbuf);
      LOG_INFO(OLEDbuf);
    }
    else 
    {
      sprintf(OLEDbuf,"Bal. %d:         NONE",numGauge);
      LOG_INFO(OLEDbuf);
      OLEDDebugDisplay(OLEDbuf);
    }
  }
// INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa ---- INIT LoRa 
   OLEDDebugDisplay("RN2483A  in progress");
    LOG_INFO("Init LoRa en cours...");
  if (SETUPinitLoRa())
  {
    LOG_INFO("RN2483A initialisé");

char msg[20];
    byteArrayToHexString(config.materiel.DevEUI, 8, msg, 17);
    sprintf(OLEDbuf,"ID: %s",msg);
    OLEDDebugDisplay(OLEDbuf);
    LOG_INFO(OLEDbuf);
    OLEDDebugDisplay("LoRa              OK");
    LOG_INFO("LoRa initialisé");
  }
  else
  {
    OLEDDebugDisplay("LoRa          Failed");  
    LOG_INFO("LoRa non initialisé /!\\");
  }
  LOG_INFO("Envoi LoRa \"Restart\" Payload...");// ici ajouter testpayload
  RN2483AsendLoRaPayload((uint8_t*)testPayload,7);
  LOG_INFO("\"Restart\" Payload envoyé");
  initRTC();
  LOG_INFO("RTC initialisé");
//Affiche heure DS3231
static DateTime oldSystemTime(1692712245);  //1692712245);  // timestamp Unix
char localbuf[21] = "00:00:00    00/00/00";  
  DateTime systemTime = rtc.now();

  snprintf(localbuf, 21, "%02d:%02d:%02d    %02d/%02d/%02d", 
          systemTime.hour(), systemTime.minute(), systemTime.second(),
          systemTime.day(), systemTime.month(), systemTime.year()-2000);
  LOG_INFO(localbuf);
// Affiche heure µC
//OLEDDebugDisplay("01234567890123456789");
  OLEDDebugDisplay("Set Time          OK");
  LOG_INFO("Set Time OK");

// Désactiver TOUTES les interruptions temporairement
    noInterrupts();
  
// Configuration DS3231 AVANT interruption
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  // Configuration des alarmes RTC 1 et 2
  if (DEBUG_INTERVAL_1SEC)
  {
    LOG_INFO("Initialisation IRQ1");
    DS3231setRTCAlarm1();
  }
  if (DEBUG_WAKEUP_PAYLOAD)
  {
    LOG_INFO("Initialisation IRQ2");
    DS3231setRTCAlarm2();
  }
  OLEDDebugDisplay("Set RTC Alarms    OK");
// Configuration interruption externe
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
// AJOUTÉ: Debug configuration
debugSerial.print("Configuration interruption sur pin ");
debugSerial.println(RTC_INTERRUPT_PIN);
  
  LowPower.attachInterruptWakeup(RTC_INTERRUPT_PIN, ISRonRTCAlarm, FALLING);
LOG_INFO("Initialisation RTC terminee");

//  DS3231forcerSynchronisation();
//  debugSerial.println("Mise à l'heure");

  dht.begin();
  if  (!read_DHT(dht))  // initialise HiveSensor_Data.DHT_Hum et HiveSensor_Data.DHT_Temp
  {
    OLEDDebugDisplay("DHT               OK");
    LOG_INFO("DHT Done");
  }
  else
  {
    OLEDDebugDisplay("DHT            Error");
    LOG_ERROR("DHT: Error"); 
  } 
// init tensions Moyennes Bat/Sol 
  for ( int z=0;z<10;z++)
  {
    HiveSensor_Data.Bat_Voltage=getVBatMoy();   // calcul moyenne de 10 lectures
    HiveSensor_Data.Solar_Voltage=getVSolMoy();   // calcul moyenne de 10 lectures
  }
  LOG_INFO("Tensions BAT et SOL mesurées");
  LOG_INFO("Envoi Payload en cours...");
//  OLEDDebugDisplayReset();
  buildLoraPayload();
  RN2483AsendLoRaPayload((uint8_t*)payload,sizeof(payload)); //19);   // hex
  LOG_INFO("Payload envoyé.");


// Activer les interruptions
  rtc.writeSqwPinMode(DS3231_OFF); 
  
// Réactiver les interruptions
    interrupts();
  LOG_INFO("Start loop()");
}

// ---------------------------------------------------------------------------*
// ===== LOOP PRINCIPAL =====
// ---------------------------------------------------------------------------*
void loop() 
{ static int index=0; 
  static int counter1s=0;   

  loopWDT  = millis();
  gererLEDsNonBloquant();        // *** APPEL OBLIGATOIRE À CHAQUE CYCLE ***

// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);

    
  if (modeExploitation)         // OK, validé, GreenLED => couleur Red
  {
    delay(10);  // anti rebond ILS/Switch
    handleOperationMode();    // normalement rien à faire dans ce mode.
// Envoi du payLoad si ISR2 

// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);


// ---------------------------------------------------------------------------*
// Gestion Clignotement LED Rouge non bloquant
// ---------------------------------------------------------------------------*
    if (wakeup1Sec) 
    {  
      wakeup1Sec = false; 
  // attention     if (wakeup1Sec)   retseté plus bas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
      config.applicatif.redLedDuration = 100;  // Clignotement Rouge = 100 ms
      LEDStartRed();                           //Clignotement 100ms
  //debugSerial.println("BlinkRED");   
  // FIN Gestion Clignotement LED non bloquant 
      
  // Gestion des affichages de pages rafraichies    
  // FIN Gestion des affichages de pages rafraichies        
    }  
  } 
  else                          // OK, validé, BlueLED modeProgrammation
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
  if (wakeup1Sec) // && !modeExploitation)            // màj heure, 
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
      debugSerialPrintNextAlarm(nextPayload,2);
//debugSerial.print("menuDeep: ");    // MMMMMMMMMMMMMMMMMMMMMMMMMMM
//debugSerial.println(currentMenuDepth);
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
               HiveSensor_Data.Brightness = getLuminance();
//  HiveSensor_Data.Lux = ???();
//  HiveSensor_Data.ProcessorTemp = getTemperature(); // lecture Temp en String
/*               HiveSensor_Data.Bat_Voltage=getVBatMoy();
*/              HiveSensor_Data.Solar_Voltage=getVSolMoy();
               break;
     case 3 :
//debugSerial.println("Case3"); 
//logPeson = true;
              if (Peson[config.materiel.Num_Carte][0])
              {
                Contrainte_List[0] = MESURESHX711GetStrainGauge(0,scaleA,1);
                HiveSensor_Data.HX711Weight[0] = calculePoids(0);
              }
// afficher poids bal(1) sur fenêtre OLEDdisplayWeightBal(void)
logPeson = false;
              break;
     case 4 :
//debugSerial.println("Case4"); 
//logPeson = true;
              if (Peson[config.materiel.Num_Carte][1])
              {
                Contrainte_List[1] = MESURESHX711GetStrainGauge(1,scaleB,1);
                HiveSensor_Data.HX711Weight[1] = calculePoids(1);
              }               
logPeson = false;
              break;
     case 5 :
//debugSerial.println("Case5");
//logPeson = true;
              if (Peson[config.materiel.Num_Carte][2])
              {
                Contrainte_List[2] = MESURESHX711GetStrainGauge(2,scaleC,1);
                HiveSensor_Data.HX711Weight[2] = calculePoids(2);
              }               
logPeson = false;
              break;
     case 6 :
//debugSerial.println("Case6");
//logPeson = true;
              if (Peson[config.materiel.Num_Carte][3])
              {
                Contrainte_List[3] = MESURESHX711GetStrainGauge(3,scaleD,1);
                HiveSensor_Data.HX711Weight[3] = calculePoids(3);
              }               
logPeson = false;
              break;
     case 7 :
//debugSerial.println("Case7");
              readingT=getTemperature();
               break;
      case 8 :
//debugSerial.println("Case8");
              break;
      case 9 :      // Alive: '.' sur OLED
  //            debugSerial.print(".");
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



// mode rafraichissement rapide des balances
// attention le scale.begin  dure > 400 ms
// les 10 lectures 900 ms
//debugSerial.println("BalRap A");
    if (InfoBalScreenRefreshBal_1)
    {
      Contrainte_List[0] = MESURESHX711GetStrainGauge(0,scaleA,AVR_3);
      HiveSensor_Data.HX711Weight[0] = calculePoids(0);
    }
//debugSerial.println("BalRap B");   
    if (InfoBalScreenRefreshBal_2)
    {
      Contrainte_List[1] = MESURESHX711GetStrainGauge(1,scaleB,AVR_3);
      HiveSensor_Data.HX711Weight[1] = calculePoids(1);
    }   
//debugSerial.println("BalRap C");            
    if (InfoBalScreenRefreshBal_3)
    {
      Contrainte_List[2] = MESURESHX711GetStrainGauge(2,scaleC,AVR_3);
      HiveSensor_Data.HX711Weight[2] = calculePoids(2);
    }    
// debugSerial.println("BalRap D");               
    if (InfoBalScreenRefreshBal_4)
    {
      Contrainte_List[3] = MESURESHX711GetStrainGauge(3,scaleD,AVR_3);
      HiveSensor_Data.HX711Weight[3] = calculePoids(3);
    }  
 
 
// ---------------------------------------------------------------------------*
// Gestion rafraichissement écrans (System\INFOS, )
// ---------------------------------------------------------------------------*
    OLEDRefreshDisplay();
  }
// fin de ISR1

// ---------------------------------------------------------------------------*
// traite : 
// ---------------------------------------------------------------------------*
    handleProgrammingMode();  // faire gestion Clavier et actions associées

// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);
    
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
