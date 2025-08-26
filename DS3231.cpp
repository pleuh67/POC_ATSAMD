
#define __INIT_DONE
#include "define.h"


/**
 * @brief Initialise le module RTC DS3231
 * @param Aucun
 * @return void
 */
void initRTC(void) 
{
  if (!rtc.begin()) 
  {
    debugSerial.println("Erreur: RTC introuvable");
    DS3231hardReset();
    if (!rtc.begin()) 
    {
      debugSerial.println("Erreur: RESET RTC pas OK");
      while (1) delay(10);
    }
  }
   
  if (rtc.lostPower()) 
  {
    debugSerial.println("RTC a perdu l'heure, mise à jour avec l'heure de compilation");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  debugSerial.println("RTC initialisé");
}

/**
 * @brief Retourne l'heure système actuelle
 * @param Aucun
 * @return DateTime Heure actuelle
 */
DateTime getSystemTime(void) 
{
    return rtc.now();
}

 /**
 * @brief Configure les alarmes du RTC
 * Alarme 1 : Toutes les secondes (mode programmation)
 * Alarme 2 : Payload toutes les X minutes
 * @param Aucun
 * @return void
 */
void setRTCAlarms(void) 
{
  clearRTCAlarms();
debugSerial.println("=== CONFIGURATION ALARMES RTC + INTERRUPTIONS ===");
  if (DEBUG_INTERVAL_1SEC) // && !modeExploitation) 
  {
    DateTime nextSecond = rtc.now() + TimeSpan(0, 0, 0, 1);
debugSerial.println("s"); 
rtc.setAlarm1(nextSecond, DS3231_A1_PerSecond);
//    rtc.setAlarm1(nextSecond, DS3231_A1_Second);
//debugSerialPrintNextAlarm(nextSecond, 1);  
  }
  
  // ALARME 2 : Payload toutes les X minutes
  if (DEBUG_WAKEUP_PAYLOAD) 
  {
    DateTime nextPayload = rtc.now() + TimeSpan(0, 0, config.applicatif.wakeupIntervalPayload, 0);
   rtc.setAlarm2(nextPayload, DS3231_A2_Minute);
debugSerialPrintNextAlarm(nextPayload,2);
OLEDDrawScreenNextPayload(6, 0, nextPayload );
  }
  
debugSerial.print("État pin RTC après config alarmes: ");
debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
debugSerial.println("=== FIN CONFIGURATION ALARMES + INTERRUPTIONS ===");
// Activer les interruptions
  rtc.writeSqwPinMode(DS3231_OFF); 
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
void DS3231CompleteReset() {
    Wire.beginTransmission(0x68);
    
    // Reset de tous les registres principaux
    for(int i = 0; i <= 0x12; i++) {
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
 * @brief Affiche heure et status Alarmes sur SerialDebug
 * @param Aucun
 * @return void
 */
void testConnexionDS3231(void)
{
    debugSerial.println("=== TEST CONNEXION DS3231 ===");
    
    // Test lecture heure
    DateTime now = rtc.now();
    debugSerial.print("Heure lue: ");
    debugSerial.print(now.hour()); debugSerial.print(":");
    debugSerial.print(now.minute()); debugSerial.print(":");
    debugSerial.println(now.second());
    
    // Test état alarmes
    debugSerial.print("Alarme 1 fired: ");
    debugSerial.println(rtc.alarmFired(1) ? "OUI" : "NON");
    debugSerial.print("Alarme 2 fired: ");
    debugSerial.println(rtc.alarmFired(2) ? "OUI" : "NON");
    
    // Test pin interruption
    debugSerial.print("Pin interruption état: ");
    debugSerial.println(digitalRead(RTC_INTERRUPT_PIN) ? "HIGH" : "LOW");
    
    debugSerial.println("=== FIN TEST DS3231 ===");
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
    
    if (OLED)
    {
        OLEDDisplayMessageL8("Heure synchronisee", false, false);
    }
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
    DateTime heureSysteme = getSystemTime();
    
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
        
        if (OLED)
        {
            sprintf(OLEDbuf, "Sync: %02d:%02d:%02d", 
                    heureDS3231.hour(), heureDS3231.minute(), heureDS3231.second());
            OLEDDisplayMessageL8(OLEDbuf, false, false);
        }
        
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
    debugSerial.println("=== SYNCHRONISATION FORCÉE ===");
    copyDS3231TimeToMicro(true);
    
    // Vérification après synchronisation
    delay(100);
    printTimeComparison();
}


/**
 * @brief Affiche l'heure système et l'heure RTC côte à côte pour comparaison
 * @param Aucun
 * @return void
 */
void printTimeComparison(void)
{
    DateTime systemTime = getSystemTime();
    DateTime rtcTime = rtc.now();
    debugSerial.println();
    debugSerial.println("=== COMPARAISON DES HORLOGES ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    debugSerial.print("RTC:     ");
    debugSerial.print(rtcTime.day()); debugSerial.print("/");
    debugSerial.print(rtcTime.month()); debugSerial.print("/");
    debugSerial.print(rtcTime.year()); debugSerial.print(" ");
    debugSerial.print(rtcTime.hour()); debugSerial.print(":");
    debugSerial.print(rtcTime.minute()); debugSerial.print(":");
    debugSerial.println(rtcTime.second());
    
    long diff = systemTime.unixtime() - rtcTime.unixtime();
    debugSerial.print("Différence: ");
    debugSerial.print(diff);
    debugSerial.println(" secondes");
    debugSerial.println("===============================");
}


/**
 * @brief Affiche l'heure et la date système sur le port série
 * @param Aucun
 * @return void
 */
void printTimeOndebugSerial(void)
{
    DateTime systemTime = getSystemTime();
    debugSerial.println();
    debugSerial.println("=== HEURE SYSTÈME ===");
    
    debugSerial.print("Système: ");
    debugSerial.print(systemTime.day()); debugSerial.print("/");
    debugSerial.print(systemTime.month()); debugSerial.print("/");
    debugSerial.print(systemTime.year()); debugSerial.print(" ");
    debugSerial.print(systemTime.hour()); debugSerial.print(":");
    debugSerial.print(systemTime.minute()); debugSerial.print(":");
    debugSerial.println(systemTime.second());
    
    print2digits(systemTime.hour());
    debugSerial.print(":");
    print2digits(systemTime.minute());
    debugSerial.print(":");
    print2digits(systemTime.second());
    debugSerial.print(" ");
    print2digits(systemTime.day());
    debugSerial.print("/");
    print2digits(systemTime.month());
    debugSerial.print("/");
    print2digits(systemTime.year());
    debugSerial.println("");
}
