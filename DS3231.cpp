
#define __INIT_DONE
#include "define.h"


/**
 * @brief Initialise le module RTC DS3231
 * @param Aucun
 * @return void
 */
#define debugSerialinitRTC  // decommenter pour les messages debugSerial
void initRTC(void) 
{
  if (!rtc.begin()) 
  {
#ifdef debugSerialinitRTC
  debugSerial.println("Erreur: RTC introuvable");
OLEDDebugDisplay("RTC introuvable");  
#endif  
    DS3231hardReset();
    if (!rtc.begin()) 
    {
#ifdef debugSerialinitRTC
  debugSerial.println("Erreur: RESET RTC pas OK => FIN");
#endif  
      while (1) 
      {
        blinkBlueLED();
        delay(600);
      }
    }
  }
  else
  {
 #define REFRESHCLOCK 0    // 1 pour mise à l'heure compil forcée   
    if (rtc.lostPower() || REFRESHCLOCK) 
    {
#ifdef debugSerialinitRTC
  debugSerial.println("RTC a perdu l'heure, mise à jour avec l'heure de compilation");
#endif  
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
    }
#ifdef debugSerialinitRTC
    debugSerial.println("RTC initialisé");
#endif  
    OLEDDebugDisplay("initRTC OK");
  }
}

 /**
 * @brief Configure alarme du RTC
 * Alarme 1 : Toutes les secondes (mode programmation)
 * @param Aucun
 * @return void
 */
void DS3231setRTCAlarm1(void) 
{
  return; 

  
// Configuration DS3231 AVANT interruption
//  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.clearAlarm(1);
debugSerial.print("=== CONFIGURATION ALARME1 ");
  if (DEBUG_INTERVAL_1SEC) // && !modeExploitation) 
  {
    DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 1);
    rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);
  }
debugSerial.println("=== 1s DONE ===");
}



// L'appel à partir d'ici parfois beugue et loop ne s'execute plus...
// IRQ 1 et lowpower false
// les IRQ2 suivantes restent opérationnelles
// remis le 29/06: no interrupt et 
 /**
 * @brief Configure alarme du RTC
 * Alarme 2 : Payload toutes les X minutes
 * @param Aucun
 * @return void
 */
void DS3231setRTCAlarm2(void) 
{
// Désactiver TOUTES les interruptions temporairement
    noInterrupts();
    
    // Effacer les flags
// Configuration DS3231 AVANT interruption
//      rtc.writeSqwPinMode(DS3231_OFF);
//      rtc.clearAlarm(1);
// déjà fait dans fonction appelante:      rtc.clearAlarm(2);

   // AJOUTEZ ceci pour être sûr :
  //  rtc.alarmFired(1);  // Lit et efface le flag alarme 1
 //  rtc.alarmFired(2);  // Lit et efface le flag alarme 2
   
    // Reprogrammer A2
debugSerial.println("=== CONFIGURATION ALARMES RTC + INTERRUPTIONS ===");
  // ALARME 2 : Payload toutes les X minutes
  if (DEBUG_WAKEUP_PAYLOAD) 
  {
    nextPayload = rtc.now() + TimeSpan(0, 0, config.applicatif.SendingPeriod, 0);
    rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
    debugSerialPrintReprogNextAlarm(2);
    debugSerialPrintNextAlarm(nextPayload,2);
//OLEDDrawScreenNextPayload(7, 0, nextPayload );  // Status message
  }
//debugSerial.println("=== FIN CONFIGURATION ALARMES + INTERRUPTIONS ===");

// Activer les interruptions
  rtc.writeSqwPinMode(DS3231_OFF); 
  
// Réactiver les interruptions
    interrupts();
}

/**
 * @brief Efface les alarmes du RTC
 * @param Aucun
 * @return void
 */
void clearRTCAlarms(void) 
{
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
}


/**
 * @brief Hard Reset du RTC
 * @param Aucun
 * @return void
 */
