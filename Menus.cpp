
#define __INIT_DONE
#include "define.h"

// PROGRAMMATION MODE

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
        
    OLEDClear();
OLEDDrawScreenTime(0, 0); // Affiche Time/Date au complet
    OLEDDrawText(1,1, 0, "MODE PROGRAMMATION");
    OLEDDrawText(1,2, 0, "PIN_PE = LOW");
    OLEDDrawText(1,3, 0, "T1: Test clavier");
    OLEDDrawText(1,4, 0, "T2: Config");
    OLEDDrawText(1,5, 0, "T3: Heure/Date");
    OLEDDrawText(1,6, 0, "T4: Debug");
    OLEDDrawText(1,7, 0, "T5: Infos");
    debugSerial.println("=== MODE PROGRAMMATION ===");
    debugSerial.println("Menu principal affiché");
  }
    
  // Traitement des touches du menu principal    
//  key_code_t touche = readKey();
  if (touche != KEY_NONE)
  {
    menuDisplayed = false; // Forcer le réaffichage du menu après action
        
    switch (touche)
    {
      case KEY_1:
          debugSerial.println("Test clavier sélectionné");
          OLEDDisplayMessageL8("Test clavier...", false, false);
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
      OLEDClear();
      OLEDDrawText(1,0, 0, "=== CONFIGURATION ===");
      OLEDDrawText(1,2, 0, "T1: Duree LED rouge");
      OLEDDrawText(1,3, 0, "T2: Duree LED builtin");
      OLEDDrawText(1,4, 0, "T3: Intervalle payload");
      OLEDDrawText(1,5, 0, "T4: Sauvegarder");
      OLEDDrawText(1,7, 0, "T5: Retour");
      key_code_t touche = readKey();
      switch (touche)
      {
        case KEY_1:
        {
/*                
                uint32_t nouvelleDuree = inputDecimal("Duree LED rouge (ms)", config.applicatif.redLedDuration);
                config.applicatif.redLedDuration = nouvelleDuree;
                OLEDDisplayMessageL8("Duree LED modifiee", false, false);
*/
            break;
        }
        case KEY_2:
        {
/*
                uint32_t nouvelleDuree = inputDecimal("Duree LED builtin (ms)", config.applicatif.builtinLedDuration);
                config.applicatif.builtinLedDuration = nouvelleDuree;
                OLEDDisplayMessageL8("Duree LED modifiee", false, false);
*/
            break;
        }
        case KEY_3:
        {
/*
                uint32_t nouvelIntervalle = inputDecimal("Intervalle payload (min)", config.applicatif.wakeupIntervalPayload);
                config.applicatif.wakeupIntervalPayload = nouvelIntervalle;
                OLEDDisplayMessageL8("Intervalle modifie", false, false);
*/
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
    OLEDClear();
    OLEDDrawText(1,0, 0, "=== HEURE / DATE ===");
    OLEDDrawText(1,2, 0, "T1: Saisir heure");
    OLEDDrawText(1,3, 0, "T2: Saisir date");
    OLEDDrawText(1,4, 0, "T3: Afficher temps");
    OLEDDrawText(1,5, 0, "T4: Comparer horloges");
    OLEDDrawText(1,7, 0, "T5: Retour");
    key_code_t touche = readKey();
    switch (touche)
    {
      case KEY_1:
      {
/*
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
*/                
          break;
        }
        case KEY_2:
        {
/*
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
*/
          break;
        }
        case KEY_3:
              debugSerialPrintTime();
              OLEDDisplayMessageL8("Temps affiche serie", false, false);
              break;
        case KEY_4:
                debugSerialPrintTimeComparison();
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
        
    key_code_t touche = readKey();
    switch (touche)
    {
      case KEY_1:
                DEBUG_WAKEUP_PAYLOAD = !DEBUG_WAKEUP_PAYLOAD;
                debugSerial.print("DEBUG_WAKEUP_PAYLOAD: ");
                debugSerial.println(DEBUG_WAKEUP_PAYLOAD ? "ON" : "OFF");
                if (!DEBUG_WAKEUP_PAYLOAD) clearRTCAlarms();
                else DS3231setRTCAlarm2();
                break;
                
      case KEY_2:
                DEBUG_INTERVAL_1SEC = !DEBUG_INTERVAL_1SEC;
                debugSerial.print("DEBUG_INTERVAL_1SEC: ");
                debugSerial.println(DEBUG_INTERVAL_1SEC ? "ON" : "OFF");
                if (!DEBUG_INTERVAL_1SEC) clearRTCAlarms();
                else DS3231setRTCAlarm1();
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
  OLEDDisplaySystemInfo();
  debugSerialPrintSystemInfo();
   
// Attendre une touche pour continuer
  while (readKey() == KEY_NONE)
  {
    delay(100);
  }
  OLEDDisplayMessageL8("Retour menu princ.", false, false);
}




















// EXPLOITATION MODE
// Pas de menus normalement


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
        
    OLEDClear();
    OLEDDrawText(1,0, 0, PROJECT_NAME);
    OLEDDrawText(1,1, 0, "Mode: EXPLOITATION");
    OLEDDrawText(1,2, 0, "PIN_PE = HIGH");
    sprintf(OLEDbuf, "Reveil: %d min", config.applicatif.wakeupIntervalPayload);
    OLEDDrawText(1,4, 0, OLEDbuf);
    OLEDDrawScreenTime(0, 0);   // 0, 6 ????
            
// Affichage état debug
    OLEDDrawText(1, 7, 0, DEBUG_LOW_POWER ? "Sleep: ON" : "Sleep: OFF");
        
    debugSerial.println("=== MODE EXPLOITATION ===");
    debugSerial.print("Prochain réveil payload dans: ");
    debugSerial.print(config.applicatif.wakeupIntervalPayload);
    debugSerial.println(" minutes");
  }
}
