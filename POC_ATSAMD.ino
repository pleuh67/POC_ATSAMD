
//Virer 3 x colonne?0:0 valider sur 0.96"
// https://claude.ai/chat/75e1ba53-8b90-4f09-97a5-cfc366e36a8e
// 16:40:59.416 -> Alarme 2 (payload) programmée pour: 16:39:16  afficher a chaque reprogrammation
//  Compiled: Aug 22 2025, 08:34:58, 1.1.1-PL


/**
 * @file POC_IRQ_LOW_POWER.ino
 * @brief Programme de démonstration pour SODAQ EXPLORER avec gestion basse consommation
 * @author Votre nom
 * @version 1.1.1-PL
 * @date 2025
 * 
 * Description : Gestion de deux modes de fonctionnement avec interruptions RTC
 * - MODE EXPLOITATION : réveil périodique pour LED rouge
 * - MODE PROGRAMMATION : réveil périodique + clignotement LED builtin + interface utilisateur
 */

#define __MAIN__

// ===== INCLUDES PROJET =====
#include ".\define.h"

//#define __SendLoRa

// =====  PROGRAMME =====
// ===== SETUP =====
void setup() 
{
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
  if (OLED) 
  {
    OLEDInit();
    OLEDDebugDisplay("OLEDInit OK"); // scrolling lors du setup(); 
  }

  init2483A();
  initLoRa();

// void DS3231CompleteReset() si DS3231 out!

  // Initialisation RTC
  initRTC();
  OLEDDebugDisplay("initRTC OK");

  // Initialisation configuration
  initConfig();
  OLEDDebugDisplay("initConfig OK");
        
  // Configuration des alarmes RTC 1 et 2
  setRTCAlarms();
  OLEDDebugDisplay("setRTCAlarms OK");
    
  // Configuration interruption externe
  pinMode(RTC_INTERRUPT_PIN, INPUT_PULLUP);
// AJOUTÉ: Debug configuration
debugSerial.print("Configuration interruption sur pin ");
debugSerial.println(RTC_INTERRUPT_PIN);

// AJOUTÉ: Test état initial
debugSerial.print("État initial pin RTC: ");
debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");

// AJOUTÉ: Configuration DS3231 AVANT interruption
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
    
  LowPower.attachInterruptWakeup(RTC_INTERRUPT_PIN, onRTCAlarm, FALLING);
    
 // AJOUTÉ: Tests supplémentaires
debugSerial.println("Interruption RTC attachée");
delay(1000);
debugSerial.print("État pin après config: ");
debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");

debugSerial.println("Initialisation terminee");

  forcerSynchronisationDS3231();

debugSerial.println("Mise à l'heure");

  dht.begin();
  read_DHT(dht);

  sprintf(serialbuf,"Humidité : %.2f ",Data_LoRa.DHT_Hum );
  debugSerial.println(serialbuf); 
  sprintf(serialbuf,"Temperature : %.2f °C",Data_LoRa.DHT_Temp );
  debugSerial.println(serialbuf); 
    
  if (OLED) 
  {
    OLEDDebugDisplay("Init terminee");
    delay(2000);
    OLEDClear();
  }
//  vider cache : OLEDDebugDisplay()  
 OLEDDebugDisplayReset();
 debugSerial.println("Start loop(); =====================================");

}



//int cnt=0;

