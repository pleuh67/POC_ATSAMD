//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*

// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// @brief Met le microcontrôleur en veille
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
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

    // Configuration DS3231 AVANT interruption
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
    
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

// ---------------------------------------------------------------------------*
// @brief Configure les registres de bas niveau pour la basse consommation
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
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