void DS3231hardReset() 
{
    // Reset des registres de contrôle
    Wire.beginTransmission(0x68); // Adresse I2C du DS3231
    Wire.write(0x0E); // Registre de contrôle
    Wire.write(0x1C); // Valeur de reset par défaut
    Wire.endTransmission();
    
    Wire.beginTransmission(0x68);
    Wire.write(0x0F); // Registre de status
    Wire.write(0x00); // Clear tous les flags
    Wire.endTransmission();
    
    // Optionnel : remettre à zero l'heure
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

/**
 * @brief Reset du RTC
 * @param Aucun
 * @return void
 */
void DS3231CompleteReset() 
{
    Wire.beginTransmission(0x68);
    
    // Reset de tous les registres principaux
    for(int i = 0; i <= 0x12; i++) 
    {
        Wire.write(i);
        Wire.write(0x00);
        Wire.endTransmission();
        Wire.beginTransmission(0x68);
    }
    
    // Registres de contrôle spéciaux
    Wire.write(0x0E);
    Wire.write(0x1C); // Control register default
    Wire.endTransmission();
    
    delay(100);
    
    // Redémarrer l'oscillateur
    rtc.begin();
}


/**
 * @brief Synchronise l'heure du microcontrôleur avec celle du DS3231
 * @param Aucun
 * @return void
 */
void synchronizeDS3231TimeToMicro(void)
{
  debugSerial.println("=== SYNCHRONISATION DS3231 -> MICRO ===");
    
    // Lecture de l'heure du DS3231
  DateTime heureDS3231 = rtc.now();
    
    // Affichage de l'heure lue
  debugSerial.print("Heure DS3231: ");
  debugSerial.print(heureDS3231.day()); debugSerial.print("/");
  debugSerial.print(heureDS3231.month()); debugSerial.print("/");
  debugSerial.print(heureDS3231.year()); debugSerial.print(" ");
  debugSerial.print(heureDS3231.hour()); debugSerial.print(":");
  debugSerial.print(heureDS3231.minute()); debugSerial.print(":");
  debugSerial.println(heureDS3231.second());
    
    // Note: Sur ATSAMD21, il n'y a pas de RTC interne séparée
    // L'heure système est déjà synchronisée avec le DS3231
    // Cette fonction sert principalement pour le debug et la vérification
    
  debugSerial.println("Synchronisation terminée");
  OLEDDisplayMessageL8("Heure synchronisee", false, false);
}

/**
 * @brief Copie l'heure du DS3231 vers le microcontrôleur avec option de forçage
 * @param forcer True pour forcer la copie même si les heures sont proches
 * @return void
 */
void copyDS3231TimeToMicro(bool forcer)
{
    static unsigned long derniereCopie = 0;
    const unsigned long INTERVALLE_COPIE_MS = 60000; // 1 minute
    
    // Vérifier s'il faut faire la copie
    if (!forcer && (millis() - derniereCopie < INTERVALLE_COPIE_MS))
    {
        return; // Pas encore temps de recopier
    }
    
    debugSerial.println("=== COPIE DS3231 -> MICRO ===");
    
    // Lire l'heure actuelle du DS3231
    DateTime heureDS3231 = rtc.now();
    
    // Obtenir l'heure système actuelle pour comparaison
    DateTime heureSysteme = rtc.now();
    
    // Calculer la différence
    long difference = heureDS3231.unixtime() - heureSysteme.unixtime();
    
    debugSerial.print("Différence DS3231-Système: ");
    debugSerial.print(difference);
    debugSerial.println(" secondes");
    
    // Si la différence est significative ou si forcé, synchroniser
    if (forcer || abs(difference) > 2) // Plus de 2 secondes de différence
    {
        debugSerial.println("Synchronisation nécessaire...");
        
        // Affichage avant synchronisation
        debugSerial.print("Avant - DS3231: ");
        debugSerial.print(heureDS3231.hour()); debugSerial.print(":");
        debugSerial.print(heureDS3231.minute()); debugSerial.print(":");
        debugSerial.print(heureDS3231.second());
        debugSerial.print(" | Système: ");
        debugSerial.print(heureSysteme.hour()); debugSerial.print(":");
        debugSerial.print(heureSysteme.minute()); debugSerial.print(":");
        debugSerial.println(heureSysteme.second());
        
        // Sur ATSAMD21, le "système" utilise déjà le DS3231
        // Cette fonction simule la copie pour des microcontrôleurs avec RTC interne
        
        // Pour un vrai microcontrôleur avec RTC interne, vous ajouteriez ici :
        // setClock(heureDS3231.year(), heureDS3231.month(), heureDS3231.day(),
        //          heureDS3231.hour(), heureDS3231.minute(), heureDS3231.second());
        
        debugSerial.println("Heure copiée du DS3231 vers micro");
        
        sprintf(OLEDbuf, "Sync: %02d:%02d:%02d", 
                heureDS3231.hour(), heureDS3231.minute(), heureDS3231.second());
        OLEDDisplayMessageL8(OLEDbuf, false, false);
        debugSerial.println(OLEDbuf);        
        derniereCopie = millis();
    }
    else
    {
        debugSerial.println("Synchronisation non nécessaire (différence < 2s)");
    }
}

/**
 * @brief Fonction utilitaire pour forcer une synchronisation immédiate
 * @param Aucun
 * @return void
 */
void forcerSynchronisationDS3231(void)
{
    debugSerial.println("=== SYNCHRONISATION FORCEE ===");
    copyDS3231TimeToMicro(true);
    
    // Vérification après synchronisation
    delay(100);
    debugSerialPrintTimeComparison();
}