// ===== LOOP PRINCIPAL =====
void loop() 
{  static int index=0; 
   static int counter1s=1,counter15m=1;   


//debugSerial.println("M");
// *** TRAITEMENT CLAVIER NON-BLOQUANT ***
  processContinuousKeyboard();
// *** UTILISATION DES TOUCHES ***
  key_code_t touche = readKeyNonBlocking();
  if (touche != KEY_NONE)
  {
    // Traiter la touche
    debugSerial.print("Touche pressée: ");
    debugSerial.println(keyToString(touche));
  }

// *** APPEL OBLIGATOIRE À CHAQUE CYCLE ***
    gererLEDsNonBloquant(); 
  
// Vérification du mode
  modeExploitation = digitalRead(PIN_PE);
  if (modeExploitation)         // OK, validé, GreenLED => couleur Red
  {
  debugSerial.println("H"); 
  handleOperationMode();    // normalement rien à faire dans ce mode.
  debugSerial.println("h"); 
  } 
  else                          // OK, validé, BlueLED
  {
//debugSerial.println("H"); 
    handleProgrammingMode();  // faire gestion Clavier et actions associées
//debugSerial.println("h");
  }
 
/*
if (!(index%10)) // tous les 10 loop
{
  read_DHT(dht);
  sprintf(serialbuf,"Humidité : %.2f ",Data_LoRa.DHT_Hum );
  debugSerial.println(serialbuf); 
  sprintf(serialbuf,"Temperature : %.2f °C",Data_LoRa.DHT_Temp );
  debugSerial.println(serialbuf); 
}
index++;
*/
    
// Gestion des interruptions traitées en dehors de 
// handleProgrammingMode() et handleOperationMode()
  if (wakeupPayload)                                    // Envoi LoRa, LED Activité LoRa
  {
    wakeupPayload = false;
sprintf(serialbuf, "IRQ2 15m %d =====", counter15m);
debugSerial.println(serialbuf); 
/* 
 sprintf(OLEDbuf,  "IRQ2 15m %d ", counter15m);
OLEDDebugDisplay(OLEDbuf);   
*/       
counter15m++;



    
 debugSerial.println("PROG/Réveil payload");
/*    if (OLED) 
    {
        OLEDDebugDisplay("PROG/Reveil payload");
    }
*/
    turnOnRedLED();     // PCB donne GREEN?
#ifdef __SendLoRa
    Send_LoRa_Mess((uint8_t*)testPayload,7);
#endif    
    turnOffRedLED();
    OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet
  }
  
  if (wakeup1Sec) // && !modeExploitation)                  // màj heure, blink LED
  {     
sprintf(serialbuf, "IRQ1 1s %d =====", counter1s);
debugSerial.println(serialbuf); 
/* 
 sprintf(OLEDbuf,  "IRQ1 1s %d ", counter1);
OLEDDebugDisplay(OLEDbuf);   
*/       
counter1s++;
//debugSerial.println("S"); 
    wakeup1Sec = false;
    LEDStartBlue();
    if (OLED) 
    {
      OLEDDrawScreenRefreshTime(0, 0); // partial refresh Time/Date every second
    }
//debugSerial.println("s"); 
  }
    
    // Entrée en veille si activée
  if (DEBUG_LOW_POWER && modeExploitation) // pas de low power en config
  {
debugSerial.println("L");     
    debugSerial.flush();  
    sleep();
debugSerial.println("l"); 
  }
}

// ===== IMPLÉMENTATION DES FONCTIONS =====



/**
 * @brief Définit la configuration par défaut
 * @param Aucun
 * @return void
 */
void setDefaultConfig(void) 
{
    config.materiel.version = CONFIG_VERSION;
    config.materiel.adresseRTC = DS3231_ADDRESS;    // Adresse RTC Module DS3231
    config.materiel.adresseOLED = OLED_ADDRESS;     // Adresse écran OLED
    config.materiel.adresseEEPROM = EEPROM_ADDRESS; // Adresse EEPROM Module DS3231
   
    config.applicatif.redLedDuration = RED_LED_DURATION;
    config.applicatif.greenLedDuration = GREEN_LED_DURATION;
    config.applicatif.blueLedDuration = BLUE_LED_DURATION;
    config.applicatif.builtinLedDuration = BUILTIN_LED_DURATION;
    config.applicatif.wakeupIntervalPayload = WAKEUP_INTERVAL_PAYLOAD;
    config.applicatif.interval1Sec = INTERVAL_1SEC;
    
    config.checksum = calculateChecksum(&config);
}

/**
 * @brief Gestion du mode exploitation
 * @param Aucun
 * @return void
 */
void handleOperationMode(void) 
{ // Mode exploitation : seulement réveil payload
  if (OLED) 
  {
    if (switchToOperationMode)    // affiche qu'une fois
    { 
      switchToProgrammingMode = true;
      OLEDClear(); 
      OLEDDrawText(1, 7, 0, "MODE EXPLOITATION");
      switchToOperationMode = false;
    }  
  }
}

/**
 * @brief Gestion du mode programmation
 * @param Aucun
 * @return void
 */
void handleProgrammingMode(void) 
{    // Mode programmation : réveil payload + 1 sec + interface utilisateur
  if (OLED) 
  {
    if (switchToProgrammingMode)    // affiche qu'une fois
    {  
      switchToOperationMode = true;
      OLEDClear();
      OLEDDrawScreenTime(0,0);
      OLEDDrawText(1, 7, 0, "MODE PROGRAMMATION");
      switchToProgrammingMode = false;
    } 
//    else 
  //    OLEDDrawScreenRefreshTime(0, 0); // fait par IRQ1
  }
}

