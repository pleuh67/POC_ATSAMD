//       1         2         3         4         5         6         7        7
//34567890123456789012345678901234567890123456789012345678901234567890123456789
// IMPRESSION 79 COLONES EN TAILLE 12
//
// ---------------------------------------------------------------------------*

// ---------------------------------------------------------------------------*
#define __INIT_DONE
#include "define.h"

// ---------------------------------------------------------------------------*
// @brief Fonction d'interruption RTC
// @param Aucun
// @return void
// ---------------------------------------------------------------------------*
void onRTCAlarm(void) // $ ou £
{
debugSerial.println("\n€");  // €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€


// ISR1 doit être activé, sinon vu toutes les n minutes de ISR2
  if (millis() - loopWDT >  100000) // 100s sans passage par loop() => RESET
  {

debugSerial.println(loopWDT);
   loopWDT  = millis();
debugSerial.println(loopWDT);
    
    softReset();
  }
  else
    debugSerial.println(loopWDT);
  
  
// ALARME 1 : Toutes les secondes (mode programmation)
  if (rtc.alarmFired(1)) 
  {
    rtc.clearAlarm(1);
    if (!alarm1_enabled) 
      return;     // interruption execution code de IRQ1 pendant IRQ2
    wakeup1Sec = alarm1_enabled;  // True si autorisé par IRQ2
debugSerial.print("\n$irq\n");   // $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  }
  
// ALARME 2 : Payload périodique    
  if (rtc.alarmFired(2)) 
  {
    wakeup1Sec = false;    // annule si activée
displayNextPayload = true;
    alarm1_enabled = false;  // Bloquer alarme 1
    wakeupPayload = true;
    rtc.clearAlarm(1);       
    rtc.clearAlarm(2);      
    DS3231setRTCAlarm2(); // Reprogrammer prochaine alarme
debugSerial.println("\n£");  // £££££££££££££££££££££££££££££££££££££££££££££££££££££££££
  }
}