/**
 * @brief Fonction d'interruption RTC
 * @param Aucun
 * @return void
 */
void onRTCAlarm(void) 
{
// ALARME 1 : Toutes les secondes (mode programmation)
  if (rtc.alarmFired(1)) 
  {
    rtc.clearAlarm(1);
    wakeup1Sec = true;
// en mode  DS3231_A1_PerSecond, ne pas reprogrammer l'alarme
// L'alarme DS3231_A1_PerSecond se répète automatiquement
// Seulement reprogrammer si le mode change
// reprogramer si DS3231_A1_Second, faire :  
// Reprogrammer l'alarme 1 seconde si en mode programmation
// ici toutes les minutes quand la seconded programmée survient
    if (DEBUG_INTERVAL_1SEC && !modeExploitation) 
    {
//      DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 1);
 //     rtc.setAlarm1(nextSecond, DS3231_A1_Second);
//     rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);     
// debugSerialPrintNextAlarm(nextSecond, 1); 
    }
  }
  
  // ALARME 2 : Payload périodique    
  if (rtc.alarmFired(2)) 
  {
    wakeupPayload = true;
    rtc.clearAlarm(2);      
    setRTCAlarms(); // Reprogrammer prochaine alarme
  }
}



void non_forceTestAlarm(void)
{
    debugSerial.println("=== TEST FORCÉ ALARME (10 SECONDES) ===");
    
    // Alarme dans 10 secondes
    DateTime testTime = rtc.now() + TimeSpan(0, 0, 0, 10);
    rtc.clearAlarm(1);
    rtc.setAlarm1(testTime, DS3231_A1_Minute);
    
    debugSerial.print("Alarme test programmée pour: ");
    debugSerial.print(testTime.hour()); debugSerial.print(":");
    debugSerial.print(testTime.minute()); debugSerial.print(":");
    debugSerial.println(testTime.second());
    
    debugSerial.println("Surveillez le pin et les messages...");
}


/**
 * @brief Met le microcontrôleur en veille
 * @param Aucun
 * @return void
 */
void sleep(void) 
{     static int sleepCount = 0;
    
    
    sleepCount++;
  OLEDDebugDisplay("LowPower.sleep()");
    // AJOUTÉ: Debug entrée en veille
    debugSerial.print("=== ENTRÉE EN VEILLE #");
    debugSerial.print(sleepCount);
    debugSerial.println(" ===");
    
    // AJOUTÉ: État pin avant veille
    debugSerial.print("État pin RTC avant veille: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    // AJOUTÉ: État des alarmes
    debugSerial.print("Alarmes actives: ");
    debugSerial.print("A1="); debugSerial.print((DEBUG_INTERVAL_1SEC && !modeExploitation) ? "ON" : "OFF");
    debugSerial.print(" A2="); debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
    
    // AJOUTÉ: Forcer envoi série avant veille
    debugSerial.flush();
    delay(10);
    
    LowPower.sleep();  // ← VEILLE
    
    // AJOUTÉ: Debug réveil
    debugSerial.println("=== RÉVEIL DÉTECTÉ ===");
    
    // AJOUTÉ: État pin après réveil
    debugSerial.print("État pin RTC après réveil: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    // AJOUTÉ: Stabilisation
    delay(10);
}


// ===== FONCTIONS UTILITAIRES SUPPLÉMENTAIRES =====

/**
 * @brief Configure les registres de bas niveau pour la basse consommation
 * @param Aucun
 * @return void
 */
void configureLowPowerMode(void)
{
    // Désactiver les périphériques non utilisés pour économiser l'énergie
    // Configuration spécifique ATSAMD21
    
    // Désactiver l'USB si non utilisé en mode exploitation
    if (modeExploitation && DEBUG_LOW_POWER)
    {
       USB->DEVICE.CTRLA.bit.ENABLE = 0;
    }
    
    debugSerial.println("Mode basse consommation configuré");
}

/**
 * @brief Vérifie l'état des alarmes RTC et met à jour les flags - ALARMES ÉCHANGÉES
 * @param Aucun
 * @return void
 */
void checkRTCStatus(void)
{
// Vérification de l'alarme 1 (1 seconde)
  if (rtc.alarmFired(1))
  {
    debugSerial.println("*** ALARME 1 DÉTECTÉE (1 SECONDE) ***");
    wakeup1Sec = true;
    rtc.clearAlarm(1);
 // même remarque que dans : void onRTCAlarm(void)    
  }
    
    // Vérification de l'alarme 2 (payload)
  if (rtc.alarmFired(2))
  {
    debugSerial.println("*** ALARME 2 DÉTECTÉE (PAYLOAD) ***");
    wakeupPayload = true;
    rtc.clearAlarm(2);
        
    // Reprogrammer l'alarme payload
    if (DEBUG_WAKEUP_PAYLOAD)
    {
      DateTime nextPayload = rtc.now() + TimeSpan(0, 0, config.applicatif.wakeupIntervalPayload, 0);
      rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
 debugSerialPrintNextAlarm(nextPayload,2);           
    }
  }
    
    // Debug périodique de l'état des alarmes (toutes les 60 secondes)
    static unsigned long lastAlarmDebug = 0;
  if (millis() - lastAlarmDebug > 60000)
  {
    lastAlarmDebug = millis();
        
    debugSerial.println("=== ÉTAT ALARMES RTC (ÉCHANGÉES) ===");
    debugSerial.print("Alarme 1 (1 sec) activée: ");
    debugSerial.println((DEBUG_INTERVAL_1SEC && !modeExploitation) ? "OUI" : "NON");
    debugSerial.print("Alarme 2 (payload) activée: ");
    debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "OUI" : "NON");
    debugSerial.print("Mode: ");
    debugSerial.println(modeExploitation ? "EXPLOITATION" : "PROGRAMMATION");
       
    // Afficher l'heure actuelle
    DateTime now = rtc.now();
    debugSerial.print("Heure actuelle: ");
    debugSerial.print(now.hour()); debugSerial.print(":");
    debugSerial.print(now.minute()); debugSerial.print(":");
    debugSerial.println(now.second());
  }
}


/**
 * @brief Gère l'affichage et les actions du mode exploitation
 * @param Aucun
 * @return void
 */
void executeOperationMode(void)     // pas appelé à la base....
{
    static unsigned long lastModeDisplay = 0;
    
    // Affichage périodique du mode (toutes les 10 secondes)
    if (millis() - lastModeDisplay > 10000)
    {
        lastModeDisplay = millis();
        
        if (OLED)
        {
            OLEDClear();
            OLEDDrawText(1,0, 0, PROJECT_NAME);
            OLEDDrawText(1,1, 0, "Mode: EXPLOITATION");
            OLEDDrawText(1,2, 0, "PIN_PE = HIGH");
            
            sprintf(OLEDbuf, "Reveil: %d min", config.applicatif.wakeupIntervalPayload);
            OLEDDrawText(1,4, 0, OLEDbuf);
            
            OLEDDrawScreenTime(0, 0);   // 0, 6 ????
            
            // Affichage état debug
            OLEDDrawText(1, 7, 0, DEBUG_LOW_POWER ? "Sleep: ON" : "Sleep: OFF");
        }
        
        debugSerial.println("=== MODE EXPLOITATION ===");
        debugSerial.print("Prochain réveil payload dans: ");
        debugSerial.print(config.applicatif.wakeupIntervalPayload);
        debugSerial.println(" minutes");
    }
}

/**
 * @brief Gère l'affichage et les actions du mode programmation
 * @param Aucun
 * @return void
 */
void executeProgrammingMode(void)
{ static unsigned long lastModeDisplay = 0;
  static bool menuDisplayed = false;
    
  // Affichage initial du menu
  if (!menuDisplayed || (millis() - lastModeDisplay > 30000))
  {
    lastModeDisplay = millis();
    menuDisplayed = true;
        
    if (OLED)
    {
      OLEDClear();
      OLEDDrawText(1,0, 0, "MODE PROGRAMMATION");
      OLEDDrawText(1,1, 0, "PIN_PE = LOW");
      OLEDDrawText(1,3, 0, "T1: Test clavier");
      OLEDDrawText(1,4, 0, "T2: Config");
      OLEDDrawText(1,5, 0, "T3: Heure/Date");
      OLEDDrawText(1,6, 0, "T4: Debug");
      OLEDDrawText(1,7, 0, "T5: Infos");
    }
    debugSerial.println("=== MODE PROGRAMMATION ===");
    debugSerial.println("Menu principal affiché");
  }
    
  // Traitement des touches du menu principal    
  key_code_t touche = readKey();
  if (touche != KEY_NONE)
  {
    menuDisplayed = false; // Forcer le réaffichage du menu après action
        
    switch (touche)
    {
      case KEY_1:
          debugSerial.println("Test clavier sélectionné");
          if (OLED)
          {
            OLEDDisplayMessageL8("Test clavier...", false, false);
          }
          // La fonction testKbd() sera appelée automatiquement dans la boucle
          break;
                
      case KEY_2:
          debugSerial.println("Configuration sélectionnée");
          handleConfigurationMenu();
          break;
                
      case KEY_3:
          debugSerial.println("Heure/Date sélectionnée");
          handleTimeDateRegisterMenu();
          break;
                
      case KEY_4:
          debugSerial.println("Debug sélectionné");
          handleDebugMenu();
          break;
              
      case KEY_5:
          debugSerial.println("Infos sélectionnées");
          displaySystemInfo();
          break;
                
      default:
          break;
    }
  }
  // Test clavier en continu en mode programmation
  debugSerialPrintKbdKey();
}

/**
 * @brief Gère le menu de configuration
 * @param Aucun
 * @return void
 */
void handleConfigurationMenu(void)
{
    bool menuConfig = true;
    
    while (menuConfig)
    {
        if (OLED)
        {
            OLEDClear();
            OLEDDrawText(1,0, 0, "=== CONFIGURATION ===");
            OLEDDrawText(1,2, 0, "T1: Duree LED rouge");
            OLEDDrawText(1,3, 0, "T2: Duree LED builtin");
            OLEDDrawText(1,4, 0, "T3: Intervalle payload");
            OLEDDrawText(1,5, 0, "T4: Sauvegarder");
            OLEDDrawText(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
            {
                uint32_t nouvelleDuree = inputDecimal("Duree LED rouge (ms)", config.applicatif.redLedDuration);
                config.applicatif.redLedDuration = nouvelleDuree;
                OLEDDisplayMessageL8("Duree LED modifiee", false, false);
                break;
            }
            
            case KEY_2:
            {
                uint32_t nouvelleDuree = inputDecimal("Duree LED builtin (ms)", config.applicatif.builtinLedDuration);
                config.applicatif.builtinLedDuration = nouvelleDuree;
                OLEDDisplayMessageL8("Duree LED modifiee", false, false);
                break;
            }
            
            case KEY_3:
            {
                uint32_t nouvelIntervalle = inputDecimal("Intervalle payload (min)", config.applicatif.wakeupIntervalPayload);
                config.applicatif.wakeupIntervalPayload = nouvelIntervalle;
                OLEDDisplayMessageL8("Intervalle modifie", false, false);
                break;
            }
            
            case KEY_4:
                saveConfigToEEPROM();
                OLEDDisplayMessageL8("Configuration sauvee!", false, false);
                break;
                
            case KEY_5:
                menuConfig = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}

/**
 * @brief Gère le menu de configuration Heure/Date
 * @param Aucun
 * @return void
 */
void handleTimeDateRegisterMenu(void)
{
    bool menuHeure = true;
    
    while (menuHeure)
    {
        if (OLED)
        {
            OLEDClear();
            OLEDDrawText(1,0, 0, "=== HEURE / DATE ===");
            OLEDDrawText(1,2, 0, "T1: Saisir heure");
            OLEDDrawText(1,3, 0, "T2: Saisir date");
            OLEDDrawText(1,4, 0, "T3: Afficher temps");
            OLEDDrawText(1,5, 0, "T4: Comparer horloges");
            OLEDDrawText(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
            {
                char heureStr[9] = "12:34:56";
                DateTime maintenant = rtc.now();
                sprintf(heureStr, "%02d:%02d:%02d", maintenant.hour(), maintenant.minute(), maintenant.second());
                inputTime(heureStr);
                
                // Mettre à jour le RTC avec la nouvelle heure
                int hh = (heureStr[0]-'0')*10 + (heureStr[1]-'0');
                int mm = (heureStr[3]-'0')*10 + (heureStr[4]-'0');
                int ss = (heureStr[6]-'0')*10 + (heureStr[7]-'0');
                
                DateTime nouvelleHeure(maintenant.year(), maintenant.month(), maintenant.day(), hh, mm, ss);
                rtc.adjust(nouvelleHeure);
                
                OLEDDisplayMessageL8("Heure mise a jour!", false, false);
                break;
            }
            
            case KEY_2:
            {
                char dateStr[11] = "01/01/2025";
                DateTime maintenant = rtc.now();
                sprintf(dateStr, "%02d/%02d/%04d", maintenant.day(), maintenant.month(), maintenant.year());
                inputDate(dateStr);
                
                // Mettre à jour le RTC avec la nouvelle date
                int jj = (dateStr[0]-'0')*10 + (dateStr[1]-'0');
                int mm = (dateStr[3]-'0')*10 + (dateStr[4]-'0');
                int yyyy = (dateStr[6]-'0')*1000 + (dateStr[7]-'0')*100 + (dateStr[8]-'0')*10 + (dateStr[9]-'0');
                
                DateTime nouvelleDate(yyyy, mm, jj, maintenant.hour(), maintenant.minute(), maintenant.second());
                rtc.adjust(nouvelleDate);
                
                OLEDDisplayMessageL8("Date mise a jour!", false, false);
                break;
            }
            
            case KEY_3:
                printTimeOndebugSerial();
                OLEDDisplayMessageL8("Temps affiche serie", false, false);
                break;
                
            case KEY_4:
                printTimeComparison();
                OLEDDisplayMessageL8("Comparaison serie", false, false);
                break;
                
            case KEY_5:
                menuHeure = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}

/**
 * @brief Gère le menu de debug
 * @param Aucun
 * @return void
 */
void handleDebugMenu(void)
{
    bool menuDebug = true;
    
    while (menuDebug)
    {
        if (OLED)
        {
            OLEDClear();
            OLEDDrawText(1,0, 0, "=== DEBUG ===");
            
            sprintf(OLEDbuf, "Payload: %s", DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
            OLEDDrawText(1,2, 0, OLEDbuf);
            
            sprintf(OLEDbuf, "1Sec: %s", DEBUG_INTERVAL_1SEC ? "ON" : "OFF");
            OLEDDrawText(1,3, 0, OLEDbuf);
            
            sprintf(OLEDbuf, "Sleep: %s", DEBUG_LOW_POWER ? "ON" : "OFF");
            OLEDDrawText(1,4, 0, OLEDbuf);
            
            OLEDDrawText(1,6, 0, "T1/T2/T3: Toggle");
            OLEDDrawText(1,7, 0, "T5: Retour");
        }
        
        key_code_t touche = readKey();
        switch (touche)
        {
            case KEY_1:
                DEBUG_WAKEUP_PAYLOAD = !DEBUG_WAKEUP_PAYLOAD;
                debugSerial.print("DEBUG_WAKEUP_PAYLOAD: ");
                debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
                if (!DEBUG_WAKEUP_PAYLOAD) clearRTCAlarms();
                else setRTCAlarms();
                break;
                
            case KEY_2:
                DEBUG_INTERVAL_1SEC = !DEBUG_INTERVAL_1SEC;
                debugSerial.print("DEBUG_INTERVAL_1SEC: ");
                debugSerial.println(DEBUG_INTERVAL_1SEC ? "ON" : "OFF");
                if (!DEBUG_INTERVAL_1SEC) clearRTCAlarms();
                else setRTCAlarms();
                break;
                
            case KEY_3:
                DEBUG_LOW_POWER = !DEBUG_LOW_POWER;
                debugSerial.print("DEBUG_LOW_POWER: ");
                debugSerial.println(DEBUG_LOW_POWER ? "ON" : "OFF");
                configureLowPowerMode();
                break;
                
            case KEY_5:
                menuDebug = false;
                break;
                
            default:
                break;
        }
        
        delay(100);
    }
}


/**
 * @brief Affiche les informations système
 * @param Aucun
 * @return void
 */
void displaySystemInfo(void)  // remplacer par OLED et serialDebug
{
    if (OLED)
    {
        OLEDClear();
        OLEDDrawText(1,0, 0, "=== INFOS SYSTEME ===");
        OLEDDrawText(1,1, 0, PROJECT_NAME);
        OLEDDrawText(1,2, 0, "Version: " VERSION);
        
        sprintf(OLEDbuf, "Mode: %s", modeExploitation ? "EXPLOIT" : "PROGRAM");
        OLEDDrawText(1,3, 0, OLEDbuf);
        
        sprintf(OLEDbuf, "Config v: %d.%02d", config.materiel.version/100, config.materiel.version%100);
        OLEDDrawText(1,4, 0, OLEDbuf);
        
        OLEDDrawText(1,6, 0, "Appuyer pour continuer");
    }
    
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
    
    // Attendre une touche pour continuer
    while (readKey() == KEY_NONE)
    {
        delay(100);
    }
    
    OLEDDisplayMessageL8("Retour menu principal", false, false);
}
